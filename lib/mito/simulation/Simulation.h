// code guard
#if !defined(mito_simulation_Simulation_h)
#define mito_simulation_Simulation_h


namespace mito::simulation {

    class Simulation {

      public:
        // me
        using simulation_type = Simulation;
        // the execution context type
        using execution_context_type = context_t;
        // the clock type
        using clock_type = scalar_t;

      public:
        // default constructor
        Simulation();

        // destructor
        ~Simulation() {}

      private:
        // delete move constructor
        Simulation(simulation_type &&) noexcept = delete;

        // delete copy constructor
        Simulation(const simulation_type &) = delete;

        // delete assignment operator
        Simulation & operator=(const simulation_type &) = delete;

        // delete move assignment operator
        Simulation & operator=(simulation_type &&) noexcept = delete;

      public:
        // accessor for the simulation execution context
        auto context() const -> const execution_context_type &;

        // accessor for the simulation clock time
        auto clock() const -> const clock_type &;

      private:
        // the simulation execution context
        execution_context_type _execution_context;

        // the simulation clock
        clock_type _clock;

        // friendship with the singleton
        using SimulationSingleton = utilities::Singleton<simulation_type>;
        friend SimulationSingleton;
    };

}    // namespace mito


// get the inline definitions
#define mito_simulation_Simulation_icc
#include "Simulation.icc"
#undef mito_simulation_Simulation_icc


#endif    // mito_simulation_Simulation_h

// end of file
