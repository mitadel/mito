// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::vtk {

    template <int D>
    class ParallelGridVTKWriter : public Writer {

      public:
        // the grid dimension
        static constexpr int dim = D;
        // the type of grid
        using grid_type = vtkSmartPointer<vtkUnstructuredGrid>;

      protected:
        // constructor
        // (protected so this class cannot be instantiated unless by the derived classes)
        ParallelGridVTKWriter(std::string filename) : Writer(filename), _grid(grid_type::New()) {}

      public:
        auto write() const -> void override
        {
            // setup the vtk mpi controller
            auto * controller = vtkMPIController::New();
            int mpiInitializedExternally = 1;
            controller->Initialize(nullptr, nullptr, mpiInitializedExternally);
            controller->SetGlobalController(controller);

            // get the size of the job
            int n_tasks = controller->GetNumberOfProcesses();

            // get my process' rank
            int task_id = controller->GetLocalProcessId();

            // create a new writer (for this task)
            auto writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
            // set the name of the output file
            writer->SetFileName((this->_filename + "_" + std::to_string(task_id) + ".vtu").data());
            // sign the grid up for writing
#if VTK_MAJOR_VERSION <= 8
            writer->SetInput(_grid);
#else
            writer->SetInputData(_grid);
#endif
            // write this task's grid to file
            writer->Write();

            // create a new parallel grid writer
            auto pwriter = vtkSmartPointer<vtkXMLPUnstructuredGridWriter>::New();
            pwriter->SetFileName((this->_filename + ".pvtu").data());
            // sign the grid up for writing
            pwriter->SetInputData(_grid);
            // set the number of pieces to be written
            pwriter->SetNumberOfPieces(n_tasks);
            // write the pieces associated with this task only
            pwriter->SetStartPiece(task_id);
            pwriter->SetEndPiece(task_id);
            pwriter->Write();

            // finalize the controller
            controller->Finalize(mpiInitializedExternally);

            // all done
            return;
        }

      protected:
        // the grid
        grid_type _grid;
    };

}    // namespace mito::io::vtk


// end of file
