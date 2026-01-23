// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io {

    // a class that writes to file
    class Writer {

      public:
        // constructor
        Writer(std::string filename) : _filename(filename) {}

        // the {write} method
        virtual auto write() const -> void = 0;

      protected:
        // the name of the output file to be written
        std::string _filename;
    };

}    // namespace mito::io


// end of file
