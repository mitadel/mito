// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::geometry {

    template <int D>
    class Point : public utilities::Shareable {

      private:
        constexpr Point() {}

        constexpr ~Point() override {}

        // delete copy constructor
        Point(const Point &) = delete;

        // delete move constructor
        Point(Point &&) = delete;

        // delete assignment operator
        Point & operator=(const Point &) = delete;

        // delete move assignment operator
        Point & operator=(Point &&) = delete;

        // private friendship with the repository of points
        friend class utilities::Repository<point_t<D>>;
    };

}    // namespace mito


// end of file
