# -*- cmake -*-
#
# Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
#

#
# mito
#

# materials
mito_benchmark_driver(benchmarks/mito.lib/materials/gent.cc)
mito_benchmark_driver(benchmarks/mito.lib/tensor/wedge.cc)
mito_benchmark_driver(benchmarks/mito.lib/integration/integration.cc)
mito_benchmark_driver(benchmarks/mito.lib/fields/laplacian.cc)

# end of file
