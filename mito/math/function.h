
#ifndef __MITO__FUNCTION__
#define __MITO__FUNCTION__

#include "../mito.h"

namespace mito {

    // templatized typedef for functions
    template <typename X, typename Y = real>
    using function_t = std::function<Y(const X &)>;

    // We need a class function to explicitly put the return value Y in the template
    template <typename X, typename Y = real>
    class Function {

      public:
        inline Function(const function_t<X, Y> & f) : _function(f) {}
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
            return _function(x);
        }

        // cast operator from Function<X, Y> to function_t<X, Y>
        inline operator function_t<X, Y>() const { return _function; }

      private:
        const function_t<X, Y> _function;
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
