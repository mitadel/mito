#include "../mito.h"

namespace mito {

// f(X,x,t) with (X \in R^D1, x \in R^D2, t \in R) -> R
template <DIM D1, DIM D2 = D1>
class Function {

  public:
    // constructors
    Function(function<vector<D1>, real> f) : _f(f), _Df() {}
    Function(function<vector<D1>, real> f, std::array<function<vector<D1>, real>, D1> Df) : 
        _f(f), _Df(Df) {}

    ~Function() {}

    //inline real operator()(const vector<D1> & X, const vector<D2> & x, real t) const {
    inline real operator()(const vector<D1> & X) const {
        // evaluate _f
        return _f(X);
    }

  public:
    // accessor for function partial derivatives
    inline const function<vector<D1>, real> & Df(size_t i) const {
        // assert the function knows its partial derivatives
        assert (_Df.size() > 0);
        // return the i-th partial derivative
        return _Df[i];
    }

  private:
    // the function
    function<vector<D1>, real> _f;
    // the derivatives of f with respect to X (position in the reference configuration)
    std::array<function<vector<D1>, real>, D1> _Df;
};

// function to compute the Divergence of a function at point X
template<DIM D1>
inline real Div(const Function<D1> & function, const vector<D1> & X) {
    real DivX = 0.0;
    for (size_t i(0); i < D1; ++i) {
        DivX += function.Df(i)(X);
    }
    return DivX;
}

// function to compute the Gradient of a function at point X
template<DIM D1>
inline vector<D1> Grad(const Function<D1> & function, const vector<D1> & X) {
    vector<D1> GradX;
    for (size_t i = 0; i < D1; ++i) {
        GradX[i] = function.Df(i)(X);
    }
    return GradX;
}

}
