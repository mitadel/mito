// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::vtk {

    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<meshT, coordSystemT>)
    class MeshWriterVTK : public GridWriterVTK<meshT::dim> {

      private:
        // the mesh type
        using mesh_type = meshT;
        // the coordinate system type
        using coord_system_type = coordSystemT;
        // the dimension of the physical space
        static constexpr int D = mesh_type::dim;
        // the type of node
        using node_type = typename mesh_type::cell_type::node_type;
        // the type of a collection of nodes (nodes are mapped to the index of the vtk points;
        // points that are shared among multiple elements have the same index)
        using nodes_type = std::unordered_map<node_type, int, utilities::hash_function<node_type>>;

      private:
        auto _create_vtk_grid(const mesh_type & mesh, const coord_system_type & coordinate_system)
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
                auto cellVtk = vtkCellPointer<typename mesh_type::cell_type::simplex_type>();

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
        MeshWriterVTK(
            std::string filename, const mesh_type & mesh, const coord_system_type & coord_system) :
            GridWriterVTK<D>(filename)
        {
            _create_vtk_grid(mesh, coord_system);
        }

        template <class Y>
        auto attach_field(const fem::nodal_field_t<D, Y> & field, std::string fieldname) -> void
        {
            // get the number of nodes
            auto n_nodes = field.n_nodes();

            // check the number of nodes in the field equals the number of points in the grid
            assert(n_nodes == this->_grid->GetNumberOfPoints());

            // initialize a vtk array
            auto vtkArray = vtkSmartPointer<vtkDoubleArray>::New();
            vtkArray->SetName(fieldname.data());
            vtkArray->SetNumberOfComponents(Y::size);
            vtkArray->SetNumberOfTuples(n_nodes);

            // populate the array with the nodal values
            for (auto & [node, value] : field) {
                // get the index corresponding to the current node
                auto index = _nodes.at(node);
                vtkArray->SetTuple(index, value.begin());
            }

            // insert array into output mesh
            this->_grid->GetPointData()->AddArray(vtkArray);
        }

      private:
        // a collection of nodes in the grid
        nodes_type _nodes;
    };

}    // namespace mito::io::vtk


// end of file
