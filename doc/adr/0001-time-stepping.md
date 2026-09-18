## Context
### Design of Front End
Currently, a vision exists regarding how we want the user to interact with `mito`. That is, in broad strokes, a typical frontend consists of three sequential steps:
1. The independent construction of the **discretization** (based on the mesh provided in an external file) and the **physical model**:
    - The discretization contains all data relevant to the domain on which the problem lives - e.g. the mesh, any boundary conditions, the type of discretization used (e.g. finite element, including shape function order, and so on).
    - The physical model contains all data to the physical problem we're solving. This includes e.g. the constituent material. 

    The construction is independent in the sense that these objects are constructed without requiring any knowledge of each other's contents. That is, the discretization is fully independent of the chosen physical model, and vice versa. Long term, this means that the same physical model should be applicable to different discretizations, even from different families (e.g., it should be compatible with both finite elements and finite volumes).

2. The construction of the **discrete model**, which takes in the discretization and physical model and computes the **discrete weak form**. Note that the discrete model in itself agnostic to the physical problem it solves; it can only query the aforementioned physical model for what blocks are present in it. Moreover, note that the weak form is _discrete_ in the sense that its construction requires knowledge of the finite element type; the object represents the elementary contributions to the weak form.

3. The construction of the **time stepper**, which takes in the discrete model and marches is through time, e.g. through an implicit Euler scheme, or a Runge-Kutta scheme, or a Newmark-$\beta$ method.

> #### Design Principle 
> ***
> The physical model should be the _only_ object in the code that has an awareness of the specific physical problem it is being solved. The discretization should _not_ contain any physical information. The discrete model should be the _only_ object in the code that knows how to assemble any discrete operators. The time stepper should be the _only_ object in the code that knows how to assemble the effective system matrices needed to march through time.

### Distinction Between Semi Discrete and Fully Discrete Models
So far, we have referred to simply _the_ discrete model. However, for reasons that will become clear later, it is convenient to distinguish between **semi-discrete** and **fully discrete** models. In the context of FEM, we consider fully discrete models to be of the form
$$
A\bm u=\bm b
$$
which will arise from e.g. steady-state problems, or transient problems discretized with space-time elements. Notably, they do not contain neither spatial derivatives, nor temporal derivatives. 

On the other hand, an example of a semi-discrete model is given by
$$
M\ddot{\bm u}+K\bm u=\bm f
$$
Semi-discrete models are discretized in space, but not yet in time.

> It is also possible to create systems that are first discretized in time, and then in space. However, in the context of `mito`, semi-discrete systems refer to the type of system defined above. 





## Future work
There are 
> #### Regarding the generality of the discrete model
> ***
> In the above, we implicitly assumed that the physical model is able to act on a finite element mesh; that is, in pseudocode, the constructor of the discrete model looks like
> ```
> template<typename physicalModelT, typename discretizationT>
> class DiscreteModel
>{
> template<typename physicalModelT, typename discretizationT>
>  DiscreteModel ( physicalModelT& physicalModelT_, discretizationT& discretizationT_ ) 
>  {
>    discreteWeakform = discreteWeakform ( physicalModelT_.template getBlock<discretizationT::element_type> () );
>  }
>}
>```
> Then, the physical model could, using concepts, 

. For example, if the discretization represents a finite _difference_ discretization, then the physical model could be written in such a way that


### Difference Between Fully Discrete Weak Form and Semi Discrete Weak Form
In the afore, we have only referred to the 


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
- Variation B, where we store semi-discrete operators in the discrete class.

At first glance, variation B may seem preferable as it feels 'natural' to store the semi-discrete matrices in the semi-discrete model class. However, upon closer inspection, the following issue arises. ImplicitEuler requires the presence of a stiffness matrix, capacitance matrix and a load vector. This is inherent to it being a first order time integration scheme. Other time integration schemes may require the storage of an inertial matrix or the damping matrix. Moreover, in case of an explicit time integration scheme, we very likely want to store the _lumped_ mass matrix (which would not be of `matrix_type`, but of `vector_type`). Thus, which matrices need to be stored at all depend on something 'upstream' of the SemiDiscreteModel; therefore, it makes more sense to store them there (i.e., the time stepper). Thus, variation A is preferred.

### Class layouts
On a high-level, four classes need to be designed at this stage:
- The time stepper;
- The discrete system;
- The discrete weak form;
- The physical model.
The discretization has been largely designed already; 

