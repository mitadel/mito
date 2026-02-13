// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io {

    // class that writes a mesh to file in {summit} format
    template <mesh::mesh_c meshT, geometry::coordinate_system_c coordSystemT>
    requires(utilities::same_dim_c<meshT, coordSystemT>)
    class MeshSummitWriter : public Writer {

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
        // the type of a collection of points (points are mapped to indices; points that are shared
        // among multiple elements have the same index)
        using points_type =
            std::unordered_map<point_type, int, utilities::hash_function<point_type>>;

      public:
        // constructor
        MeshSummitWriter(
            std::string filename, const mesh_type & mesh, const coord_system_type & coord_system,
            std::string element_type) :
            Writer(filename),
            _mesh(mesh),
            _coord_system(coord_system),
            _points(),
            _element_type(element_type)
        {
            // index assigned to each point (start counting from 1, summit mesh convention)
            auto index = 1;

            // insert the points corresponding to the mesh nodes (eliminating duplicates)
            for (const auto & cell : mesh.cells()) {
                for (const auto & node : cell.nodes()) {
                    const auto & point = node->point();
                    auto [_, inserted] = _points.insert({ point, index });
                    // if the point was inserted in the map (i.e. if it is not a duplicate)
                    if (inserted) {
                        // increment index
                        ++index;
                    }
                }
            }
        }

        // destructor
        ~MeshSummitWriter() = default;

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

            // a sorted vector with points (the vector subscript is the point index minus one)
            std::vector<point_type> points_vector(std::size(_points));
            for (const auto & [point, index] : _points) {
                points_vector[index - 1] = point;
            }

            // write the points to file in an order determined by their index
            for (const auto & point : points_vector) {
                const auto & coord = _coord_system.coordinates(point);
                outfile << std::setprecision(15);
                for (int d = 0; d < D; ++d)
                    outfile << coord[d] << " ";
                outfile << std::endl;
            }

            // write the cells to file
            for (const auto & cell : _mesh.cells()) {
                outfile << summit::cell<cell_type>::type << " ";
                for (const auto & node : cell.nodes()) {
                    outfile << _points.at(node->point()) << " ";
                }
                // TOFIX: material label is always 1 for now
                outfile << 1 << " " << _element_type << std::endl;
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

        // the type of element
        std::string _element_type;
    };

}    // namespace mito::io


// end of file
