=========
Tutorials
=========

This section showcases practical examples of using MiTo for computational modeling tasks.
Each tutorial is a complete, self-contained C++ application that can be built and run independently.

.. toctree::
   :maxdepth: 2

   tutorial_1_functions
   tutorial_2_tensors
   tutorial_3_fields
   tutorial_4_divergence_theorem_2d
   tutorial_5_divergence_theorem_2d_in_3d

Building Tutorials Locally
==========================

Each tutorial can be built locally using CMake:

.. code-block:: bash

   cd tutorial/<tutorial-name>
   mkdir -p build
   cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
   cmake --build build
   ./build/main

The tutorial binaries will output results to the terminal. Some tutorials generate mesh files (VTU format) that can be visualized in ParaView.
