mito_test_driver(tests/mito.lib/divergence-theorem/main.cc)
mito_test_driver(tests/mito.lib/erase-element/main.cc)
mito_test_driver(tests/mito.lib/fields/main.cc)
mito_test_driver(tests/mito.lib/flip-diagonal/main.cc)
mito_test_driver(tests/mito.lib/functions/main.cc)
mito_test_driver(tests/mito.lib/gent/main.cc)
mito_test_driver(tests/mito.lib/io/loadSummitMesh2D.cc)
mito_test_driver(tests/mito.lib/io/loadSummitMesh3D.cc)
mito_test_driver(tests/mito.lib/io/loadSummitSegmentMesh3D.cc)

if(WITH_VTK)
    mito_test_driver(tests/mito.lib/io/pointCloudToVtk.cc)
    mito_test_driver(tests/mito.lib/io/summitMeshToVtk.cc)
    mito_test_driver(tests/mito.lib/io/tetraToVtk.cc)
endif()

mito_test_driver(tests/mito.lib/mesh/main.cc)
mito_test_driver(tests/mito.lib/oriented-simplex/main.cc)
mito_test_driver(tests/mito.lib/point/main.cc)
mito_test_driver(tests/mito.lib/pyre-grids/main.cc)
mito_test_driver(tests/mito.lib/quadrature/main.cc)
mito_test_driver(tests/mito.lib/segmented-container/main.cc)
mito_test_driver(tests/mito.lib/segmented-container-iterator/main.cc)
mito_test_driver(tests/mito.lib/shared-pointer/main.cc)
mito_test_driver(tests/mito.lib/simplices/main.cc)
mito_test_driver(tests/mito.lib/traits/main.cc)

if(WITH_VTK)
    add_test(NAME tests.mito.lib.io.pytest
        COMMAND ${BASH_PROGRAM} -c "pytest -v ${CMAKE_SOURCE_DIR}/tests/mito.lib/io/check_output_vtk.py"
    )

    # some tests require cleanup
    add_test(NAME tests.mito.lib.io.cleanup
        COMMAND ${BASH_PROGRAM} -c "rm ${CMAKE_SOURCE_DIR}/tests/mito.lib/io/*.vtk"
    )

    # some tests must happen in a specific order
    set_property(TEST tests.mito.lib.io.pytest PROPERTY
        DEPENDS mito.lib.io.main.cc
    )
    set_property(TEST tests.mito.lib.io.cleanup PROPERTY
        DEPENDS tests.mito.lib.io.pytest
    )
endif()