
// code guard
#if !defined(mito_manifolds_Tensor_h)
#define mito_manifolds_Tensor_h


namespace mito::manifolds {

    // Class for {Tensor}:
    //      - N1 is the number of input one-forms
    //      - N2 is the number of input one-vectors
    //
    template <class F>
    // TOFIX: requires that the input arguments of {F} are {N1} one-forms and {N2} vectors
    class Tensor {
      private:
        // TOFIX:
        // constexpr int N1 = ... get N1 and N2 from the input of {F}
        // constexpr int N2 = ... get N1 and N2 from the input of {F}
        // the number of indices
        // static constexpr int D = N1 + N2;

        //   public:
        // static constexpr int dim = D;

      public:
        // constructor
        constexpr Tensor(F f) : _f { f } {}

        // contraction operator
        template <typename... argsT>
        constexpr auto operator()(argsT... args) const
        {
            return _f(args...);
        }

      private:
        // the function specifying how the tensor contracts with one-forms and one-vectors
        F _f;
    };
}


#endif    // mito_manifolds_Field_h

// end of file
