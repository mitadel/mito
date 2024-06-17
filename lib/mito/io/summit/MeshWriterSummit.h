// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io {

    // class that writes a mesh to file in {summit} format
    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<meshT, coordSystemT>)
    class MeshWriterSummit : public Writer {

      private:
        // the mesh type
        using mesh_type = meshT;
        // the cell type
        using cell_type = typename mesh_type::cell_type;
        // the coordinate system type
        using coord_system_type = coordSystemT;
        // the dimension of the physical space
        static constexpr int D = mesh_type::dim;
        // the type of point
        using point_type = typename coord_system_type::point_type;
        // the type of a collection of points
        using points_type = std::unordered_set<point_type, utilities::hash_function<point_type>>;

      public:
        // constructor
        MeshWriterSummit(
            std::string filename, const mesh_type & mesh, const coord_system_type & coord_system) :
            Writer(filename),
            _mesh(mesh),
            _coord_system(coord_system),
            _points()
        {
            // insert the points corresponding to the mesh nodes (eliminating duplicates)
            for (const auto & cell : mesh.cells()) {
                for (const auto & node : cell.nodes()) {
                    _points.insert(node->point());
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
            outfile << std::size(_points) << " " << _mesh.nCells() << " " << 1 << std::endl;

            // write the points to file
            for (const auto & point : _points) {
                const auto & coord = _coord_system.coordinates(point);
                outfile << std::setprecision(15) << coord << std::endl;
            }

            // write the cells to file
            for (const auto & cell : _mesh.cells()) {
                outfile << summit::cell<cell_type>::type << " ";
                for (const auto & node : cell.nodes()) {
                    outfile << std::distance(_points.begin(), _points.find(node->point())) + 1
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

        // a collection of points in the mesh
        points_type _points;
    };

}    // namespace mito::io


// end of file
