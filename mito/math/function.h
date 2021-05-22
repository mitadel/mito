
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

        // inline real operator()(const vector<D> & X, const vector<D2> & x, real t) const {
        inline auto operator()(const X & x) const
        {
            // evaluate _f
            return _functor(x);
        }

        // cast operator from Function<X, Y> to functor<X, Y>
        inline operator functor<X, Y>() const { return _functor; }

      private:
        const functor<X, Y> _functor;
    };

    // TODO: see if expression templates can be used to avoid copies with intermediate results
    template <typename X, typename Y>
    mito::Function<X, Y> operator+(const mito::Function<X, Y> & fA, const mito::Function<X, Y> & fB)
    {
        return mito::Function<X, Y>([fA, fB](const X & x) { return fA(x) + fB(x); });
    }
}

#endif    // __MITO__FUNCTION__

// end of file
