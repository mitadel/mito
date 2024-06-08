// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::vtk {

    template <int D>
    class GridWriterVTK : public GridWriter<D> {

      private:
        // the type of grid
        using grid_type = vtkSmartPointer<vtkUnstructuredGrid>;

      protected:
        GridWriterVTK(std::string filename) : GridWriter<D>(filename), _grid(grid_type::New()) {}

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

        template <class Y>
        auto attach_field(const fem::nodal_field_t<D, Y> & field, std::string fieldname) -> void
        {
            // get the number of nodes
            auto n_nodes = field.n_nodes();

            // check the number of nodes in the field equals the number of points in the grid
            assert(n_nodes == _grid->GetNumberOfPoints());

            // initialize a vtk array
            auto vtkArray = vtkSmartPointer<vtkDoubleArray>::New();
            vtkArray->SetName(fieldname.data());
            vtkArray->SetNumberOfComponents(Y::size);
            vtkArray->SetNumberOfTuples(n_nodes);

            // populate the array with the nodal values
            for (auto & [node, value] : field) {
                // get the index corresponding to the current point
                auto index =
                    std::distance(this->_points.begin(), this->_points.find(node->point()));
                vtkArray->SetTuple(index, value.begin());
            }

            // insert array into output mesh
            _grid->GetPointData()->AddArray(vtkArray);
        }

      protected:
        // the grid
        grid_type _grid;
    };

}    // namespace mito::io::vtk


// end of file
