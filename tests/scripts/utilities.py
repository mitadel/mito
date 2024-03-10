#
# Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
#

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