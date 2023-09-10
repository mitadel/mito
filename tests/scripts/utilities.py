import os
import vtk


def load_vtk_mesh(filename):
    # load the VTK file
    reader = vtk.vtkXMLUnstructuredGridReader()
    reader.SetFileName(filename)
    reader.Update()
    return reader.GetOutput()

def cleanup(filename):
    os.remove(filename)