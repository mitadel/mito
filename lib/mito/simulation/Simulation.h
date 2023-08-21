// code guard
#if !defined(mito_simulation_Simulation_h)
#define mito_simulation_Simulation_h


namespace mito::simulation {

    class Simulation {

      public:
        // me
        using simulation_type = Simulation;
        // the clock type
        using clock_type = scalar_t;

      public:
        // default constructor
        Simulation();

        // destructor
        ~Simulation() {}

      private:
        // delete move constructor
        Simulation(simulation_type &&) = delete;

        // delete copy constructor
        Simulation(const simulation_type &) = delete;

        // delete assignment operator
        Simulation & operator=(const simulation_type &) = delete;

        // delete move assignment operator
        Simulation & operator=(simulation_type &&) = delete;

      private:
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
