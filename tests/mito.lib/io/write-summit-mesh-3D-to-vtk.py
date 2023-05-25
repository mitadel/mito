import os
import pytest
import vtk


def load_mesh(filename):
    # get file path
    test_directory = os.path.dirname(os.path.abspath(__file__))
    filepath = test_directory + "/" + filename
    print("File path:", filepath)

    # load the VTK file
    reader = vtk.vtkXMLUnstructuredGridReader()
    reader.SetFileName(filepath)
    reader.Update()
    return reader.GetOutput()


def test_SummitMeshToVtk():
    filename = "cube_output.vtk"

    # get the loaded mesh
    mesh = load_mesh(filename)

    # check the number of cells (tetrahedra)
    num_cells = mesh.GetNumberOfCells()
    print("Number of tetrahedra:", num_cells)
    assert num_cells == 368

    # check points of the mesh
    points = mesh.GetPoints()
    num_points = points.GetNumberOfPoints()
    print("Number of points:", num_points)
    assert num_points == 139

    # cleanup
    os.remove(filename)