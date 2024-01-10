import pytest

from utilities import load_vtk_mesh, cleanup


def test_SummitMeshToVtk():
    filename = "rectangle_output.vtu"

    # get the loaded mesh
    mesh = load_vtk_mesh(filename)

    # check the number of cells (tetrahedra)
    num_cells = mesh.GetNumberOfCells()
    print("Number of cells:", num_cells)
    assert num_cells == 3690

    # check points of the mesh
    points = mesh.GetPoints()
    num_points = points.GetNumberOfPoints()
    print("Number of points:", num_points)
    assert num_points == 1930

    # remove file
    cleanup(filename)