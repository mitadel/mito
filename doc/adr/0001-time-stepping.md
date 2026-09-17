## Context
Currently, a vision exists regarding how we want the user to interact with `mito`. That is, a typical frontend looks like:
1. The independent construction of the discretisation (based on the mesh provided in an external file) and the physical model:
    - The discretization contains all data relevant to the domain on which the problem lives - e.g. the mesh, any boundary conditions, the type of discretization used (e.g. finite element, including shape function order, and so on).
    - The physical model contains all data to the physical problem we're solving. This includes e.g. the constituent material. 

    Independent in the sense that these objects are constructed without requiring any knowledge of each other's contents. That is, the discretization is fully independent of the chosen physical model, and vice versa. Long term, this means that the same physical model should be applicable to different discretizations, even from different families (e.g., it should be compatible with both finite elements and finite volumes).

> #### Design Principle 
> ***
> Note that the physical model should be the _only_ object in the code that has an awareness of the specific physical problem it is being solved. Similarly, the discretization should _not_ contain any physical information. 

2. The construction of the discrete model, which .  . Note that the discrete model is in itself agnostic to the physical problem it solves; it can only query the aforementioned physical model to understand 
In practice, this would look like
```
DiscreteModel::compute
```

## Solution
The frontend was designed in a 'bottom-up' approach. For the design of the time stepping architecture, let us consider how the backend would look like. For sake of simplicity, let us consider the implementation of an implicit Euler scheme. That is, for the first-order system of ODEs, 
$$
M\dot{\bm u}+K\bm u=\bm F
$$
where $M$ is the capacitance matrix, $K$ is the stiffness matrix and $F$ is the load vector; for sake of convenience, we refer to these as the **semi-discrete operators**. 
The time stepping scheme is given by
$$
\left(M+\Delta t K\right)\bm{u}^{n+1}=\Delta t\bm F^{n+1}+M\bm u^{n}
$$
such that we have the equivalent linear system
$$
A\bm{u}^{n+1}=\bm b
$$
with effective system operators
$$
A=M+\Delta t K\\
\bm b =\Delta t \bm F^{n+1}+M\bm u^n
$$
Two questions immediately arise: 
- Which matrices do we need to store?
- Where do we store these matrices? 

Regarding the former, ostensibly, it should be possible to store _only_ the effective system operators. However, consider the case we want to use an adaptive time step; that is, $\Delta t$ is not constant. Then we would frequently need to rebuild $A$ and $\bm b$. If we do not store the semi-discrete operators anywhere, we would need to rebuild them every time $\Delta t$ changes; even if we _know_ their contents do not change. Therefore, for now, let us assume that we store both the semi-discrete and effective system operators _somewhere_. 

Let us thus consider _where_ these operators are stored:
- The effective system operators inarguably need to be stored in the time stepper; they depend fully on the chosen time stepper and thus it is the only logical place for them. 
- The semi-discrete operators are evidently a property of the discrete model; it is the composition of the physical model and the discretization that determines the contents of the the semi-discrete operators. However, just because a class has the intelligence to _compute_ something, does not imply that it has the need to _store_ it too. 

Let us therefore work out the two variations: 
- Variation A, where we store semi-discrete operators in the time-stepper class.


We can define two variations - one where the matrices are stored in the time stepper; the other where they are stored in the discrete model. 

### Variation A - stored in time stepper
Our time stepper, in this case, looks like
```
template<typename discreteModelT>
class ImplicitEuler
{
  public:
    auto assemble_semi_discrete_operators () -> void
    {
      _discrete_model.compute_stiffness_matrix ( _stiffness_matrix );
      _discrete_model.compute_capacitance_matrix ( _capacitance_matrix );
      _discrete_model.compute_load_vector ( _load_vector );
    }

    auto assemble_effective_system_operators ( scalar_type dt, vector_type u_previous ) -> void
    {
      _effective_stiffness_matrix = _stiffness_matrix * dt + _mass_matrix;
      _effective_load_vector      = _load_vector * dt + _mass_matrix * u_previous;
    }

  private:
    discreteModelT _discrete_model;

    matrix_type _effective_stiffness_matrix;
    vector_type _effective_load_vector;

    matrix_type _stiffness_matrix;
    matrix_type _capacitance_matrix;
    vector_type _load_vector;
}
```

In turn, we would the discrete model to look a little something like
```
template<typename physicalModelT, typename discretizationT>
class DiscreteModel
{
  public:
    auto compute_stiffness_matrix ( matrix_type& _stiffness_matrix ) -> void
    {
      for element in _discretization:
      {
        auto elementary_matrix = _physical_model.compute_stiffness_term ( element );
        emplace_elementary_matrix ( elementary_matrix, _discretization );
      }
    }

  private:
    physicalModelT _physical_model;
    discretizationT _discretization;
}
```
and similar for `compute_capacitance_matrix` and so on. In turn, the physical model would look something like this:
```
class SomePhysicsModel
{
  public:
    template<typename materialT>
    SomePhysicsModel ( materialT material ) : _material ( material ) {;};

    template<typename elementT>
    auto compute_stiffness_term ( const elementT& element ) -> elementT::elementary_matrix
    {
      return GradGradBlock ( material.diffusivity ) + GradValueBlock ( material.velocity );
    }

  private:
    materialT _material;

}
``` 
and similar for `compute_capacitance_term` and so on. Note that we completely omit the weak form in this variation. 

### Variation B - stored in discrete model
Our time stepper, in this case, looks like
```
template<typename discreteModelT>
class ImplicitEuler
{
  public:
    auto assemble_semi_discrete_operators () -> void
    {
      _discrete_model.compute_stiffness_matrix ();
      _discrete_model.compute_capacitance_matrix ();
      _discrete_model.compute_load_vector ();
    }

    auto assemble_effective_system_operators ( scalar_type dt, vector_type u_previous ) -> void
    {
      _effective_stiffness_matrix = _discrete_model.stiffness_matrix () * dt + _discrete_model.mass_matrix ();
      _effective_load_vector      = __discrete_model.load_vector () * dt + _discrete_model.mass_matrix () * u_previous;
    }

  private:
    discreteModelT _discrete_model;

    matrix_type _effective_stiffness_matrix;
    vector_type _effective_load_vector;
}
```


## Scope limits
A few things 

We could accomplish this in two ways:
- The backend uses the same layout as the frontend. 
- The backend is developed independently, and we design the API to be a wrapper around the backend to obtain this frontend. 
It is not settled at this stage which option we go for. However, ceteris paribus, the former should be preferred, so that when researchers switch from the front-end to the back-end, they understand the structure of the code. Thus, for now we assume we wish the former, and reconsider if we come across any insurmountable problems. 
