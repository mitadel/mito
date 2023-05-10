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
        // Create some points.
        vtkSmartPointer<vtkPoints> pointsVtk = vtkSmartPointer<vtkPoints>::New();

        for (const auto & node : mesh.geometry().nodes()) {
            const auto & point = *node.second.handle();
            std::cout << point << std::endl;
            const auto & coordinates = point.coordinates();
            pointsVtk->InsertNextPoint(coordinates[0], coordinates[1], coordinates[2]);
        }

        // Create a tetrahedron from the points.
        vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
        vtkSmartPointer<vtkTetra> tetra = vtkSmartPointer<vtkTetra>::New();

        tetra->GetPointIds()->SetId(0, 0);
        tetra->GetPointIds()->SetId(1, 1);
        tetra->GetPointIds()->SetId(2, 2);
        tetra->GetPointIds()->SetId(3, 3);
        cells->InsertNextCell(tetra);

        // Create an unstructured grid and set the points and cells.
        vtkSmartPointer<vtkUnstructuredGrid> grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
        grid->SetPoints(pointsVtk);
        grid->SetCells(VTK_TETRA, cells);

        // Write the grid to a VTK XML file.
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