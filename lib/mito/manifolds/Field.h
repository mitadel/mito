// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


// code guard
#if !defined(mito_manifolds_Field_h)
#define mito_manifolds_Field_h


namespace mito::manifolds {

    /*
     * A field of vectors or forms. The fields implements {operator()} for an input type
     * {coordinates_t}, which returns the value of the field at that physical location.
     */

    template <geometry::CoordinateType coordT, functions::function_c F>
    requires(geometry::coordinates_c<typename F::input_type>)
    class Field {
      private:
        // the type of underlying function
        using function_type = F;

      public:
        // the type in input to the field (e.g. where the field is defined)
        using input_type = function_type::input_type;
        // the type of field (e.g. vector field, form field, ...)
        using output_type = function_type::output_type;
        // the coordinate type
        static constexpr geometry::CoordinateType coordinate_type = coordT;
        // the type of coordinates
        using coordinates_type = input_type;

      public:
        // constructor
        constexpr Field(function_type f) : _f{ std::move(f) } {}

        // the value of the field at position {x}
        constexpr auto operator()(const coordinates_type & x) const -> output_type { return _f(x); }

        // accessor to the underlying function
        constexpr auto function() const -> const function_type & { return _f; }

      private:
        // the function assigning the value of the field at each point
        function_type _f;
    };
}


#endif    // mito_manifolds_Field_h

// end of file
