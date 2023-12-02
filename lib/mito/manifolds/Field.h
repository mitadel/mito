
// code guard
#if !defined(mito_manifolds_Field_h)
#define mito_manifolds_Field_h


namespace mito::manifolds {

    /*
     * A field of vectors or forms. The fields implements {operator()} for an input type
     * {coordinates_t}, which returns the value of the field at that physical location.
     */

    template <class F>
    class Field {
      private:
        // the type in input to the field (e.g. where the field is defined)
        using input_type = input<F>::input_type;
        // the type of field (e.g. vector field, form field, ...)
        using output_type = input<F>::output_type;

      public:
        // the type of coordinates
        using coordinates_type = input_type;
        // publish the dimension of the physical space
        static constexpr int dim = coordinates_type::dim;
        // publish my template parameter
        using function_type = F;

      public:
        // constructor
        constexpr Field(F f) : _f { f } {}

        // the value of the field at position {x}
        constexpr auto operator()(const coordinates_type & x) const -> output_type { return _f(x); }

      private:
        // the function assigning the value of the field at each point
        F _f;
    };
}


#endif    // mito_manifolds_Field_h

// end of file
