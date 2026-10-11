// code guard
#pragma once


namespace mito::models {

    struct ParabolicPDE {
        // the stiffness tag
        struct Stiffness {};
        // the capacitance tag
        struct Capacitance {};
        // the load tag
        struct Load {};

        // the tuple of all terms
        using Terms = std::tuple<Stiffness, Capacitance, Load>;
    };
}


// end of file
