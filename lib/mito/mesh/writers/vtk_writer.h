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
        // map mesh points to the index of the vtk points. Points that are shared among
        // multiple elements have the same index.
        std::unordered_map<
            mito::geometry::point_t<D>, int,
            mito::utilities::hash_function<mito::geometry::point_t<D>>>
            mapPoints;

        // global index assigned to each vtk point
        auto indexPointVtk = 0;

        // create vtk points
        vtkSmartPointer<vtkPoints> pointsVtk = vtkSmartPointer<vtkPoints>::New();

        // create vtk cells from the points
        vtkSmartPointer<vtkCellArray> cellsVtk = vtkSmartPointer<vtkCellArray>::New();

        // loop over the cells
        for (const auto & cell : mesh.cells()) {
            // create vtk cell
            vtkSmartPointer<vtkTetra> tetrahedron = vtkSmartPointer<vtkTetra>::New();

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
                    // retrieve its coordinates
                    const auto & coordinates = pPoint.handle()->coordinates();
                    // insert the new vtk point
                    pointsVtk->InsertNextPoint(coordinates[0], coordinates[1], coordinates[2]);
                    // and add the point to the map with its global index
                    mapPoints[pPoint] = indexPointVtk;
                    // update global index for the vtk point
                    ++indexPointVtk;
                }

                // set the id of the point
                tetrahedron->GetPointIds()->SetId(indexLocalPointVtk, mapPoints[pPoint]);

                // update local index for the points in the cell
                ++indexLocalPointVtk;
            }

            // insert the new cell
            cellsVtk->InsertNextCell(tetrahedron);
        }

        // create an unstructured grid and set the points and cells
        vtkSmartPointer<vtkUnstructuredGrid> grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
        grid->SetPoints(pointsVtk);
        grid->SetCells(VTK_TETRA, cellsVtk);

        // write the grid to a file
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