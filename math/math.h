#include "../mito.h"

namespace mito {

// f(X,x,t) with (X \in R^D1, x \in R^D2, t \in R) -> R
template <DIM D1, DIM D2 = D1>
class Function {

  public:
    Function(function<vector<D1>, real> f, std::array<function<vector<D1>, real>, D1> Df) : 
        _f(f), _Df(Df) {}

    ~Function() {}

    //inline real operator()(const vector<D1> & X, const vector<D2> & x, real t) const {
    inline real operator()(const vector<D1> & X) const {
        return _f(X);
    }

  public:
    const function<vector<D1>, real> & Df(size_t i) const {return _Df[i];}

  private:
    function<vector<D1>, real> _f;
    std::array<function<vector<D1>, real>, D1> _Df;
};

template<DIM D1>
inline real Div(const Function<D1> & function, const vector<D1> & X) {
    real DivX = 0.0;
    for (size_t i(0); i < D1; ++i) {
        DivX += function.Df(i)(X);
    }
    return DivX;
}

template<DIM D1>
inline vector<D1> Grad(const Function<D1> & function, const vector<D1> & X) {
    vector<D1> GradX;
    for (size_t i = 0; i < D1; ++i) {
        GradX[i] = function.Df(i)(X);
    }
    return GradX;
}

}
