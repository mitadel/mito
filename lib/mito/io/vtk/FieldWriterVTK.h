// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::vtk {

    template <class gridT, geometry::coordinate_system_c coordSystemT>
    class FieldWriterVTK {

      private:
        // the dimension of the physical space
        static constexpr int D = coordSystemT::dim;
        // the grid type (e.g. mesh or point cloud)
        using grid_type = gridT;
        // the coordinate system type
        using coord_system_type = coordSystemT;
        // the grid writer type
        using grid_writer_type = typename grid_writer<gridT, coordSystemT>::type;

      public:
        FieldWriterVTK(
            std::string filename, const grid_type & grid, const coord_system_type & coord_system) :
            _grid_writer(filename, grid, coord_system)
        {}

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
            return _grid_writer.attach_field(field, fieldname);
        }

        auto write() const -> void
        {
            // delegate to the grid
            return _grid_writer.write();
        }

      private:
        // the grid writer
        grid_writer_type _grid_writer;
    };

}    // namespace mito::io::vtk


// end of file
