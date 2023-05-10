// code guard
#if !defined(mito_mesh_vtk_writer_h)
#define mito_mesh_vtk_writer_h

#include <vtkCellArray.h>
#include <vtkFloatArray.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkSmartPointer.h>
#include <vtkTetra.h>
#include <vtkUnstructuredGrid.h>
#include <vtkXMLUnstructuredGridWriter.h>


namespace mito::mesh {
    template <class cellT, int D>
    auto vtk_writer(std::string fileName, mito::mesh::mesh_t<cellT, D> & mesh) -> void
    {
        std::unordered_map<
            mito::geometry::point_t<D>, int,
            mito::utilities::hash_function<mito::geometry::point_t<D>>>
            mapPoints;
        auto pointVtkId = 0;

        // create vtk points
        vtkSmartPointer<vtkPoints> pointsVtk = vtkSmartPointer<vtkPoints>::New();
        // create tetra from the points
        vtkSmartPointer<vtkCellArray> cellsVtk = vtkSmartPointer<vtkCellArray>::New();

        // loop over all cells
        for (const auto & cell : mesh.cells()) {
            // retrieve vertices of the cell
            topology::vertex_set_t vertices;
            cell->vertices(vertices);

            vtkSmartPointer<vtkTetra> tetrahedron = vtkSmartPointer<vtkTetra>::New();
            auto localVtkId = 0;
            for (const auto & vertex : vertices) {
                const auto pPoint = mesh.geometry().point(vertex);

                if (mapPoints.count(pPoint) == 0) {
                    const auto & coordinates = pPoint.handle()->coordinates();
                    pointsVtk->InsertNextPoint(coordinates[0], coordinates[1], coordinates[2]);
                    mapPoints[pPoint] = pointVtkId;
                    ++pointVtkId;
                }

                tetrahedron->GetPointIds()->SetId(localVtkId, mapPoints[pPoint]);

                ++localVtkId;
            }
            cellsVtk->InsertNextCell(tetrahedron);
        }

        // create an unstructured grid and set the points and cells.
        vtkSmartPointer<vtkUnstructuredGrid> grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
        grid->SetPoints(pointsVtk);
        grid->SetCells(VTK_TETRA, cellsVtk);

        // write the grid to a file.
        vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer =
            vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
        writer->SetFileName((fileName + ".vtk").c_str());
#if VTK_MAJOR_VERSION <= 8
        writer->SetInput(grid);
#else
        writer->SetInputData(grid);
#endif
        writer->Write();
    }

}    // namespace mito::mesh

#endif    // mito_mesh_vtk_writer_h