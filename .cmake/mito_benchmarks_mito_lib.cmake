# -*- cmake -*-
#
# Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
#

#
# mito
#


# materials
mito_benchmark_driver(benchmarks/mito.lib/materials/gent.cc)

# tensor
mito_benchmark_driver(benchmarks/mito.lib/tensor/wedge.cc)

# integration
mito_benchmark_driver(benchmarks/mito.lib/integration/integration.cc)

# operators
mito_benchmark_driver(benchmarks/mito.lib/operators/laplacian.cc)

# linalg
if(WITH_PETSC)
    mito_benchmark_driver(benchmarks/mito.lib/linalg/petsc_assembly.cc)
    mito_benchmark_driver(benchmarks/mito.lib/linalg/petsc_sparsity.cc)
endif()

# PDEs
if(WITH_PETSC)
    # poisson boundary value problem
    mito_benchmark_driver(benchmarks/mito.lib/pdes/poisson.cc)
    # advection-diffusion-reaction boundary value problem
    mito_benchmark_driver(benchmarks/mito.lib/pdes/advection_diffusion_reaction.cc)
endif()

# blocks composition
mito_benchmark_driver(benchmarks/mito.lib/blocks/composition.cc)

# end of file
