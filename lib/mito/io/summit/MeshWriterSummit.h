// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io {

    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    requires(meshT::cell_type::dim == coordSystemT::coordinates_type::dim)
    class MeshWriterSummit : public GridWriter<meshT::cell_type::dim> {

      private:
        // the mesh type
        using mesh_type = meshT;
        // the coordinate system type
        using coord_system_type = coordSystemT;
        // the dimension of the physical space
        static constexpr int D = mesh_type::cell_type::dim;

      public:
        // constructor
        MeshWriterSummit(
            std::string filename, const mesh_type & mesh, const coord_system_type & coord_system) :
            GridWriter<D>(filename),
            _mesh(mesh),
            _coord_system(coord_system)
        {
            // insert the points corresponding to the mesh nodes
            for (const auto & cell : mesh.cells()) {
                for (const auto & node : cell.nodes()) {
                    this->_points.insert(node.point());
                }
            }
        }

        // destructor
        ~MeshWriterSummit() = default;

      public:
        // write mesh to file
        virtual auto write() const -> void override
        {
            // assemble the file name
            std::string filename = this->_filename + ".summit";

            // create the output file
            std::ofstream outfile(filename);

            // populate the file heading
            // TOFIX: number of materials is always 1 for now
            outfile << D << std::endl;
            outfile << std::size(this->_points) << " " << _mesh.nCells() << " " << 1 << std::endl;

            // write the points to file
            for (const auto & point : this->_points) {
                const auto & coord = _coord_system.coordinates(point);
                outfile << std::setprecision(15) << coord << std::endl;
            }

            // write the cells to file
            for (const auto & cell : _mesh.cells()) {
                outfile << mesh_type::cell_type::n_vertices << " ";
                for (const auto & node : cell.nodes()) {
                    outfile
                        << std::distance(this->_points.begin(), this->_points.find(node.point()))
                               + 1
                        << " ";
                }
                // TOFIX: material label is always 1 for now
                outfile << 1 << std::endl;
            }

            // close the file
            outfile.close();

            // all done
            return;
        }

      private:
        // a const reference to the mesh
        const mesh_type & _mesh;

        // a const reference to the coordinate system
        const coord_system_type & _coord_system;
    };

}    // namespace mito::io


// end of file
