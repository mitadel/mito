// code guard
#if !defined(mito_io_externals_h)
#define mito_io_externals_h

// externals
#include <fstream>
#include <unordered_map>

#ifdef WITH_VTK
#include <vtkCellArray.h>
#include <vtkFloatArray.h>
#include <vtkLine.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkTetra.h>
#include <vtkTriangle.h>
#include <vtkUnstructuredGrid.h>
#include <vtkXMLUnstructuredGridWriter.h>
#endif    // WITH_VTK

// support
#include "../geometry.h"
#include "../mesh.h"
#include "../topology.h"

#endif

// end of file
