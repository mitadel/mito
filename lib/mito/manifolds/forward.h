// code guard
#if !defined(mito_manifolds_forward_h)
#define mito_manifolds_forward_h


namespace mito::manifolds {

    // class manifold
    template <geometry::CoordinateSystem coordsT, class cellT, int D>
    class Manifold;

    // class P-form
    template <int P, class F>
    class Form;

    // class field
    template <class F>
    class Field;
}


#endif

// end of file
