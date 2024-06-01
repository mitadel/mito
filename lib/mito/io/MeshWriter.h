// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::vtk {

    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    requires(meshT::cell_type::dim == coordSystemT::coordinates_type::dim)
    class MeshWriter {

      private:
        // the mesh type
        using mesh_type = meshT;
        // the coordinate system type
        using coord_system_type = coordSystemT;
        // the dimension of the physical space
        static constexpr int D = mesh_type::cell_type::dim;
        // the type of grid
        using grid_type = vtkSmartPointer<vtkUnstructuredGrid>;
        using point_type = geometry::point_t<D>;
        // map mesh points to the index of the vtk points. Points that are shared among
        // multiple elements have the same index.
        using points_type = std::unordered_set<point_type, utilities::hash_function<point_type>>;

      private:
        auto _create_vtk_grid(const mesh_type & mesh, const coord_system_type & coordinate_system)
            -> grid_type
        {
            // loop over the cells
            for (const auto & cell : mesh.cells()) {
                // loop over the nodes of the cell
                for (const auto & node : cell.nodes()) {
                    // add the point to the map with its global index
                    _points.insert(node.point());
                }
            }

            // vtk points and cells
            auto pointsVtk = vtkSmartPointer<vtkPoints>::New();

            // insert the new vtk point
            for (const auto & point : _points) {
                insert_vtk_point(coordinate_system.coordinates(point), pointsVtk);
            }

            // vtk unstructured grid
            auto gridVtk = vtkSmartPointer<vtkUnstructuredGrid>::New();

            // loop over the cells
            for (const auto & cell : mesh.cells()) {

                // create vtk cell
                auto cellVtk = vtkCellPointer<typename mesh_type::cell_type::simplex_type>();

                // local index for the points of the cell
                auto indexLocalPointVtk = 0;

                // loop over the nodes of the cell
                for (const auto & node : cell.nodes()) {
                    // retrieve the corresponding point
                    const auto & point = node.point();
                    // assert that the point is present in the set of points
                    assert(_points.contains(point));
                    // calculate the index of the point
                    auto index = std::distance(_points.begin(), _points.find(point));
                    // set the id of the point
                    cellVtk->GetPointIds()->SetId(indexLocalPointVtk, index);
                    // update local index for the points in the cell
                    ++indexLocalPointVtk;
                }

                // insert the new cell
                gridVtk->InsertNextCell(cellVtk->GetCellType(), cellVtk->GetPointIds());
            }

            // set the grid points
            gridVtk->SetPoints(pointsVtk);

            // all done
            return gridVtk;
        }

      public:
        MeshWriter(
            std::string filename, const mesh_type & mesh, const coord_system_type & coord_system) :
            _filename(filename),
            _points(),
            _grid(_create_vtk_grid(mesh, coord_system))
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

            // get the number of nodes
            auto n_nodes = field.n_nodes();

            // check the number of nodes in the field equals the number of points in the grid
            std::cout << n_nodes << "\t" << _grid->GetNumberOfPoints() << std::endl;
            // assert(n_nodes == _grid->GetNumberOfPoints());
            // and equal to the number of indices that we have recorded so far
            // assert(n_nodes == int(std::size(_points)));
            std::cout << n_nodes << "\t" << int(std::size(_points)) << std::endl;

            // initialize a vtk array
            auto vtkArray = vtkSmartPointer<vtkDoubleArray>::New();
            vtkArray->SetName(fieldname.data());
            vtkArray->SetNumberOfComponents(Y::size);
            vtkArray->SetNumberOfTuples(field.n_nodes());

            // populate the array with the nodal values
            for (auto & [node, value] : field) {
                // get the index corresponding to the current point
                auto index = std::distance(_points.begin(), _points.find(node.point()));
                vtkArray->SetTuple(index, value.begin());
            }

            // insert array into output mesh
            _grid->GetPointData()->AddArray(vtkArray);

            // all done
            return;
        }

        auto write() const -> void
        {
            // create a new writer
            auto writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
            // set the name of the output file
            writer->SetFileName((_filename + ".vtu").data());

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

      private:
        // the name of the output file to be written
        std::string _filename;

        // a map storing point -> index relation
        points_type _points;

        // the grid
        grid_type _grid;
    };

}    // namespace mito::io::vtk


// end of file
