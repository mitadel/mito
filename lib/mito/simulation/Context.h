// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_simulation_Context_h)
#define mito_simulation_Context_h


namespace mito::simulation {

    class Context {

      public:
        // me
        using context_type = Context;

      public:
        // default constructor
        Context();

        // destructor
        ~Context();

      private:
        // delete move constructor
        Context(context_type &&) noexcept = delete;

        // delete copy constructor
        Context(const context_type &) = delete;

        // delete assignment operator
        Context & operator=(const context_type &) = delete;

        // delete move assignment operator
        Context & operator=(context_type &&) noexcept = delete;

      public:
        // accessor for my process' rank
        auto task_id() const -> int;

        // accessor for the size of the job
        auto n_tasks() const -> int;

      private:
        // my process' rank
        int _task_id;

        // the number of tasks
        int _n_tasks;
    };

}    // namespace mito


// get the inline definitions
#define mito_simulation_Context_icc
#include "Context.icc"
#undef mito_simulation_Context_icc


#endif    // mito_simulation_Context_h

// end of file
