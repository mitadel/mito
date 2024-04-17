// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_geometry_Coordinates_h)
#define mito_geometry_Coordinates_h


// forward declarations of {operator+} and {operator*}
// NOTE: these forward declarations are needed so the compiler knows they are template functions
//       when we {friend} these functions with the class {Coordinates}
namespace mito::geometry {

    // addition of coordinates xA + xB
    template <int D, CoordinateType coordT>
    constexpr auto operator+(const Coordinates<D, coordT> &, const Coordinates<D, coordT> &)
        -> Coordinates<D, coordT>;

    // multiplication of coordinates a * xB
    template <int D, CoordinateType coordT>
    constexpr auto operator*(const scalar_t &, const Coordinates<D, coordT> &)
        -> Coordinates<D, coordT>;

    // coordinates subtraction returning vectors
    template <int D, CoordinateType coordT>
    constexpr auto operator-(const Coordinates<D, coordT> &, const Coordinates<D, coordT> &)
        -> mito::vector_t<D>;

    // operator less than
    template <int D, CoordinateType coordT>
    constexpr auto operator<(const Coordinates<D, coordT> &, const Coordinates<D, coordT> &)
        -> bool;
}


namespace mito::geometry {

    // class for a set of coordinates in a {coordT} coordinates system (e.g. CARTESIAN, POLAR, ...)
    // in {D} dimensions
    template <int D, CoordinateType coordT>
    class Coordinates {
      private:
        // my underlying array type
        using array_t = mito::vector_t<D>;

      public:
        // alias for me
        using coordinates_type = Coordinates<D, coordT>;
        // publish the dimension of the physical space
        static constexpr int dim = D;

      public:
        // default constructor
        constexpr Coordinates() : _array() {}

        // constructor
        constexpr Coordinates(mito::scalar_t (&&coords)[D]) : _array(std::move(coords)) {}

        // constructor
        constexpr Coordinates(const array_t coords) : _array(coords) {}

        // constructor from parameter pack
        template <class... T>
        constexpr Coordinates(T... coords)
        requires(sizeof...(T) == D)
            : _array{ coords... }
        {}

        // destructor
        constexpr ~Coordinates() {}

        // default copy constructor
        constexpr Coordinates(const Coordinates &) = default;

        // default move constructor
        constexpr Coordinates(Coordinates &&) noexcept = default;

        // default assignment operator
        constexpr Coordinates & operator=(const Coordinates &) = default;

        // default move assignment operator
        constexpr Coordinates & operator=(Coordinates &&) noexcept = default;

      public:
        // friendship with addition of coordinates
        friend constexpr auto operator+<>(const coordinates_type &, const coordinates_type &)
            -> coordinates_type;

        // friendship with scaling of coordinates
        friend constexpr auto operator*<>(const scalar_t &, const coordinates_type &)
            -> coordinates_type;

        // friendship with {operator==} for coordinates
        friend constexpr auto operator==<>(const coordinates_type &, const coordinates_type &)
            -> bool;

        // friendship with {operator-} for coordinates
        friend constexpr auto operator-<>(const coordinates_type &, const coordinates_type &)
            -> mito::vector_t<D>;

        // friendship with {operator<} for coordinates
        friend constexpr auto operator< <>(const coordinates_type &, const coordinates_type &)
            -> bool;

        // cast to the underlying array of coordinates
        constexpr operator array_t() const { return _array; }

      public:
        // components accessor
        constexpr auto operator[](int i) const -> scalar_t { return _array[i]; }

        auto print() const -> void
        {
            // print the coordinates of the point
            std::cout << "Coordinates: " << _array << std::endl;

            // all done
            return;
        }

      private:
        // the array of coordinates
        array_t _array;
    };

    template <int D, CoordinateType coordT>
    std::ostream & operator<<(std::ostream & os, const Coordinates<D, coordT> & coord) noexcept
    {
        // print the coordinates
        coord.print();

        // all done
        return os;
    }

}    // namespace mito


#endif    // mito_geometry_Coordinates_h

// end of file
