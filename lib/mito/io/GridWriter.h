// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io {

    template <int D>
    class GridWriter {

      private:
        // the type of point
        using point_type = geometry::point_t<D>;
        // the type of a collection of points
        using points_type = std::unordered_set<point_type, utilities::hash_function<point_type>>;

      public:
        GridWriter(std::string filename) : _filename(filename), _points() {}

        virtual auto write() const -> void = 0;

      protected:
        // the name of the output file to be written
        std::string _filename;

        // a collection of points in the grid
        points_type _points;
    };

}    // namespace mito::io


// end of file
