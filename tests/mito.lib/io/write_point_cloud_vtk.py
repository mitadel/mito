import pytest

from utilities import load_vtk_mesh, cleanup


def test_WritePointCloudToVtk():
    filename = "point_cloud_output.vtk"

    # get the loaded mesh
    mesh = load_vtk_mesh(filename)

    # check the number of cells (tetrahedra)
    num_tetrahedra = mesh.GetNumberOfCells()
    print("Number of tetrahedra:", num_tetrahedra)
    assert num_tetrahedra == 0

    # check points of the mesh
    points = mesh.GetPoints()
    num_points = points.GetNumberOfPoints()
    print("Number of points:", num_points)
    assert num_points == 3

    # remove file
    cleanup(filename)