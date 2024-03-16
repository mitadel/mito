// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_geometry_CoordinateSystem_h)
#define mito_geometry_CoordinateSystem_h


namespace mito::geometry {

    template <int D, CoordinateType coordT>
    class CoordinateSystem {
      public:
        // a set of coordinates
        using coordinates_type = coordinates_t<D, coordT>;

      private:
        // a point
        using point_type = point_t<D>;
        // id type of point
        using point_id_type = utilities::index_t<point_type>;
        // a map between points and their coordinates
        using coordinates_map_type = std::map<point_id_type, coordinates_type>;

      public:
        // I-th basis vector in dimension D
        template <int I>
        requires(I >= 0 && I < D)
        static constexpr auto e = pyre::tensor::unit<pyre::tensor::vector_t<D>, I>;

      public:
        // constructor
        CoordinateSystem() : _coordinates_map() {}

        // destructor
        ~CoordinateSystem() {}

      private:
        // delete copy constructor
        CoordinateSystem(const CoordinateSystem<D, coordT> &) = delete;

        // delete move constructor
        CoordinateSystem(CoordinateSystem<D, coordT> &&) noexcept = delete;

        // delete assignment operator
        void operator=(const CoordinateSystem<D, coordT> &) = delete;

        // delete move assignment operator
        void operator=(CoordinateSystem<D, coordT> &&) noexcept = delete;

      public:
        // place the {point} at location {coord}
        auto place(const point_type & point, const coordinates_type & coord) -> void
        {
            // record the new point-coordinates pair
            auto ret = _coordinates_map.emplace(point.id(), coord);

            // if the point was not insterted, then it is a duplicate
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
            return 0.5 * (coordinates(point_a) + coordinates(point_b));
        }

      private:
        // the coordinates of all points
        coordinates_map_type _coordinates_map;
    };

}    // namespace mito


#endif    // mito_geometry_CoordinateSystem_h


// end of file
