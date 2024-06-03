// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::vtk {

    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    requires(meshT::cell_type::dim == coordSystemT::coordinates_type::dim)
    class MeshWriterVTK : public GridWriterVTK<meshT::cell_type::dim> {

      private:
        // the mesh type
        using mesh_type = meshT;
        // the coordinate system type
        using coord_system_type = coordSystemT;
        // the dimension of the physical space
        static constexpr int D = mesh_type::cell_type::dim;

      private:
        auto _create_vtk_grid(const mesh_type & mesh, const coord_system_type & coordinate_system)
        {
            // loop over the cells
            for (const auto & cell : mesh.cells()) {
                // loop over the nodes of the cell
                for (const auto & node : cell.nodes()) {
                    // add the point to the collection of points (eliminating duplicates)
                    this->_points.insert(node.point());
                }
            }

            // vtk points and cells
            auto pointsVtk = vtkSmartPointer<vtkPoints>::New();

            // insert the new vtk point
            for (const auto & point : this->_points) {
                insert_vtk_point(coordinate_system.coordinates(point), pointsVtk);
            }

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
                    assert(this->_points.contains(point));
                    // calculate the index of the point
                    auto index = std::distance(this->_points.begin(), this->_points.find(point));
                    // set the id of the point
                    cellVtk->GetPointIds()->SetId(indexLocalPointVtk, index);
                    // update local index for the points in the cell
                    ++indexLocalPointVtk;
                }

                // insert the new cell
                this->_grid->InsertNextCell(cellVtk->GetCellType(), cellVtk->GetPointIds());
            }

            // set the grid points
            this->_grid->SetPoints(pointsVtk);

            // all done
            return;
        }

      public:
        MeshWriterVTK(
            std::string filename, const mesh_type & mesh, const coord_system_type & coord_system) :
            GridWriterVTK<D>(filename)
        {
            _create_vtk_grid(mesh, coord_system);
        }
    };

}    // namespace mito::io::vtk


// end of file
