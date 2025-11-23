// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::geometry {

    template <coordinates_c coordT>
    class CoordinateSystem {
      public:
        // a set of coordinates
        using coordinates_type = coordT;
        // the dimension of the physical space
        static constexpr int dim = coordinates_type::dim;
        // a point
        using point_type = point_t<dim>;

      private:
        // alias {dim} for internal purposes
        static constexpr int D = dim;
        // a map between points and their coordinates
        using coordinates_map_type =
            std::unordered_map<point_type, coordinates_type, utilities::hash_function<point_type>>;

      public:
        // constructor
        constexpr CoordinateSystem() : _coordinates_map() {}

        // constructor for change of coordinates
        template <coordinates_c otherCoordT>
        requires(coordT::dim == otherCoordT::dim)
        constexpr CoordinateSystem(const CoordinateSystem<otherCoordT> & other_coord_sys) :
            _coordinates_map()
        {
            // loop on the points and coordinates of the other coordinate system
            for (const auto & [point, coord] : other_coord_sys) {
                // store the same point with the transformed coordinates
                place(point, transform_coordinates<coordT>(coord));
            }
        }

        // destructor
        constexpr ~CoordinateSystem() = default;

      private:
        // delete copy constructor
        constexpr CoordinateSystem(const CoordinateSystem<coordT> &) = delete;

        // delete move constructor
        constexpr CoordinateSystem(CoordinateSystem<coordT> &&) noexcept = delete;

        // delete assignment operator
        constexpr void operator=(const CoordinateSystem<coordT> &) = delete;

        // delete move assignment operator
        constexpr void operator=(CoordinateSystem<coordT> &&) noexcept = delete;

      public:
        // place the {point} at location {coord}
        constexpr auto place(const point_type & point, const coordinates_type & coord) -> void
        {
            // record the new point-coordinates pair
            auto ret = _coordinates_map.emplace(point, coord);

            // if the point was not inserted, then it is a duplicate
            if (ret.second == false) {
                // report
                journal::firewall_t firewall("geometry::CoordinateSystem::place");
                firewall << journal::at(__HERE__)
                         << "Duplicate coordinates for same point in coordinate system."
                         << journal::endl;
                // do not allow duplicates for now
                assert(false);
            }

            // all done
            return;
        }

        // fetch the coordinates at point {point}
        constexpr auto coordinates(const point_type & point) const -> const coordinates_type &
        {
            // look-up the coordinates of the point in the map
            return _coordinates_map.at(point);
        }

        // get the coordinates of the midpoint between {point_a} and {point_b}
        constexpr auto midpoint(const point_type & point_a, const point_type & point_b) const
            -> coordinates_type
        {
            // easy enough
            return coordinates(point_a) + 0.5 * (coordinates(point_b) - coordinates(point_a));
        }

        // support for ranged for loops
        constexpr auto begin() const { return std::begin(_coordinates_map); }
        constexpr auto end() const { return std::end(_coordinates_map); }

      private:
        // the coordinates of all points
        coordinates_map_type _coordinates_map;
    };

}    // namespace mito


// end of file
