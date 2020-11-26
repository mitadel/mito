#include "../mito.h"
#include "../elements/elements.h"
//#include "../quadrature/quadrature.h"

namespace mito {

// f(X,x,t) with (X \in R^D, x \in R^D2, t \in R) -> R
template <DIM D, DIM D2 = D>
class ScalarField {

    // typedef for a scalar valued function
    typedef function<vector<D>, real > function_t;

  public:
    // constructors
    ScalarField(function_t f) : _f(f), _Df() {}
    ScalarField(function_t f, std::array<function_t, D> Df) : 
        _f(f), _Df(Df) {}
    // TOFIX: This constructor should be deleted, but it is needed. See comment below. 
    ScalarField() : _f(), _Df() {}

    ~ScalarField() {}

    //inline real operator()(const vector<D> & X, const vector<D2> & x, real t) const {
    inline real operator()(const vector<D> & X) const {
        // evaluate _f
        return _f(X);
    }

  public:
    // accessor for function partial derivatives
    inline const function_t & Df(size_t i) const {
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
template <size_t N, DIM D>
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

    inline const ScalarField<D> & operator[](size_t i) const {
        // assert there exists the i-th partial derivative
        assert (i < _components.size());
        // return the i-th component
        return _components[i];
    }

    inline ScalarField<D> & operator[](size_t i) {
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
inline real Div(const VectorField<D, D> & function, const vector<D> & X) {
    real DivX = 0.0;
    for (size_t i(0); i < D; ++i) {
        DivX += function[i].Df(i)(X);
    }
    return DivX;
}

// function to compute the Gradient of a scalar field at point X
template<DIM D>
inline vector<D> Grad(const ScalarField<D> & function, const vector<D> & X) {
    vector<D> GradX;
    for (size_t i = 0; i < D; ++i) {
        GradX[i] = function.Df(i)(X);
    }
    return std::move(GradX);
}

// function to compute the Gradient of a vector field at point X
template<DIM D>
inline VectorField<D, D> Grad(const ScalarField<D> & function) {
    // TOFIX: This is the reason why we need a simple constructor with no arguments...
    //       We could avoid it if we could write something like:
    //          std::array<ScalarField<D>, D> components{function.Df(0), ..., function.Df(D-1)};
    // e.g.:
    //std::array<ScalarField<D>, D> components {function.Df(0), function.Df(1)};
    std::array<ScalarField<D>, D> components;
    for (size_t i = 0; i < D; ++i) {
        components[i] = function.Df(i);
    }

    VectorField<D, D> GradX(std::move(components));
    return std::move(GradX);
}

template<DIM D>
class Integrator 
{
  public:
    Integrator(const Elements<D> & elements, size_t degree) 
        /*: _quadRule(elementSet.type() + std::to_string(degree))*/ {
    }

    real integrate(const ScalarField<D> & function) {
        std::cout << "integrating ... " << std::endl;
        return 0.0;
    }

  private:
    //QuadRule<D> _quadRule;

};

}

// end of file
