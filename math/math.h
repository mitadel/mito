#include "../mito.h"
#include "../elements/elements.h"
#include "../quadrature/quadrature.h"

// TOFIX: Check the std::move throughout. I am not sure they do what I expect them to.

namespace mito {

// TODO: Once we converge on what ScalarField, and VectorField look like, we should expand this to
//       second and fourth order tensors.

// f(X,x,t) with (X \in R^D, x \in R^D2, t \in R) -> R
template <DIM D, DIM D2 = D>
class ScalarField {

    // typedef for a scalar valued function
    using function_t = function<vector<D>>;

  public:
    // constructors
    ScalarField(function_t f) : _f(f), _Df() {}
    ScalarField(function_t f, std::array<function_t, D> Df) : 
        _f(f), _Df(Df) {}

    ~ScalarField() {}

    //inline real operator()(const vector<D> & X, const vector<D2> & x, real t) const {
    inline real operator()(const vector<D> & X) const {
        // evaluate _f
        return _f(X);
    }

    inline std::vector<real> operator()(const std::vector<vector<D > > & X) const {
        std::vector<real> values(X.size(), 0.0);
        // evaluate operator() at all elements of X 
        for (auto i = 0; i < X.size(); ++i) {
            values[i] = operator()(X[i]);
        }
        return std::move(values);
    }

  public:
    // accessor for function partial derivatives
    inline const function_t & Df(dim_t i) const {
        // assert there exists the i-th partial derivative
        assert (i < _Df.size());
        // return the i-th partial derivative
        return _Df[i];
    }

  private:
    // the function
    function_t _f;
    // the derivatives of f with respect to X (position in the reference configuration)
    std::array<function_t, D> _Df;
};

// template on vector dimension N, spatial dimension D
template <DIM N, DIM D>
class VectorField
{
  public:
    VectorField(std::array<ScalarField<D>, N> components) : _components(components) {}
    ~VectorField() {}

    inline vector<N> operator()(const vector<D> & X) const {
        vector<N> result;
        for (int i = 0; i < N; ++i) {
            result[i] = _components[i](X);
        }
        return std::move(result);
    }

    inline const ScalarField<D> & operator[](dim_t i) const {
        // assert there exists the i-th partial derivative
        assert (i < _components.size());
        // return the i-th component
        return _components[i];
    }

    inline ScalarField<D> & operator[](dim_t i) {
        // assert there exists the i-th partial derivative
        assert (i < _components.size());
        // return the i-th component
        return _components[i];
    }

  private:
    std::array<ScalarField<D>, N> _components;
};

// function to compute the Divergence of a vector field at point X 
template<DIM D>
inline real divX(const VectorField<D, D> & function, const vector<D> & X) {
    real result = 0.0;
    for (auto i(0); i < D; ++i) {
        result += function[i].Df(i)(X);
    }
    return result;
}

// function to compute the gradient of a scalar field with respect to the reference configuration 
// at point X
template<DIM D>
inline vector<D> gradX(const ScalarField<D> & function, const vector<D> & X) {
    vector<D> result;
    for (auto i = 0; i < D; ++i) {
        result[i] = function.Df(i)(X);
    }
    return std::move(result);
}

// helper function to compute the gradient of a vector field with respect to the reference 
// configuration at point X (template with index sequence)
template<DIM D, std::size_t... I>
inline VectorField<D, D> _gradX(const ScalarField<D> & function, std::index_sequence<I...>) {
    return VectorField<D, D>({ function.Df(I)... });
}

// function to compute the gradient of a vector field with respect to the reference configuration 
// at point X
template<DIM D>
inline VectorField<D, D> gradX(const ScalarField<D> & function) {
    return _gradX(function, std::make_index_sequence<D>{});
}

// TODO: Keep in mind that we will need integrator and the above defined fields to compute integrals 
//       of contact forces down the road. Do we have enough machinery for that? 

// template with respect to element type T and to degree of exactness r of quadrature rule 
template<class ElementType, int r>
class Integrator 
{
    static const DIM D = ElementType::physicalDim;
    static const DIM d = ElementType::parametricDim;

  public:
    Integrator(const Elements<D> & elements) 
        : _elements(elements), _quadRule(QuadratureRule<ElementType, r>())
        {}

    real integrate(const ScalarField<D> & function) {
        std::cout << "integrating ... " << std::endl;

        std::vector<vector<D> > coordinates = _quadRule.quadraturePoints(_elements);
        std::vector<real> values = function(coordinates); 

        real result = 0.0;

        // TOFIX: Typedef elem_t, quad_t, dim_t so as to give a compilation error if misused
        //        Also: consider using p2::grid to decouple memory from indexing. 
        //        Syntax is as follows:
        //              index_t i {e, q, j};
        //              values[i];

        for (auto e = 0; e < _elements.nElements(); ++e) {
            for (auto q = 0; q < _quadRule.nQuad(); ++q) {
                result += values[e * _quadRule.nQuad() + q] * _quadRule.weight(q); 
                    /* * _elements.jacobian(e); Jacobians are dummy for now and identically zero */ 
            }
        }

        return result;
    }

  private:
    QuadRule<ElementType> _quadRule;
    const Elements<D > & _elements;

};

}

// end of file
