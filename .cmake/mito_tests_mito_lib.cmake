#
# Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
#


# functions
mito_test_driver(tests/mito.lib/functions/algebra.cc)
mito_test_driver(tests/mito.lib/functions/function_from_functor.cc)
mito_test_driver(tests/mito.lib/functions/derivative_constants.cc)
mito_test_driver(tests/mito.lib/functions/derivative_chain_rule.cc)
mito_test_driver(tests/mito.lib/functions/derivative_higher_order.cc)
mito_test_driver(tests/mito.lib/functions/derivative_sum.cc)
mito_test_driver(tests/mito.lib/functions/derivative_product.cc)
mito_test_driver(tests/mito.lib/functions/derivative_inverse.cc)
mito_test_driver(tests/mito.lib/functions/partial_derivatives.cc)

#  geometry
mito_test_driver(tests/mito.lib/geometry/coordinates.cc)
mito_test_driver(tests/mito.lib/geometry/coordinate_system.cc)
mito_test_driver(tests/mito.lib/geometry/coordinate_transformation.cc)
mito_test_driver(tests/mito.lib/geometry/barycenter_segment_1D.cc)
mito_test_driver(tests/mito.lib/geometry/barycenter_segment_2D.cc)
mito_test_driver(tests/mito.lib/geometry/barycenter_segment_3D.cc)
mito_test_driver(tests/mito.lib/geometry/barycenter_triangle_2D.cc)
mito_test_driver(tests/mito.lib/geometry/barycenter_triangle_3D.cc)
mito_test_driver(tests/mito.lib/geometry/barycenter_tetrahedron_3D.cc)
mito_test_driver(tests/mito.lib/geometry/cell_directors.cc)
mito_test_driver(tests/mito.lib/geometry/point.cc)
mito_test_driver(tests/mito.lib/geometry/euclidean_metric_2D.cc)
mito_test_driver(tests/mito.lib/geometry/euclidean_metric_3D.cc)
mito_test_driver(tests/mito.lib/geometry/euclidean_submanifold_metric_3D.cc)
mito_test_driver(tests/mito.lib/geometry/cube_volume.cc)
mito_test_driver(tests/mito.lib/geometry/metric.cc)
mito_test_driver(tests/mito.lib/geometry/euclidean_metric_space.cc)
mito_test_driver(tests/mito.lib/geometry/polar_metric_space.cc)
mito_test_driver(tests/mito.lib/geometry/spherical_metric_space.cc)

# fem
mito_test_driver(tests/mito.lib/fem/quadrature_field.cc)
mito_test_driver(tests/mito.lib/fem/nodal_field_sphere.cc)

# io
mito_test_driver(tests/mito.lib/io/read_mesh_summit_2D.cc)
mito_test_driver(tests/mito.lib/io/read_mesh_summit_3D.cc)
mito_test_driver(tests/mito.lib/io/read_mesh_summit_segment_3D.cc)
mito_test_driver(tests/mito.lib/io/summit_to_summit_mesh_2D.cc)

if(WITH_VTK)
    mito_test_driver_pytest_check(tests/mito.lib/io/write_point_cloud_vtk.cc)
    mito_test_driver_pytest_check(tests/mito.lib/io/summit_to_vtk_mesh_3D.cc)
    mito_test_driver_pytest_check(tests/mito.lib/io/summit_to_vtk_mesh_2D.cc)
    mito_test_driver_pytest_check(tests/mito.lib/io/summit_to_vtk_mesh_lattice_3D.cc)

    if(WITH_PARALLEL_VTK)
        mito_test_driver_mpi(tests/mito.lib/io/parallel_vtk_cloud_writer.cc 2)
        mito_test_driver_mpi(tests/mito.lib/io/parallel_vtk_field_writer.cc 2)
    endif()
endif()

# solvers
if(WITH_PETSC)
    mito_test_driver(tests/mito.lib/solvers/petsc_external_initialize.cc)
    mito_test_driver(tests/mito.lib/solvers/petsc_solve_linear_system.cc)
endif()

# tensor
mito_test_driver(tests/mito.lib/tensor/one_forms.cc)
mito_test_driver(tests/mito.lib/tensor/contractions.cc)
mito_test_driver(tests/mito.lib/tensor/wedge_product_forms.cc)
mito_test_driver(tests/mito.lib/tensor/tensor_product_forms.cc)

# fields
mito_test_driver(tests/mito.lib/fields/fields.cc)
mito_test_driver(tests/mito.lib/fields/fields_traits.cc)
mito_test_driver(tests/mito.lib/fields/calculus_scalar_field.cc)
mito_test_driver(tests/mito.lib/fields/polar_metric_field.cc)
mito_test_driver(tests/mito.lib/fields/spherical_metric_field.cc)

