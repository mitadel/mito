#  geometry
mito_test_driver(tests/mito.lib/geometry/coordinates.cc)
mito_test_driver(tests/mito.lib/geometry/cell_barycenter.cc)
mito_test_driver(tests/mito.lib/geometry/point.cc)

# io
mito_test_driver(tests/mito.lib/io/read_mesh_summit_2D.cc)
mito_test_driver(tests/mito.lib/io/read_mesh_summit_3D.cc)
mito_test_driver(tests/mito.lib/io/read_mesh_summit_segment_3D.cc)

if(WITH_VTK)
    mito_test_driver_pytest_check(tests/mito.lib/io/write_point_cloud_vtk.cc)
    mito_test_driver_pytest_check(tests/mito.lib/io/summit_to_vtk_mesh_3D.cc)
    mito_test_driver_pytest_check(tests/mito.lib/io/summit_to_vtk_mesh_2D.cc)
endif()

# manifolds
mito_test_driver(tests/mito.lib/manifolds/cube_volume.cc)
mito_test_driver(tests/mito.lib/manifolds/disk_volume_polar_cartesian.cc)
mito_test_driver(tests/mito.lib/manifolds/euclidean_gradient.cc)
mito_test_driver(tests/mito.lib/manifolds/euclidean_manifold_2D.cc)
mito_test_driver(tests/mito.lib/manifolds/euclidean_manifold_3D.cc)
mito_test_driver(tests/mito.lib/manifolds/euclidean_metric_2D.cc)
mito_test_driver(tests/mito.lib/manifolds/euclidean_metric_3D.cc)
mito_test_driver(tests/mito.lib/manifolds/fields.cc)
mito_test_driver(tests/mito.lib/manifolds/fields_traits.cc)
mito_test_driver(tests/mito.lib/manifolds/one_forms.cc)
mito_test_driver(tests/mito.lib/manifolds/metric.cc)
mito_test_driver(tests/mito.lib/manifolds/polar_metric.cc)
mito_test_driver(tests/mito.lib/manifolds/contractions.cc)
mito_test_driver(tests/mito.lib/manifolds/tensor_product_forms.cc)
mito_test_driver(tests/mito.lib/manifolds/triangle_2D.cc)
mito_test_driver(tests/mito.lib/manifolds/tetrahedron_3D.cc)
mito_test_driver(tests/mito.lib/manifolds/wedge_product_forms.cc)

# materials
mito_test_driver(tests/mito.lib/materials/gent.cc)

# math
mito_test_driver(tests/mito.lib/math/functions.cc)
mito_test_driver(tests/mito.lib/math/permutation_sign.cc)
mito_test_driver(tests/mito.lib/math/tensor_functions.cc)

# mesh
mito_test_driver(tests/mito.lib/mesh/build_mesh.cc)
mito_test_driver(tests/mito.lib/mesh/tetra_1D.cc)
mito_test_driver(tests/mito.lib/mesh/tetra_2D.cc)
mito_test_driver(tests/mito.lib/mesh/tetra_2D_rectangle.cc)
mito_test_driver(tests/mito.lib/mesh/tetra_3D.cc)
mito_test_driver(tests/mito.lib/mesh/tetra_3D_cube.cc)
mito_test_driver(tests/mito.lib/mesh/tetra_zero_subdivisions.cc)
mito_test_driver(tests/mito.lib/mesh/tetra_multiple_subdivisions.cc)
mito_test_driver(tests/mito.lib/mesh/erase_element.cc)

if(WITH_METIS)
    mito_test_driver(tests/mito.lib/mesh/metis_partitioner.cc)
    mito_test_driver(tests/mito.lib/mesh/metis_partitioner_load_mesh.cc)
    mito_test_driver(tests/mito.lib/mesh/metis_partitioner_single_partition.cc)

    if(WITH_MPI)
        mito_test_driver_mpi(tests/mito.lib/mesh/metis_partitioner_mpi.cc 2)
        mito_test_driver_mpi(tests/mito.lib/mesh/metis_partitioner_mpi_load_mesh.cc 2)
    endif()
endif()

# topology
mito_test_driver(tests/mito.lib/topology/cell_edges.cc)
mito_test_driver(tests/mito.lib/topology/erase_element.cc)
mito_test_driver(tests/mito.lib/topology/flip_diagonal.cc)
mito_test_driver(tests/mito.lib/topology/oriented_simplex_segment.cc)
mito_test_driver(tests/mito.lib/topology/oriented_simplex_triangle.cc)
mito_test_driver(tests/mito.lib/topology/tetrahedron.cc)
mito_test_driver(tests/mito.lib/topology/triangle.cc)
mito_test_driver(tests/mito.lib/topology/segment.cc)
mito_test_driver(tests/mito.lib/topology/simplices.cc)

# utilities
mito_test_driver(tests/mito.lib/utilities/segmented_container.cc)
mito_test_driver(tests/mito.lib/utilities/segmented_container_iterator.cc)
mito_test_driver(tests/mito.lib/utilities/shared_pointer.cc)

# quadrature
mito_test_driver(tests/mito.lib/quadrature/quadrature_parametric_segment.cc)
mito_test_driver(tests/mito.lib/quadrature/quadrature_parametric_triangle.cc)
mito_test_driver(tests/mito.lib/quadrature/quadrature_parametric_tetrahedron.cc)

# integration
# mito_test_driver(tests/mito.lib/integration/divergence_theorem.cc) #TOFIX
mito_test_driver(tests/mito.lib/integration/quadrature_1D.cc)
mito_test_driver(tests/mito.lib/integration/quadrature_2D.cc)
mito_test_driver(tests/mito.lib/integration/quadrature_load_mesh_2D.cc)
# mito_test_driver(tests/mito.lib/integration/quadrature_flip_segment_3D.cc) #TOFIX

if(${WITH_METIS} AND ${WITH_MPI})
    mito_test_driver_mpi(tests/mito.lib/integration/quadrature_load_mesh_2D_mpi.cc 2)
endif()

if(${WITH_VTK})
    mito_test_driver_pytest_check(tests/mito.lib/integration/write_tetra_mesh_to_vtk.cc)
endif()

# unsorted
mito_test_driver(tests/pyre-grids/main.cc)