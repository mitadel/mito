import os
import vtk


def load_vtk_mesh(filename):
    # get file path
    test_directory = os.path.dirname(os.path.abspath(__file__))
    filepath = test_directory + "/" + filename
    print("File path:", filepath)

    # load the VTK file
    reader = vtk.vtkXMLUnstructuredGridReader()
    reader.SetFileName(filepath)
    reader.Update()
    return reader.GetOutput()

def cleanup(filename):
    os.remove(filename)