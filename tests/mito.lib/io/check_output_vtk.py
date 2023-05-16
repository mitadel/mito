import vtk

# Load vtk file
reader = vtk.vtkXMLUnstructuredGridReader()
reader.SetFileName("tetra_output.vtk")
reader.Update()

# Get the loaded mesh
mesh = reader.GetOutput()

# Check the number of cells (tetrahedra)
num_tetrahedra = mesh.GetNumberOfCells()
print("Number of tetrahedra:", num_tetrahedra)
assert(num_tetrahedra == 8)
