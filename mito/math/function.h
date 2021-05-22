
#ifndef __MITO__FUNCTION__
#define __MITO__FUNCTION__

#include "../mito.h"

namespace mito {

    // templatized typedef for functors
    template <typename X, typename Y = real>
    using functor = std::function<Y(const X &)>;

    // We need a class function to explicitly put the return value Y in the template
    template <typename X, typename Y = real>
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
            return mito::Function<X, real>([this, i](const X & x) { return this->_functor(x)[i]; });
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
    mito::Function<X, Y> operator+(const mito::Function<X, Y> & fA, const mito::Function<X, Y> & fB)
    {
        return mito::Function<X, Y>([fA, fB](const X & x) { return fA(x) + fB(x); });
    }

    // fa * fb
    template <typename X, typename Y>
    mito::Function<X, Y> operator*(const mito::Function<X, Y> & fA, const mito::Function<X, Y> & fB)
    {
        return mito::Function<X, Y>([fA, fB](const X & x) { return fA(x) * fB(x); });
    }

    // a * f
    template <typename X, typename Y>
    mito::Function<X, Y> operator*(const real & a, const mito::Function<X, Y> & f)
    {
        return mito::Function<X, Y>([a, f](const X & x) { return a * f(x); });
    }

    // f * a
    template <typename X, typename Y>
    mito::Function<X, Y> operator*(const mito::Function<X, Y> & f, const real & a)
    {
        return a * f;
    }

    // f / a
    template <typename X, typename Y>
    mito::Function<X, Y> operator/(const mito::Function<X, Y> & f, const real & a)
    {
        return (1.0 / a) * f;
    }

    // -f
    template <typename X, typename Y>
    mito::Function<X, Y> operator-(const mito::Function<X, Y> & f)
    {
        return -1.0 * f;
    }

    // fa - fb
    template <typename X, typename Y>
    mito::Function<X, Y> operator-(const mito::Function<X, Y> & fA, const mito::Function<X, Y> & fB)
    {
        return fA + (-fB);
    }

    // Special algebraic functions for scalar functions
    // a / f
    template <typename X>
    mito::Function<X, mito::real> operator/(const real & a, const mito::Function<X, mito::real> & f)
    {
        return mito::Function<X, mito::real>([a, f](const X & x) { return a / f(x); });
    }

    // a + f
    template <typename X>
    mito::Function<X, mito::real> operator+(const real & a, const mito::Function<X, mito::real> & f)
    {
        return mito::Function<X, mito::real>([a, f](const X & x) { return a + f(x); });
    }

    // f + a
    template <typename X>
    mito::Function<X, mito::real> operator+(const mito::Function<X, mito::real> & f, const real & a)
    {
        return a + f;
    }

    // a - f
    template <typename X>
    mito::Function<X, mito::real> operator-(const real & a, const mito::Function<X, mito::real> & f)
    {
        return a + (-f);
    }

    // f - a
    template <typename X>
    mito::Function<X, mito::real> operator-(const mito::Function<X, mito::real> & f, const real & a)
    {
        return f + (-a);
    }
}

#endif    // __MITO__FUNCTION__

// end of file
