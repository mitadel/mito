import pytest

from helper import load_vtk_mesh, cleanup


def test_WriteTetraMeshToVtk():
    filename = "tetra_output.vtk"

    # get the loaded mesh
    mesh = load_vtk_mesh(filename)

    # check the number of cells (tetrahedra)
    num_cells = mesh.GetNumberOfCells()
    print("Number of tetrahedra:", num_cells)
    assert num_cells == 8
    
    # check points of the mesh
    points = mesh.GetPoints()
    num_points = points.GetNumberOfPoints()
    print("Number of points:", num_points)
    assert num_points == 10

    # remove file
    cleanup(filename)