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

# fields
mito_benchmark_driver(benchmarks/mito.lib/fields/laplacian.cc)

if(WITH_PETSC)
    # poisson boundary value problem on a unit square
    mito_benchmark_driver(benchmarks/mito.lib/pdes/poisson.cc)
    # poisson boundary value problem on a hollow disk
    mito_benchmark_driver(benchmarks/mito.lib/pdes/laplacian_hollow_disk.cc)
endif()

# end of file