Our time stepper, in this case, looks like
```
template<typename semiDiscreteModelT>
class ImplicitEuler
{
  public:
    auto assemble_semi_discrete_operators () -> void
    {
      _semi_discrete_model.compute_stiffness_matrix ( _stiffness_matrix );
      _semi_discrete_model.compute_capacitance_matrix ( _capacitance_matrix );
      _semi_discrete_model.compute_load_vector ( _load_vector );
    }

    auto assemble_effective_system_operators ( scalar_type dt, vector_type u_previous ) -> void
    {
      _effective_stiffness_matrix = _stiffness_matrix * dt + _mass_matrix;
      _effective_load_vector      = _load_vector * dt + _mass_matrix * u_previous;
    }

  private:
    semiDiscreteModelT _semi_discrete_model;

    matrix_type _effective_stiffness_matrix;
    vector_type _effective_load_vector;

    matrix_type _stiffness_matrix;
    matrix_type _capacitance_matrix;
    vector_type _load_vector;
}
```

In turn, we would want the semi-discrete model to look a little something like
```
template<typename physicalModelT, typename discretizationT>
class SemiDiscreteModel
{
  public:
    auto compute_stiffness_matrix ( matrix_type& stiffness_matrix ) -> void
    {
      for element in _discretization:
      {
        auto elementary_matrix = _physical_model.compute_stiffness_term ( element );
        emplace_elementary_matrix ( elementary_matrix, stiffness_matrix, _discretization );
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
      return GradGradBlock ( material.diffusivity ).compute ( element ) + GradValueBlock ( material.velocity ).compute ( element );
    }

  private:
    materialT _material;

}
``` 
and similar for `compute_capacitance_term` and so on. Note that we completely omit the weak form in this variation. 

### Variation B - stored in discrete model
Our time stepper, in this case, looks like
```
template<typename semiDiscreteModelT>
class ImplicitEuler
{
  public:
    auto assemble_semi_discrete_operators () -> void
    {
      _semi_discrete_model.compute_stiffness_matrix ();
      _semi_discrete_model.compute_capacitance_matrix ();
      _semi_discrete_model.compute_load_vector ();
    }

    auto assemble_effective_system_operators ( scalar_type dt, vector_type u_previous ) -> void
    {
      _effective_stiffness_matrix = _discrete_model.stiffness_matrix () * dt + _discrete_model.capacitance_matrix ();
      _effective_load_vector      = __discrete_model.load_vector () * dt + _discrete_model.capacitance_matrix () * u_previous;
    }

  private:
    semiDiscreteModelT _semi_discrete_model;

    matrix_type _effective_stiffness_matrix;
    vector_type _effective_load_vector;
}
```
In turn, we would the semi-discrete model to look a little something like
```
template<typename physicalModelT, typename discretizationT>
class SemiDiscreteModel
{
  public:
    auto compute_stiffness_matrix () -> void
    {
      for element in _discretization:
      {
        auto elementary_matrix = _physical_model.compute_stiffness_term ( element );
        emplace_elementary_matrix ( elementary_matrix, _stiffness_matrix, _discretization );
      }
    }

    auto stiffness_matrix () -> matrix_type&
    {
      return _stiffness_matrix;
    }

  private:
    physicalModelT _physical_model;
    discretizationT _discretization;
    matrix_type _stiffness_matrix;
}
```
and similar for `capacitance_matrix` and so on. The physical model would have the same interface as in variation A. 

### Comparison
At first glance, variation B may seem preferable as it feels 'natural' to store the semi-discrete matrices in the semi-discrete model class. However, upon closer inspection, the following issue arises. ImplicitEuler requires the presence of a stiffness matrix, capacitance matrix and a load vector. This is inherent to it being a first order time integration scheme. Other time integration schemes may require the storage of an inertial matrix or the damping matrix. Moreover, in case of an explicit time integration scheme, we very likely want to store the _lumped_ mass matrix (which would not be of `matrix_type`, but of `vector_type`). Thus, which matrices need to be stored at all depend on something 'upstream' of the SemiDiscreteModel; therefore, it makes more sense to store them there (i.e., the time stepper). Thus, variation A is preferred.

The second thing that requires attention is the ostensible lack of a (SemiDiscrete)WeakForm, as its existence did not appear needed in either variation. Indeed, it appears that the PhysicalSystem essentially represents the Weakform itself. Consider the original `benchmarks/mito.lib/pdes/poisson.cc` example. There we created the weakform through the following snippet:
```
constexpr auto k = 1.0;
constexpr auto diffusivity = k * mito::functions::identity<coordinates_t, 2>();

auto fem_lhs_block = mito::fem::blocks::diffusion<finite_element_t>(diffusivity);

auto f = 2.0 * std::numbers::pi * std::numbers::pi * mito::functions::sin(std::numbers::pi * x)
        * mito::functions::sin(std::numbers::pi * y);

auto fem_rhs_block = mito::fem::blocks::source<finite_element_t, 2>(f);

auto weakform = mito::fem::weakform(fem_lhs_block, fem_rhs_block);
```
Note that `mito::fem::blocks::diffussion` is just an alias for a `grad_grad_block`.


