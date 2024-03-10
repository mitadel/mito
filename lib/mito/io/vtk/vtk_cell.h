// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_io_vtk_cell_h)
#define mito_io_vtk_cell_h


namespace mito::io::vtk {

    // associate to a mito cell type its corresponding vtk cell type
    template <class cellT>
    struct vtkCellT {};

    template <>
    struct vtkCellT<topology::segment_t> {
        using type = vtkLine;
    };

    template <>
    struct vtkCellT<topology::triangle_t> {
        using type = vtkTriangle;
    };

    template <>
    struct vtkCellT<topology::tetrahedron_t> {
        using type = vtkTetra;
    };

}

#endif

// end of file
