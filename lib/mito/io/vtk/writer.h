// code guard
#if !defined(mito_io_vtk_writer_h)
#define mito_io_vtk_writer_h


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

    template <int D>
    auto insertVtkPoint(const vector_t<D> &, vtkSmartPointer<vtkPoints> &) -> void;

    template <>
    auto insertVtkPoint(const vector_t<3> & coord, vtkSmartPointer<vtkPoints> & pointsVtk) -> void
    {
        // add the point as new vtk point
        pointsVtk->InsertNextPoint(coord[0], coord[1], coord[2]);
    }

    template <>
    auto insertVtkPoint(const vector_t<2> & coord, vtkSmartPointer<vtkPoints> & pointsVtk) -> void
    {
        // add the point as new vtk point
        pointsVtk->InsertNextPoint(coord[0], coord[1], 0.);
    }

    template <class cellT, int D>
    auto createVtkUnstructuredGrid(const mito::mesh::mesh_t<cellT, D> & mesh)
        -> vtkSmartPointer<vtkUnstructuredGrid>
    {
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
            auto cellVtk = vtkCellPointer<cellT>();

            // local index for the points of the cell
            auto indexLocalPointVtk = 0;

            // retrieve vertices of the cell
            auto vertices = cell->vertices();

            // loop over the vertices of the cell
            for (const auto & vertex : vertices) {
                // retrieve the corresponding point
                const auto pPoint = mesh.geometry().point(vertex);
                // if the point is not present in the map
                if (mapPoints.count(pPoint) == 0) {
                    // insert the new vtk point
                    insertVtkPoint(pPoint->coordinates(), pointsVtk);
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

    template <class cellT, int D>
    auto writer(std::string fileName, const mito::mesh::mesh_t<cellT, D> & mesh) -> void
    {
        // create vtk unstructured grid
        const auto gridVtk = createVtkUnstructuredGrid(mesh);

        // write grid to file
        write(fileName, gridVtk);
    }


    template <int D>
    auto writer(std::string fileName, const geometry::point_cloud_t<D> & cloud) -> void
    {
        // get point cloud compositions
        const auto & points = cloud.points();

        // vtk unstructured grid
        auto gridVtk = vtkSmartPointer<vtkUnstructuredGrid>::New();
        // vtk points and cells
        auto pointsVtk = vtkSmartPointer<vtkPoints>::New();

        // iterate over the points
        for (const auto & point : points) {
            insertVtkPoint(point->coordinates(), pointsVtk);
        }

        // set the grid points
        gridVtk->SetPoints(pointsVtk);

        // write grid to file
        write(fileName, gridVtk);
    }

}    // namespace mito::io::vtk

#endif    // mito_io_vtk_writer_h