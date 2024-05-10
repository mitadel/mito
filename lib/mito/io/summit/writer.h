// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_io_summit_writer_h)
#define mito_io_summit_writer_h


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

        // id type of point
        using point_id_type = utilities::index_t<point_type>;

        // allocate memory for points
        std::unordered_map<point_id_type, const point_type &> points;

        // get the points corresponding to the mesh nodes
        for (const auto & cell : mesh.cells()) {
            for (const auto & node : cell.nodes()) {
                const auto & point = node.point();
                points.insert({ point.id(), point });
            }
        }

        // populate the heading
        outfile << D << std::endl;
        outfile << std::size(points) << " " << mesh.nCells() << " " << 1 << std::endl;

        // write the points
        for (const auto & [_, point] : points) {
            const auto & coord = coordinate_system.coordinates(point);
            outfile << std::setprecision(15) << coord << std::endl;
        }

        // write the cells
        for (const auto & cell : mesh.cells()) {
            outfile << cellT::n_vertices << " ";
            for (const auto & node : cell.nodes()) {
                outfile << distance(points.begin(), points.find(node.point().id())) + 1 << " ";
            }
            outfile << 1 << std::endl;
        }

        // close the file
        outfile.close();

        // all done
        return;
    }

}    // namespace mito::io::summit

#endif    // mito_io_summit_writer_h