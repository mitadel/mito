// code guard
#if !defined(mito_geometry_Coordinates_h)
#define mito_geometry_Coordinates_h


// forward declarations of {operator+} and {operator*}
// NOTE: these forward declarations are needed so the compiler knows they are template functions
//       when we {friend} these functions with the class {Coordinates}
namespace mito::geometry {

    // addition of coordinates xA + xB
    template <int D, CoordinateSystem coordT>
    constexpr auto operator+(const Coordinates<D, coordT> & xA, const Coordinates<D, coordT> & xB)
        -> Coordinates<D, coordT>;

    // multiplication of coordinates a * xB
    template <int D, CoordinateSystem coordT>
    constexpr auto operator*(const scalar_t & a, const Coordinates<D, coordT> & xB)
        -> Coordinates<D, coordT>;

    // the distance from the origin
    // TOFIX: not sure whether it makes sense to talk about {length} of a set of coordinates
    template <int D, CoordinateSystem coordT>
    constexpr auto length(const Coordinates<D, coordT> & xA) -> scalar_t;
}


namespace mito::geometry {

    // class for a set of coordinates in a {coordT} coordinates system (e.g. CARTESIAN, POLAR, ...)
    // in {D} dimensions
    template <int D, CoordinateSystem coordT>
    class Coordinates {
      private:
        // alias for me
        using coordinates_type = Coordinates<D, coordT>;
        // my underlying array type
        using array_t = mito::vector_t<D>;

      public:
        // constructor
        constexpr Coordinates(const mito::scalar_t (&&coords)[D]) : _array(coords) {}

        // constructor
        constexpr Coordinates(const array_t coords) : _array(coords) {}

        // destructor
        constexpr ~Coordinates() {}

        // default copy constructor
        constexpr Coordinates(const Coordinates &) = default;

        // default move constructor
        constexpr Coordinates(Coordinates &&) = default;

        // default assignment operator
        constexpr Coordinates & operator=(const Coordinates &) = default;

        // default move assignment operator
        constexpr Coordinates & operator=(Coordinates &&) = default;

      public:
        // friendship with addition of coordinates
        friend constexpr auto operator+<>(const coordinates_type & xA, const coordinates_type & xB)
            -> coordinates_type;

        // friendship with scaling of coordinates
        friend constexpr auto operator*<>(const scalar_t & a, const coordinates_type & xB)
            -> coordinates_type;

        // friendship with {operator==} for coordinates
        friend constexpr auto operator==<>(const coordinates_type & xA, const coordinates_type & xB)
            -> bool;

        // friendship with {length} of coordinates
        friend constexpr auto length<>(const coordinates_type & xA) -> scalar_t;

      public:
        auto print() const noexcept -> void
        {
            // print the coordinates of the point
            std::cout << "Coordinates: " << _array << std::endl;

            // all done
            return;
        }

      private:
        // the array of coordinates
        const array_t _array;
    };

    template <int D, CoordinateSystem coordT>
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
