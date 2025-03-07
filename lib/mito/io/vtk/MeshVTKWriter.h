// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::vtk {

    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT, class vtkGridWriterT>
    requires(
        utilities::same_dim_c<meshT, coordSystemT> && utilities::same_dim_c<meshT, vtkGridWriterT>)
    class MeshVTKWriter : public vtkGridWriterT {
      public:
        // the grid type
        using grid_type = meshT;
        // the grid writer type
        using grid_writer_type = vtkGridWriterT;

      private:
        // the coordinate system type
        using coord_system_type = coordSystemT;
        // the dimension of the physical space
        static constexpr int D = grid_type::dim;
        // the type of node
        using node_type = typename grid_type::cell_type::node_type;
        // the type of a collection of nodes (nodes are mapped to the index of the vtk points;
        // points that are shared among multiple elements have the same index)
        using nodes_type = std::unordered_map<node_type, int, utilities::hash_function<node_type>>;

      private:
        auto _create_vtk_grid(const grid_type & mesh, const coord_system_type & coordinate_system)
        {
            // vtk points and cells
            auto pointsVtk = vtkSmartPointer<vtkPoints>::New();

            // global index assigned to each vtk point
            auto indexPointVtk = 0;

            // loop over the cells
            for (const auto & cell : mesh.cells()) {
                // loop over the nodes of the cell
                for (const auto & node : cell.nodes()) {
                    // try to insert the point in the map
                    auto [_, inserted] = _nodes.insert({ node, indexPointVtk });
                    // if the point was inserted the map (i.e. not a duplicate)
                    if (inserted) {
                        // insert the new vtk point
                        insert_vtk_point(coordinate_system.coordinates(node->point()), pointsVtk);
                        // update global index for the vtk point
                        ++indexPointVtk;
                    }
                }
            }

            // loop over the cells
            for (const auto & cell : mesh.cells()) {

                // create vtk cell
                auto cellVtk = vtkCellPointer<typename grid_type::cell_type::simplex_type>();

                // local index for the points of the cell
                auto indexLocalPointVtk = 0;

                // loop over the nodes of the cell
                for (const auto & node : cell.nodes()) {
                    // assert that the node is present in the collection of nodes
                    assert(_nodes.contains(node));
                    // get the index of the node
                    auto index = _nodes.at(node);
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
        MeshVTKWriter(
            std::string filename, const grid_type & mesh, const coord_system_type & coord_system) :
            grid_writer_type(filename)
        {
            _create_vtk_grid(mesh, coord_system);
        }

        // accessor for the nodes
        auto nodes() const -> const nodes_type & { return _nodes; }

      private:
        // a collection of nodes in the grid
        nodes_type _nodes;
    };

}    // namespace mito::io::vtk


// end of file
