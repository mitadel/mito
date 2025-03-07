// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::vtk {

    namespace {
        // the type of the grid writer depending on the type of grid and on the coordinate system
        template <class gridT>
        struct field_type;

        // specialization to {mesh_c} case
        template <mesh::mesh_c gridT>
        struct field_type<gridT> {
            template <class Y>
            using type = discretization::nodal_field_t<gridT::dim, Y>;
        };

        // specialization to {point_cloud_c} case
        template <geometry::point_cloud_c gridT>
        struct field_type<gridT> {
            template <class Y>
            using type = discretization::point_field_t<gridT::dim, Y>;
        };

        // utility function to get the data pointer
        template <typename Y>
        constexpr auto dim() -> int
        {
            if constexpr (std::is_arithmetic_v<Y>) {
                return 1;
            } else {
                return Y::size;
            }
        }

        // utility function to get the data pointer
        template <typename Y>
        auto begin(Y & value)
        {
            if constexpr (std::is_arithmetic_v<Y>) {
                return &value;
            } else {
                return std::begin(value);
            }
        }
    }

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
        // the field type
        template <class Y>
        using field_type = typename field_type<grid_type>::type<Y>;

      public:
        FieldVTKWriter(
            std::string filename, const grid_type & grid, const coord_system_type & coord_system) :
            _grid_writer(filename, grid, coord_system)
        {}

      private:
        template <class Y>
        auto _attach_field(const field_type<Y> & field, std::string fieldname) -> void
        {
            // get the number of entries in the field
            auto field_size = field.size();

            // get the grid
            auto & grid = _grid_writer.grid();

            // check the number of entries in the field equals the number of points in the grid
            assert(field_size == grid->GetNumberOfPoints());

            // initialize a vtk array
            auto vtkArray = vtkSmartPointer<vtkDoubleArray>::New();
            vtkArray->SetName(fieldname.data());
            vtkArray->SetNumberOfComponents(dim<Y>());
            vtkArray->SetNumberOfTuples(field_size);

            // populate the array
            if constexpr (mesh::mesh_c<grid_type>) {
                // get the nodes in the grid
                const auto & nodes = _grid_writer.nodes();

                // populate the array with the nodal values
                for (const auto & [node, index] : nodes) {
                    // get the index corresponding to the current node
                    vtkArray->SetTuple(index, begin(field(node)));
                }
            } else if constexpr (geometry::point_cloud_c<grid_type>) {
                // get the points in the grid
                const auto & points = _grid_writer.points();

                // populate the array with the point values
                int index = 0;
                for (const auto & point : points) {
                    // get the index corresponding to the current point
                    vtkArray->SetTuple(index++, begin(field(point)));
                }
            }

            // insert array into output grid
            grid->GetPointData()->AddArray(vtkArray);
        }

      public:
        // sign {field} up for writing
        template <class Y>
        auto record(const field_type<Y> & field, std::string fieldname = "") -> void
        {
            // if no name was provided
            if (fieldname == "") {
                // use the name of the field
                fieldname = field.name();
            }

            // delegate to the grid
            return _attach_field(field, fieldname);
        }

        // write the grid with the attached fields
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