# manifolds
mito_test_driver(tests/mito.lib/manifolds/euclidean_gradient.cc)
mito_test_driver(tests/mito.lib/manifolds/polar_gradient.cc)
mito_test_driver(tests/mito.lib/manifolds/spherical_gradient.cc)
mito_test_driver(tests/mito.lib/manifolds/triangle_2D.cc)
mito_test_driver(tests/mito.lib/manifolds/triangle_3D.cc)
mito_test_driver(tests/mito.lib/manifolds/tetrahedron_3D.cc)
mito_test_driver(tests/mito.lib/manifolds/tetra_rectangle_2D.cc)
mito_test_driver(tests/mito.lib/manifolds/tetra_cube_3D.cc)
mito_test_driver(tests/mito.lib/manifolds/volume_half_ball.cc)
mito_test_driver(tests/mito.lib/manifolds/volume_disk_polar_cartesian.cc)
mito_test_driver(tests/mito.lib/manifolds/volume_disk_change_coordinates.cc)
mito_test_driver(tests/mito.lib/manifolds/surface_half_sphere_cartesian.cc)
mito_test_driver(tests/mito.lib/manifolds/surface_half_sphere_spherical.cc)

# materials
mito_test_driver(tests/mito.lib/materials/gent.cc)
mito_test_driver(tests/mito.lib/materials/linear_elastic.cc)

# math
mito_test_driver(tests/mito.lib/math/permutation_sign.cc)

# mesh
mito_test_driver(tests/mito.lib/mesh/ball.cc)
mito_test_driver(tests/mito.lib/mesh/build_mesh.cc)
mito_test_driver(tests/mito.lib/mesh/erase_duplicates.cc)
mito_test_driver(tests/mito.lib/mesh/filter_ball.cc)
mito_test_driver(tests/mito.lib/mesh/tetra_segment_1D.cc)
mito_test_driver(tests/mito.lib/mesh/tetra_triangle_2D.cc)
mito_test_driver(tests/mito.lib/mesh/tetra_triangle_3D.cc)
mito_test_driver(tests/mito.lib/mesh/tetra_tetrahedron_3D.cc)
mito_test_driver(tests/mito.lib/mesh/tetra_zero_subdivisions.cc)
mito_test_driver(tests/mito.lib/mesh/tetra_multiple_subdivisions.cc)
mito_test_driver(tests/mito.lib/mesh/erase_element.cc)
mito_test_driver(tests/mito.lib/mesh/sphere.cc)
mito_test_driver(tests/mito.lib/mesh/summit_read_write.cc)

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
mito_test_driver(tests/mito.lib/topology/erase_element_check_vertices.cc)
mito_test_driver(tests/mito.lib/topology/erase_element_check_segments.cc)
mito_test_driver(tests/mito.lib/topology/flip_diagonal.cc)
mito_test_driver(tests/mito.lib/topology/oriented_simplex_segment.cc)
mito_test_driver(tests/mito.lib/topology/oriented_simplex_triangle.cc)
mito_test_driver(tests/mito.lib/topology/tetrahedron.cc)
mito_test_driver(tests/mito.lib/topology/triangle.cc)
mito_test_driver(tests/mito.lib/topology/segment.cc)
mito_test_driver(tests/mito.lib/topology/simplices.cc)

# utilities
mito_test_driver(tests/mito.lib/utilities/repository.cc)
mito_test_driver(tests/mito.lib/utilities/repository_iterator.cc)
mito_test_driver(tests/mito.lib/utilities/segmented_vector.cc)
mito_test_driver(tests/mito.lib/utilities/segmented_vector_erase.cc)
mito_test_driver(tests/mito.lib/utilities/segmented_vector_move.cc)
mito_test_driver(tests/mito.lib/utilities/segmented_vector_iterator.cc)
mito_test_driver(tests/mito.lib/utilities/segmented_vector_subscript.cc)
mito_test_driver(tests/mito.lib/utilities/segmented_vector_print.cc)
mito_test_driver(tests/mito.lib/utilities/shared_pointer.cc)

# quadrature
mito_test_driver(tests/mito.lib/quadrature/quadrature_parametric_segment.cc)
mito_test_driver(tests/mito.lib/quadrature/quadrature_parametric_triangle.cc)
mito_test_driver(tests/mito.lib/quadrature/quadrature_parametric_tetrahedron.cc)
mito_test_driver(tests/mito.lib/quadrature/quadrature_segment_1D.cc)
mito_test_driver(tests/mito.lib/quadrature/quadrature_segment_3D.cc)
mito_test_driver(tests/mito.lib/quadrature/quadrature_triangle_2D.cc)
mito_test_driver(tests/mito.lib/quadrature/quadrature_square_3D.cc)

# integration
mito_test_driver(tests/mito.lib/integration/divergence_theorem.cc)
mito_test_driver(tests/mito.lib/integration/quadrature_load_mesh_2D.cc)
mito_test_driver(tests/mito.lib/integration/quadrature_flip_segment_3D.cc)

if(${WITH_METIS} AND ${WITH_MPI})
    mito_test_driver_mpi(tests/mito.lib/integration/quadrature_load_mesh_2D_mpi.cc 2)
endif()

if(${WITH_VTK})
    mito_test_driver_pytest_check(tests/mito.lib/integration/write_tetra_mesh_to_vtk.cc)
endif()
