// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::simulation {

    class CommunicationManager {

      public:
        // me
        using communication_manager_type = CommunicationManager;

      public:
        // constructor
        explicit CommunicationManager(const Context & context);

      public:
        // my process rank
        auto task_id() const -> int;

        // the number of processes
        auto n_tasks() const -> int;

        // synchronization barrier
        auto barrier() const -> void;

        // reduction operators
        template <class valueT>
        auto reduce_sum(valueT local) const -> valueT;

        template <class valueT>
        auto reduce_min(valueT local) const -> valueT;

        template <class valueT>
        auto reduce_max(valueT local) const -> valueT;

        auto reduce_norm(double local_norm) const -> double;

        auto reduce_min_location(double local) const -> std::pair<double, int>;

        auto reduce_max_location(double local) const -> std::pair<double, int>;

        // point-to-point communication
        template <class valueT>
        auto send(const valueT & value, int destination, int tag) const -> void;

        template <class valueT>
        auto receive(int source, int tag) const -> valueT;

        template <class valueT>
        auto send(const std::vector<valueT> & values, int destination, int tag) const -> void;

        template <class valueT>
        auto receive(std::size_t count, int source, int tag) const -> std::vector<valueT>;

        // tag helpers
        static auto encode_tag(int origin, int destination) -> int;

        static auto decode_tag(int tag) -> std::pair<int, int>;

      private:
#if (WITH_MPI)
        template <class valueT>
        static auto _mpi_datatype() -> MPI_Datatype;

        template <class valueT>
        static auto _throw_invalid_mpi_type() -> MPI_Datatype;

        template <class valueT>
        auto _reduce(valueT local, MPI_Op op) const -> valueT;
#endif

      private:
        // rank and job size
        int _task_id;
        int _n_tasks;

#if (WITH_MPI)
        // communicator managed by the simulation context
        MPI_Comm _communicator;
#endif

      public:
        static constexpr int tag_multiplier = 10000;
    };

}    // namespace mito::simulation


// get the inline definitions
#define mito_simulation_CommunicationManager_icc
#include "CommunicationManager.icc"
#undef mito_simulation_CommunicationManager_icc


// end of file
