// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


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
        // friendship with {operator==} for coordinates
        friend constexpr auto operator==<>(const coordinates_type &, const coordinates_type &)
            -> bool;

        // friendship with {operator<} for coordinates
        friend constexpr auto operator< <>(const coordinates_type &, const coordinates_type &)
            -> bool;

      public:
        // components accessor
        constexpr auto operator[](int i) const -> scalar_t { return _array[i]; }

        auto print() const -> void
        {
            // print the coordinates of the point
            std::cout << _array;

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

        // helper function to print D components
        constexpr auto _print = []<size_t... I>(
                                    std::ostream & os, const Coordinates<D, coordT> & coord,
                                    std::index_sequence<I...>) {
            // print the coordinates
            ((os << coord[I] << " "), ...);
        };

        // print the coordinates
        _print(os, coord, std::make_index_sequence<D>{});

        // all done
        return os;
    }

    // operator==
    template <int D, CoordinateType coordT>
    constexpr auto operator==(const Coordinates<D, coordT> & xA, const Coordinates<D, coordT> & xB)
        -> bool
    {
        return xA._array == xB._array;
    }

    // operator<
    template <int D, CoordinateType coordT>
    constexpr auto operator<(const Coordinates<D, coordT> & xA, const Coordinates<D, coordT> & xB)
        -> bool
    {
        return xA._array < xB._array;
    }

}    // namespace mito


// end of file
