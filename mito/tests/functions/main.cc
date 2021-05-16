#include "../../mito.h"

template <typename X, typename Y>
class Function {

  public:
    Function(const mito::function<X, Y> & f) : _function(f) {}

    // inline real operator()(const vector<D> & X, const vector<D2> & x, real t) const {
    inline auto operator()(const X & x) const
    {
        // evaluate _f
        return _function(x);
    }

    inline const mito::function<X, Y> & get() const { return _function; }

  private:
    const mito::function<X, Y> _function;
};

template <typename X, typename Y>
Function<X, Y>
operator+(const Function<X, Y> & functionA, const Function<X, Y> & functionB)
{
    auto fA = functionA.get();
    auto fB = functionB.get();

    Function<X, Y> result([fA, fB](const mito::vector<2> & x) { return fA(x) + fB(x); });

    return result;
}

int
main()
{
    // a scalar function
    Function<mito::vector<2>, mito::real> function1(
        [](const mito::vector<2> & x) { return cos(x[0] * x[1]); });

    // a scalar function
    Function<mito::vector<2>, mito::real> function2([](const mito::vector<2> & x) { return 5; });

    // the sum of the two scalar functions
    Function<mito::vector<2>, mito::real> function3 = function1 + function2;

    // a point in the reference configuration
    mito::vector<2> x = { 0.0, 0.0 };

    std::cout << function1(x) << std::endl;
    std::cout << function2(x) << std::endl;
    std::cout << function3(x) << std::endl;

    auto function4 = function1 + function2 + function3;
    std::cout << function4(x) << std::endl;

    return 0;
}