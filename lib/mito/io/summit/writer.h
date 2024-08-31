// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::summit {

    template <class cellT, geometry::coordinates_c coordT>
    requires(cellT::dim == coordT::dim)
    auto writer(
        std::string fileName, const mito::mesh::mesh_t<cellT> & mesh,
        const geometry::coordinate_system_t<coordT> & coordinate_system) -> void
    {
        // the dimension of the physical space
        constexpr int D = cellT::dim;

        // append file extension
        fileName = fileName + ".summit";

        // create the output file
        std::ofstream outfile(fileName);

        // type of point
        using point_type = geometry::point_t<D>;

        // map points to an index (points that are shared among multiple elements have the same
        // index)
        std::unordered_map<point_type, int, utilities::hash_function<point_type>> points;

        // index assigned to each point (start counting from 1, summit mesh convention)
        auto index = 1;

        // insert the points corresponding to the mesh nodes
        for (const auto & cell : mesh.cells()) {
            for (const auto & node : cell.nodes()) {
                const auto & point = node.point();
                auto [_, inserted] = points.insert({ point, index });
                // if the point was inserted the map (i.e. if it is not a duplicate)
                if (inserted) {
                    // increment index
                    ++index;
                }
            }
        }

        // populate the file heading
        // TOFIX: number of materials is always 1 for now
        outfile << D << std::endl;
        outfile << std::size(points) << " " << mesh.nCells() << " " << 1 << std::endl;

        // a sorted vector with points (the vector subscript is the point index minus one)
        std::vector<point_type> points_vector(std::size(points));
        for (const auto & [point, index] : points) {
            points_vector[index - 1] = point;
        }

        // write the points to file in an order determined by their index
        for (const auto & point : points_vector) {
            const auto & coord = coordinate_system.coordinates(point);
            outfile << std::setprecision(15) << coord << std::endl;
        }

        // write the cells to file
        for (const auto & cell : mesh.cells()) {
            outfile << cellT::n_vertices << " ";
            for (const auto & node : cell.nodes()) {
                outfile << points[node.point()] << " ";
            }
            // TOFIX: material label is always 1 for now
            outfile << 1 << std::endl;
        }

        // close the file
        outfile.close();

        // all done
        return;
    }

}    // namespace mito::io::summit


// end of file
