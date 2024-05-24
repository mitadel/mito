// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::vtk {

    template <class cellT>
    auto vtkCellPointer() -> vtkSmartPointer<typename vtkCellT<cellT>::type>;

    template <>
    auto vtkCellPointer<topology::tetrahedron_t>() -> vtkSmartPointer<vtkTetra>
    {
        return vtkSmartPointer<vtkTetra>::New();
    }

    template <>
    auto vtkCellPointer<topology::triangle_t>() -> vtkSmartPointer<vtkTriangle>
    {
        return vtkSmartPointer<vtkTriangle>::New();
    }

    template <>
    auto vtkCellPointer<topology::segment_t>() -> vtkSmartPointer<vtkLine>
    {
        return vtkSmartPointer<vtkLine>::New();
    }

    template <geometry::coordinates_c coordT>
    auto insertVtkPoint(const coordT &, vtkSmartPointer<vtkPoints> &) -> void;

    template <>
    auto insertVtkPoint(
        const geometry::coordinates_t<3, geometry::CARTESIAN> & coord,
        vtkSmartPointer<vtkPoints> & pointsVtk) -> void
    {
        // add the point as new vtk point
        pointsVtk->InsertNextPoint(coord[0], coord[1], coord[2]);
    }

    template <>
    auto insertVtkPoint(
        const geometry::coordinates_t<2, geometry::CARTESIAN> & coord,
        vtkSmartPointer<vtkPoints> & pointsVtk) -> void
    {
        // add the point as new vtk point
        pointsVtk->InsertNextPoint(coord[0], coord[1], 0.);
    }

    template <class cellT, geometry::coordinates_c coordT>
    auto createVtkUnstructuredGrid(
        const mito::mesh::mesh_t<cellT> & mesh,
        const geometry::coordinate_system_t<coordT> & coordinate_system)
        -> vtkSmartPointer<vtkUnstructuredGrid>
    requires(cellT::dim == coordT::dim)
    {
        // the dimension of physical space
        constexpr int D = cellT::dim;

        // vtk unstructured grid
        auto gridVtk = vtkSmartPointer<vtkUnstructuredGrid>::New();
        // vtk points and cells
        auto pointsVtk = vtkSmartPointer<vtkPoints>::New();

        // map mesh points to the index of the vtk points. Points that are shared among
        // multiple elements have the same index.
        std::unordered_map<
            geometry::point_t<D>, int, utilities::hash_function<geometry::point_t<D>>>
            mapPoints;

        // global index assigned to each vtk point
        auto indexPointVtk = 0;

        // loop over the cells
        for (const auto & cell : mesh.cells()) {
            // create vtk cell
            auto cellVtk = vtkCellPointer<typename cellT::simplex_type>();

            // local index for the points of the cell
            auto indexLocalPointVtk = 0;

            // retrieve nodes of the cell
            auto nodes = cell.nodes();

            // loop over the nodes of the cell
            for (const auto & node : nodes) {
                // retrieve the corresponding point
                const auto pPoint = node.point();
                // if the point is not present in the map
                if (mapPoints.count(pPoint) == 0) {
                    // insert the new vtk point
                    insertVtkPoint(coordinate_system.coordinates(pPoint), pointsVtk);
                    // add the point to the map with its global index
                    mapPoints[pPoint] = indexPointVtk;
                    // update global index for the vtk point
                    ++indexPointVtk;
                }
                // set the id of the point
                cellVtk->GetPointIds()->SetId(indexLocalPointVtk, mapPoints[pPoint]);
                // update local index for the points in the cell
                ++indexLocalPointVtk;
            }

            // insert the new cell
            gridVtk->InsertNextCell(cellVtk->GetCellType(), cellVtk->GetPointIds());
        }

        // set the grid points
        gridVtk->SetPoints(pointsVtk);

        return gridVtk;
    }

    inline auto write(std::string fileName, const vtkSmartPointer<vtkUnstructuredGrid> & gridVtk)
        -> void
    {
        // write the grid to file
        auto writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
        writer->SetFileName((fileName + ".vtu").c_str());

#if VTK_MAJOR_VERSION <= 8
        writer->SetInput(gridVtk);
#else
        writer->SetInputData(gridVtk);
#endif

        writer->Write();
    }

    template <class cellT, geometry::coordinates_c coordT>
    requires(cellT::dim == coordT::dim)
    auto writer(
        std::string fileName, const mito::mesh::mesh_t<cellT> & mesh,
        const geometry::coordinate_system_t<coordT> & coordinate_system) -> void
    {
        // create vtk unstructured grid
        const auto gridVtk = createVtkUnstructuredGrid(mesh, coordinate_system);

        // write grid to file
        write(fileName, gridVtk);
    }


    template <int D, geometry::coordinates_c coordT>
    requires(D == coordT::dim)
    auto writer(
        std::string fileName, const geometry::point_cloud_t<D> & cloud,
        const geometry::coordinate_system_t<coordT> & coordinate_system) -> void
    {
        // get point cloud compositions
        const auto & points = cloud.points();

        // vtk unstructured grid
        auto gridVtk = vtkSmartPointer<vtkUnstructuredGrid>::New();
        // vtk points and cells
        auto pointsVtk = vtkSmartPointer<vtkPoints>::New();

        // iterate over the points
        for (const auto & point : points) {
            insertVtkPoint(coordinate_system.coordinates(point), pointsVtk);
        }

        // set the grid points
        gridVtk->SetPoints(pointsVtk);

        // write grid to file
        write(fileName, gridVtk);
    }

    template <class cellT, geometry::coordinates_c coordT, class Y>
    requires(cellT::dim == coordT::dim)
    auto writer(
        std::string fileName, const mito::mesh::mesh_t<cellT> & mesh,
        const geometry::coordinate_system_t<coordT> & coordinate_system,
        const fem::nodal_field_t<Y> & nodal_field
        /*const auto fields::field_c & field*/) -> void
    {
        // create vtk unstructured grid
        const auto gridVtk = createVtkUnstructuredGrid(mesh, coordinate_system);

        // initialize a vtk array
        auto vtkArray = vtkSmartPointer<vtkDoubleArray>::New();
        vtkArray->SetName(nodal_field.name().data());
        vtkArray->SetNumberOfComponents(Y::size);
        vtkArray->SetNumberOfTuples(nodal_field.n_nodes());

        // std::cout << gridVtk->GetPoints()->GetPoint(0)[0] << std::endl;
        // std::cout << gridVtk->GetNumberOfPoints() << std::endl;

        for (int i = 0; i < gridVtk->GetNumberOfPoints(); ++i) {
            vtkArray->SetTuple(i, nodal_field(i).begin());
        }

        // insert array into output mesh
        gridVtk->GetPointData()->AddArray(vtkArray);

        // write grid to file
        write(fileName, gridVtk);
    }

}    // namespace mito::io::vtk


// end of file
