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

def test_TetraToVtk():
    # get the loaded mesh
    mesh = load_mesh("tetra_output.vtk")

    # check the number of cells (tetrahedra)
    num_tetrahedra = mesh.GetNumberOfCells()
    print("Number of tetrahedra:", num_tetrahedra)
    assert num_tetrahedra == 8

def test_SummitMeshToVtk():
    # get the loaded mesh
    mesh = load_mesh("cube_output.vtk")

    # check the number of cells (tetrahedra)
    num_tetrahedra = mesh.GetNumberOfCells()
    print("Number of tetrahedra:", num_tetrahedra)
    assert num_tetrahedra == 368

def test_PointCloudToVtk():
    # get the loaded mesh
    mesh = load_mesh("point_cloud_output.vtk")

    # check the number of cells (tetrahedra)
    num_tetrahedra = mesh.GetNumberOfCells()
    print("Number of tetrahedra:", num_tetrahedra)
    assert num_tetrahedra == 0
