
// code guard
#if !defined(mito_math_Function_h)
#define mito_math_Function_h

#include "../mito.h"

namespace mito {

    // templatized typedef for functors
    template <typename X, typename Y = scalar_t>
    using functor = std::function<Y(const X &)>;

    // We need a class function to explicitly put the return value Y in the template
    template <typename X, typename Y = scalar_t>
    class Function {

      public:
        inline Function(const functor<X, Y> & f) : _functor(f) {}
        inline Function(functor<X, Y> && f) : _functor(f) {}
        // default constructor
        inline Function() = default;
        // copy constructor
        inline Function(const Function &) = default;
        // move constructor
        inline Function(Function &&) = default;
        // destructor
        inline ~Function() {};
        // assignment operator
        inline Function & operator=(const Function &) = default;
        // move operator=
        inline Function & operator=(Function &&) = default;

        inline auto operator()(const X & x) const
        {
            // evaluate _f
            return _functor(x);
        }

        inline auto operator[](int i) const
        {
            return Function<X, scalar_t>(
                [this, i](const X & x) { return this->_functor(x)[i]; });
        }

        // cast operator from Function<X, Y> to functor<X, Y>
        inline operator functor<X, Y>() const { return _functor; }

      private:
        const functor<X, Y> _functor;
    };

    // Algebraic operations on Function
    // TODO: see if expression templates can be used to avoid copies with intermediate results

    // fa + fb
    template <typename X, typename Y>
    Function<X, Y> operator+(const Function<X, Y> & fA, const Function<X, Y> & fB)
    {
        return Function<X, Y>([fA, fB](const X & x) { return fA(x) + fB(x); });
    }

    // fa * fb
    template <typename X, typename Y>
    Function<X, Y> operator*(const Function<X, Y> & fA, const Function<X, Y> & fB)
    {
        return Function<X, Y>([fA, fB](const X & x) { return fA(x) * fB(x); });
    }

    // y * f (inner product)
    template <typename X, typename Y>
    Function<X, typename type<Y>::value> operator*(const Y & y, const Function<X, Y> & f) requires(
        Y::size != 1)
    {
        return Function<X, typename type<Y>::value>([y, f](const X & x) { return y * f(x); });
    }

    // f * y (inner product)
    template <typename X, typename Y>
    Function<X, typename type<Y>::value> operator*(const Function<X, Y> & f, const Y & y) requires(
        Y::size != 1)
    {
        return y * f;
    }

    // a * f
    template <typename X, typename Y>
    Function<X, Y> operator*(const real & a, const Function<X, Y> & f)
    {
        return Function<X, Y>([a, f](const X & x) { return a * f(x); });
    }

    // f * a
    template <typename X, typename Y>
    Function<X, Y> operator*(const Function<X, Y> & f, const real & a)
    {
        return a * f;
    }

    // f / a
    template <typename X, typename Y>
    Function<X, Y> operator/(const Function<X, Y> & f, const real & a)
    {
        return (1.0 / a) * f;
    }

    // -f
    template <typename X, typename Y>
    Function<X, Y> operator-(const Function<X, Y> & f)
    {
        return -1.0 * f;
    }

    // fa - fb
    template <typename X, typename Y>
    Function<X, Y> operator-(const Function<X, Y> & fA, const Function<X, Y> & fB)
    {
        return fA + (-fB);
    }

    // Special algebraic functions for scalar functions
    // a / f
    template <typename X>
    Function<X, scalar_t> operator/(const real & a, const Function<X, scalar_t> & f)
    {
        return Function<X, scalar_t>([a, f](const X & x) { return a / f(x); });
    }

    // f1 / f2
    template <typename X, typename Y>
    Function<X, Y> operator/(const Function<X, Y> & f1, const Function<X, scalar_t> & f2)
    {
        return Function<X, Y>([f1, f2](const X & x) { return f1(x) / f2(x); });
    }

    // a + f
    template <typename X>
    Function<X, scalar_t> operator+(const real & a, const Function<X, scalar_t> & f)
    {
        return Function<X, scalar_t>([a, f](const X & x) { return a + f(x); });
    }

