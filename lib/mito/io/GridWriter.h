// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io {

    class GridWriter {

      public:
        GridWriter(std::string filename) : _filename(filename) {}

        virtual auto write() const -> void = 0;

      protected:
        // the name of the output file to be written
        std::string _filename;
    };

}    // namespace mito::io


// end of file
