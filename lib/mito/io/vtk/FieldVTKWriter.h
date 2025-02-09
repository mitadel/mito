// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::vtk {

    template <class gridWriterT, geometry::coordinate_system_c coordSystemT>
    class FieldVTKWriter {

      private:
        // the dimension of the physical space
        static constexpr int D = coordSystemT::dim;
        // the grid writer type
        using grid_writer_type = gridWriterT;
        // the grid type
        using grid_type = typename grid_writer_type::grid_type;
        // the coordinate system type
        using coord_system_type = coordSystemT;

      public:
        FieldVTKWriter(
            std::string filename, const grid_type & grid, const coord_system_type & coord_system) :
            _grid_writer(filename, grid, coord_system)
        {}

      private:
        template <class Y>
        auto _attach_field(const fem::nodal_field_t<D, Y> & field, std::string fieldname) -> void
        {
            // get the number of nodes
            auto n_nodes = field.n_nodes();

            // get the grid
            auto & grid = _grid_writer.grid();

            // get the nodes
            const auto & nodes = _grid_writer.nodes();

            // check the number of nodes in the field equals the number of points in the grid
            assert(n_nodes == grid->GetNumberOfPoints());

            // initialize a vtk array
            auto vtkArray = vtkSmartPointer<vtkDoubleArray>::New();
            vtkArray->SetName(fieldname.data());
            vtkArray->SetNumberOfComponents(Y::size);
            vtkArray->SetNumberOfTuples(n_nodes);

            // populate the array with the nodal values
            for (auto & [node, value] : field) {
                // get the index corresponding to the current node
                auto index = nodes.at(node);
                vtkArray->SetTuple(index, value.begin());
            }

            // insert array into output grid
            grid->GetPointData()->AddArray(vtkArray);
        }

      public:
        // TOFIX: use concepts to say that Y is a tensor
        template <class Y>
        auto record(const fem::nodal_field_t<D, Y> & field, std::string fieldname = "") -> void
        {
            // if no name was provided
            if (fieldname == "") {
                // use the name of the field
                fieldname = field.name();
            }

            // delegate to the grid
            return _attach_field(field, fieldname);
        }

        auto write() const -> void
        {
            // delegate to the grid
            return _grid_writer.write();
        }

      protected:
        // the grid writer
        grid_writer_type _grid_writer;
    };

}    // namespace mito::io::vtk


// end of file