    // f + a
    template <typename X>
    Function<X, scalar_t> operator+(const Function<X, scalar_t> & f, const real & a)
    {
        return a + f;
    }

    // a - f
    template <typename X>
    Function<X, scalar_t> operator-(const real & a, const Function<X, scalar_t> & f)
    {
        return a + (-f);
    }

    // f - a
    template <typename X>
    Function<X, scalar_t> operator-(const Function<X, scalar_t> & f, const real & a)
    {
        return f + (-a);
    }

    // Binary operators for functor<X, Y> and Function<X, Y>, such as:
    // fa + fb
    template <typename X, typename Y>
    Function<X, Y> operator+(const Function<X, Y> & fA, const functor<X, Y> & fB)
    {
        return fA + Function<X, Y>(fB);
    }

    // fa + fb
    template <typename X, typename Y>
    Function<X, Y> operator+(const functor<X, Y> & fA, const Function<X, Y> & fB)
    {
        return fB + fA;
    }

    // fa * fb
    template <typename X, typename Y>
    Function<X, Y> operator*(const Function<X, Y> & fA, const functor<X, Y> & fB)
    {
        return fA * Function<X, Y>(fB);
    }

    // fa * fb
    template <typename X, typename Y>
    Function<X, Y> operator*(const functor<X, Y> & fA, const Function<X, Y> & fB)
    {
        return fB * fA;
    }

    // fa - fb
    template <typename X, typename Y>
    Function<X, Y> operator-(const Function<X, Y> & fA, const functor<X, Y> & fB)
    {
        return fA - Function<X, Y>(fB);
    }

    // fa - fb
    template <typename X, typename Y>
    Function<X, Y> operator-(const functor<X, Y> & fA, const Function<X, Y> & fB)
    {
        return -(fB - fA);
    }

    // f1 / f2
    template <typename X, typename Y>
    Function<X, Y> operator/(const Function<X, Y> & f1, const functor<X, scalar_t> & f2)
    {
        return Function<X, Y>([f1, f2](const X & x) { return f1(x) / f2(x); });
    }

    // f1 / f2
    template <typename X, typename Y>
    Function<X, Y> operator/(const functor<X, Y> & f1, const Function<X, scalar_t> & f2)
    {
        return Function<X, Y>([f1, f2](const X & x) { return f1(x) / f2(x); });
    }

    // Binary operators for Y(const X &) and Function<X, Y>, such as:
    // fa + fb
    template <typename X, typename Y>
    Function<X, Y> operator+(const Function<X, Y> & fA, Y fB(const X &))
    {
        return fA + Function<X, Y>(fB);
    }

    // fa + fb
    template <typename X, typename Y>
    Function<X, Y> operator+(Y fA(const X &), const Function<X, Y> & fB)
    {
        return fB + fA;
    }

    // fa * fb
    template <typename X, typename Y>
    Function<X, Y> operator*(const Function<X, Y> & fA, Y fB(const X &))
    {
        return fA * Function<X, Y>(fB);
    }

    // fa * fb
    template <typename X, typename Y>
    Function<X, Y> operator*(Y fA(const X &), const Function<X, Y> & fB)
    {
        return fB * fA;
    }

    // fa - fb
    template <typename X, typename Y>
    Function<X, Y> operator-(const Function<X, Y> & fA, Y fB(const X &))
    {
        return fA - Function<X, Y>(fB);
    }

    // fa - fb
    template <typename X, typename Y>
    Function<X, Y> operator-(Y fA(const X &), const Function<X, Y> & fB)
    {
        return -(fB - fA);
    }

    // f1 / f2
    template <typename X, typename Y>
    Function<X, Y> operator/(const Function<X, Y> & f1, real f2(const X &))
    {
        return Function<X, Y>([f1, f2](const X & x) { return f1(x) / f2(x); });
    }

    // f1 / f2
    template <typename X, typename Y>
    Function<X, Y> operator/(Y f1(const X &), const Function<X, scalar_t> & f2)
    {
        return Function<X, Y>([f1, f2](const X & x) { return f1(x) / f2(x); });
    }
}

#endif    // mito_math_Function_h

// end of file
