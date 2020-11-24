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

    inline vector<D1> Grad(const vector<D1> & X) const {
        vector<D1> GradX;
        for (int i = 0; i < D1; ++i) {
            GradX[i] = _Df[i](X);
        }
        return GradX;
    }

    inline real Div(const vector<D1> & X) const {
    real DivX = 0.0;
    for (int i = 0; i < D1; ++i) {
        DivX += _Df[i](X);
    }
    return DivX;
    }

  private:
    function<vector<D1>, real> _f;
    std::array<function<vector<D1>, real>, D1> _Df;
};

}
