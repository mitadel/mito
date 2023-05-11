// code guard
#if !defined(mito_mesh_vtk_writer_h)
#define mito_mesh_vtk_writer_h

#include <vtkCellArray.h>
#include <vtkFloatArray.h>
#include <vtkLine.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkSmartPointer.h>
#include <vtkTriangle.h>
#include <vtkTetra.h>
#include <vtkUnstructuredGrid.h>
#include <vtkXMLUnstructuredGridWriter.h>


namespace mito::mesh {
    template <int D>
    auto vtkCellPointer(const mesh::mesh_t<topology::tetrahedron_t, D> &)
        -> vtkSmartPointer<vtkTetra>
    {
        return vtkSmartPointer<vtkTetra>::New();
    }

    template <int D>
    auto vtkCellPointer(const mesh::mesh_t<topology::triangle_t, D> &)
        -> vtkSmartPointer<vtkTriangle>
    {
        return vtkSmartPointer<vtkTriangle>::New();
    }

    template <int D>
    auto vtkCellPointer(const mesh::mesh_t<topology::segment_t, D> &) -> vtkSmartPointer<vtkLine>
    {
        return vtkSmartPointer<vtkLine>::New();
    }

    template <int D>
    auto insertVtkPoint(const geometry::point_t<D> & pPoint, vtkSmartPointer<vtkPoints> & pointsVtk)
        -> void
    {
        // retrieve the coordinates of the point
        const auto & coordinates = pPoint.handle()->coordinates();
        // add the point as new vtk point
        pointsVtk->InsertNextPoint(coordinates[0], coordinates[1], coordinates[2]);
    }

    template <>
    auto insertVtkPoint(const geometry::point_t<2> & pPoint, vtkSmartPointer<vtkPoints> & pointsVtk)
        -> void
    {
        // retrieve the coordinates of the point
        const auto & coordinates = pPoint.handle()->coordinates();
        // add the point as new vtk point
        pointsVtk->InsertNextPoint(coordinates[0], coordinates[1], 0.);
    }

    template <class cellT, int D>
    auto createVtkPointsAndCells(const mesh::mesh_t<cellT, D> & mesh)
        -> std::pair<vtkSmartPointer<vtkPoints>, vtkSmartPointer<vtkCellArray>>
    {
        // vtk points and cells
        auto pointsVtk = vtkSmartPointer<vtkPoints>::New();
        auto cellsVtk = vtkSmartPointer<vtkCellArray>::New();

        // map mesh points to the index of the vtk points. Points that are shared among
        // multiple elements have the same index.
        std::unordered_map<
            mito::geometry::point_t<D>, int,
            mito::utilities::hash_function<mito::geometry::point_t<D>>>
            mapPoints;

        // global index assigned to each vtk point
        auto indexPointVtk = 0;

        // loop over the cells
        for (const auto & cell : mesh.cells()) {
            // create vtk cell
            auto cellVtk = vtkCellPointer(mesh);

            // local index for the points of the cell
            auto indexLocalPointVtk = 0;

            // retrieve vertices of the cell
            topology::vertex_set_t vertices;
            cell->vertices(vertices);

            // loop over the vertices of the cell
            for (const auto & vertex : vertices) {
                // retrieve the corresponding point
                const auto pPoint = mesh.geometry().point(vertex);
                // if the point is not present in the map
                if (mapPoints.count(pPoint) == 0) {
                    // insert the new vtk point
                    insertVtkPoint(pPoint, pointsVtk);
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
            cellsVtk->InsertNextCell(cellVtk);
        }

        return { pointsVtk, cellsVtk };
    }

    auto createVtkUnstructuredGrid(
        const vtkSmartPointer<vtkPoints> & pointsVtk,
        const vtkSmartPointer<vtkCellArray> & cellsVtk) -> vtkSmartPointer<vtkUnstructuredGrid>
    {
        // vtk unstructured grid
        auto gridVtk = vtkSmartPointer<vtkUnstructuredGrid>::New();

        // add vtk points and cells to the grid
        gridVtk->SetPoints(pointsVtk);
        gridVtk->SetCells(VTK_TETRA, cellsVtk);

        return gridVtk;
    }

    template <class cellT, int D>
    auto vtkWriter(std::string fileName, const mesh::mesh_t<cellT, D> & mesh) -> void
    {
        // create vtk points and cells from the given mesh
        const auto [pointsVtk, cellsVtk] = createVtkPointsAndCells(mesh);

        // create vtk unstructured grid
        const auto gridVtk = createVtkUnstructuredGrid(pointsVtk, cellsVtk);

        // write the grid to file
        auto writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
        writer->SetFileName((fileName + ".vtk").c_str());

#if VTK_MAJOR_VERSION <= 8
        writer->SetInput(gridVtk);
#else
        writer->SetInputData(gridVtk);
#endif

        writer->Write();
    }

}    // namespace mito::mesh

#endif    // mito_mesh_vtk_writer_h