### Variation C - brute-forcing the weakform into the SemiDiscreteModel 
The interface of ImplicitEuler and the way it interacts with SemiDiscreteModel should not change. However, we can modify the contents of the `SemiDiscreteModel`.

```
template<typename physicalModelT, typename discretizationT>
class SemiDiscreteModel
{
  public:
    SemiDiscreteModel ( physicalModelT physical_model, discretizationT discretization )
    {
      _semiDiscreteWeakform 
      ( 
        StiffnessTerm ( GradGradBlock ( material.diffusivity ) + GradValueBlock ( material.velocity ) ),
        InertiaTerm ( ScalarScalarBlock ( material.rho ) ),
        LoadTerm ( ScalarBlock ( material.f ) )
      )
    }

    auto compute_stiffness_matrix ( matrix_type& stiffness_matrix ) -> void
    {
      for element in _discretization:
      {
        auto elementary_matrix = _physical_model.compute_stiffness_term ( element );
        emplace_elementary_matrix ( elementary_matrix, stiffness_matrix, _discretization );
      }
    }

  private:
    physicalModelT _physical_model;
    discretizationT _discretization;
    semiDiscreteWeakformT _semiDiscreteWeakform;
}
```
where we use mixins to create our SemiDiscreteWeakform.
```
template<typename elementT>
class StiffnessTerm
{
  ;
}

template<typename Terms...>
class SemiDiscreteWeakform : public Terms...
{
  ;
}
```
But no actually, this does not work: the knowledge of when to use GradGradBlock is stored in physical model. So we need to look at whether we need to rewrite that class:

### Variation D - brute-forcing the weakform into the physical model
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
    semiDiscreteWeakformT _semiDiscreteWeakform;

}
```
However, this also does not work - SemiDiscreteWeakform _requires_ knowledge of the element-type to be able to construct the blocks - hence why `compute_stiffness_term` does work because we pass it only as a method argument. But if we actually want to store SemiDiscreteWeakform inside SomePhysicalModel, then we need to give the full SomePhysicsModel-class access to the element-type we're using. As mentioned at the beginning, this is unnegotiable. 

### Variation E - placing the SemiDiscreteWeakform in the SemiDiscreteModel
Let us start from the top. 

Our time stepper, ideally, looks like the following:
```
template<typename semiDiscreteModelT, typename mathBackendT>
class ImplicitEuler
{
  public:
    auto assemble_semi_discrete_operators () -> void
    {
      _semi_discrete_model.compute_stiffness_matrix ( _stiffness_matrix );
      _semi_discrete_model.compute_capacitance_matrix ( _capacitance_matrix );
      _semi_discrete_model.compute_load_vector ( _load_vector );
    }

    auto assemble_effective_system_operators ( scalar_type dt, vector_type u_previous ) -> void
    {
      _effective_stiffness_matrix = _stiffness_matrix * dt + _mass_matrix;
      _effective_load_vector      = _load_vector * dt + _mass_matrix * u_previous;
    }

  private:
    semiDiscreteModelT _semi_discrete_model;

    mathBackendT::matrix_type _effective_stiffness_matrix;
    mathBackendT::vector_type _effective_load_vector;

    mathBackendT::matrix_type _stiffness_matrix;
    mathBackendT::matrix_type _capacitance_matrix;
    mathBackendT::vector_type _load_vector;
}
```
In turn, our SemiDiscreteModel should look like
```
template<typename physicalModelT, typename discretizationT>
class SemiDiscreteModel
{
    using weakform_t = dec

  public:
    SemiDiscreteModel ( physicalModelT physical_model, discretizationT discretization )
    {
      _semiDiscreteWeakform 
      ( 
        StiffnessTerm ( GradGradBlock ( material.diffusivity ) + GradValueBlock ( material.velocity ) ),
        InertiaTerm ( ScalarScalarBlock ( material.rho ) ),
        LoadTerm ( ScalarBlock ( material.f ) )
      )
    }

    auto compute_stiffness_matrix ( matrix_type& stiffness_matrix ) -> void
    {
      for element in _discretization:
      {
        auto elementary_matrix = _physical_model.compute_stiffness_term ( element );
        emplace_elementary_matrix ( elementary_matrix, stiffness_matrix, _discretization );
      }
    }

  private:
    physicalModelT _physical_model;
    discretizationT _discretization;
    semiDiscreteWeakformT _semiDiscreteWeakform;
}
```



## Scope limits
A few things 

We could accomplish this in two ways:
- The backend uses the same layout as the frontend. 
- The backend is developed independently, and we design the API to be a wrapper around the backend to obtain this frontend. 
It is not settled at this stage which option we go for. However, ceteris paribus, the former should be preferred, so that when researchers switch from the front-end to the back-end, they understand the structure of the code. Thus, for now we assume we wish the former, and reconsider if we come across any insurmountable problems. 
