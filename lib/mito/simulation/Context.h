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
        ~Context() {}

      private:
        // delete move constructor
        Context(context_type &&) = delete;

        // delete copy constructor
        Context(const context_type &) = delete;

        // delete assignment operator
        Context & operator=(const context_type &) = delete;

        // delete move assignment operator
        Context & operator=(context_type &&) = delete;

      private:
        // attributes...
    };

}    // namespace mito


// get the inline definitions
#define mito_simulation_Context_icc
#include "Context.icc"
#undef mito_simulation_Context_icc


#endif    // mito_simulation_Context_h

// end of file
