// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// externals
#include <vtkLine.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkDoubleArray.h>
#include <vtkSmartPointer.h>
#include <vtkTetra.h>
#include <vtkTriangle.h>
#include <vtkUnstructuredGrid.h>
#include <vtkXMLUnstructuredGridWriter.h>
#ifdef WITH_PARALLEL_VTK
#include <vtkXMLPUnstructuredGridWriter.h>
#include <vtkMPIController.h>
#endif    // WITH_PARALLEL_VTK

// support
#include "../../discrete.h"

// end of file
