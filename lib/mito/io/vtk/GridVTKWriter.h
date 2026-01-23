// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::vtk {

    template <int D>
    class GridVTKWriter : public Writer {

      public:
        // the grid dimension
        static constexpr int dim = D;

      private:
        // the type of grid
        using vtk_grid_type = vtkSmartPointer<vtkUnstructuredGrid>;

      protected:
        // constructor
        // (protected so this class cannot be instantiated unless by the derived classes)
        GridVTKWriter(std::string filename) : Writer(filename), _grid(vtk_grid_type::New()) {}

      public:
        auto write() const -> void override
        {
            // create a new writer
            auto writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
            // set the name of the output file
            writer->SetFileName((this->_filename + ".vtu").data());

            // sign the grid up for writing
#if VTK_MAJOR_VERSION <= 8
            writer->SetInput(_grid);
#else
            writer->SetInputData(_grid);
#endif
            // write the grid to file
            writer->Write();

            // all done
            return;
        }

        // accessor for the grid
        auto grid() -> vtk_grid_type & { return _grid; }

      protected:
        // the grid
        vtk_grid_type _grid;
    };

}    // namespace mito::io::vtk


// end of file
