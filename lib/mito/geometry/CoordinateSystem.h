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

      private:
        // alias {dim} for internal purposes
        static constexpr int D = dim;
        // a point
        using point_type = point_t<D>;
        // id type of point
        using point_id_type = utilities::index_t<point_type>;
        // a map between points and their coordinates
        using coordinates_map_type = std::map<point_id_type, coordinates_type>;

      public:
        // TOFIX: is this ever used?
        // I-th basis vector in dimension D
        template <int I>
        requires(I >= 0 && I < D)
        static constexpr auto e = pyre::tensor::unit<pyre::tensor::vector_t<D>, I>;

      public:
        // constructor
        CoordinateSystem() : _coordinates_map() {}

        // constructor for change of coordinates
        template <coordinates_c otherCoordT>
        requires(coordT::dim == otherCoordT::dim)
        CoordinateSystem(const CoordinateSystem<otherCoordT> & other_coord_sys) : _coordinates_map()
        {
            // loop on the points and coordinates of the other coordinate system
            for (const auto & [point, coord] : other_coord_sys) {
                // store the same point with the transformed coordinates
                place(point, transform_coordinates<coordT>(coord));
            }
        }

        // destructor
        ~CoordinateSystem() = default;

      private:
        // delete copy constructor
        CoordinateSystem(const CoordinateSystem<coordT> &) = delete;

        // delete move constructor
        CoordinateSystem(CoordinateSystem<coordT> &&) noexcept = delete;

        // delete assignment operator
        void operator=(const CoordinateSystem<coordT> &) = delete;

        // delete move assignment operator
        void operator=(CoordinateSystem<coordT> &&) noexcept = delete;

      public:
        // place the point with id {point_id} at location {coord}
        auto place(const point_id_type & point_id, const coordinates_type & coord) -> void
        {
            // record the new point-coordinates pair
            auto ret = _coordinates_map.emplace(point_id, coord);

            // if the point was not inserted, then it is a duplicate
            if (ret.second == false) {
                // report
                pyre::journal::firewall_t firewall("geometry::CoordinateSystem::place");
                firewall << pyre::journal::at(__HERE__)
                         << "Duplicate coordinates for same point in coordinate system."
                         << pyre::journal::endl;
                // do not allow duplicates for now
                assert(false);
            }

            // all done
            return;
        }

        // place the {point} at location {coord}
        auto place(const point_type & point, const coordinates_type & coord) -> void
        {
            // all done
            return place(point.id(), coord);
        }

        // fetch the coordinates at point {point}
        auto coordinates(const point_type & point) const -> const coordinates_type &
        {
            // look-up the coordinates of the point in the map
            return _coordinates_map.at(point.id());
        }

        // get the coordinates of the midpoint between {point_a} and {point_b}
        auto midpoint(const point_type & point_a, const point_type & point_b) const
            -> coordinates_type
        {
            // easy enough
            return coordinates(point_a) + 0.5 * (coordinates(point_b) - coordinates(point_a));
        }

        // support for ranged for loops
        inline auto begin() const { return std::begin(_coordinates_map); }
        inline auto end() const { return std::end(_coordinates_map); }

      private:
        // the coordinates of all points
        coordinates_map_type _coordinates_map;
    };

}    // namespace mito


// end of file
