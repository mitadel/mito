# -*- cmake -*-
#
# Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
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


# end of file
