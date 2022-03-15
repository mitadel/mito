// code guard
#if !defined(mito_mesh_OrientedSimplex_h)
#define mito_mesh_OrientedSimplex_h

/*
 * This class represents a class of equivalence for an OrientedSimplex of dimension D.
 *
 * Each instance of OrientedSimplex consists of an underlying (nonoriented) Simplex and an
 * orientation, which can be positive or negative.
 * OrientedSimplex<D> is represented recursively as a collection of D+1 OrientedSimplex<D-1>. An
 * instance of Simplex<D> represents therefore (D+1)!/2 simplices, depending on the (D+1)!/2
 * possible orderings of the subsimplices with positive (negative) sign.
 *
 * When the orientation is positive, the representative in this class of equivalence is chosen by
 * taking the first subsimplex of the underlying (nonoriented) Simplex with its natural orientation.
 * When the orientation is negative, the representative in this class of equivalence is chosen by
 * taking the first subsimplex of the underlying (nonoriented) Simplex against its natural 
 * orientation.
 */

namespace mito::mesh {

    template <int D>
    class OrientedSimplex {
      public:
        OrientedSimplex(const std::shared_ptr<simplex_t<D>> footprint, bool orientation) :
            _footprint(footprint), _orientation(orientation)
        {}

        ~OrientedSimplex() {}

      private:
        // delete default constructor
        OrientedSimplex() = delete;

        // delete copy constructor
        OrientedSimplex(const OrientedSimplex &) = delete;

        // delete move constructor
        OrientedSimplex(const OrientedSimplex &&) = delete;

        // delete assignment operator
        const OrientedSimplex & operator=(const OrientedSimplex &) = delete;

        // delete move assignment operator
        const OrientedSimplex & operator=(const OrientedSimplex &&) = delete;
      
      public:
        const auto footprint() const {return _footprint;}
        bool orientation() const { return _orientation; }
        const auto & simplices() const { return _footprint.get()->simplices(); }
        void vertices(std::set<const vertex_t *> & vertices) const {
            return _footprint.get()->vertices(vertices);
        }

      public:
        bool sanityCheck() const
        {
            // check the subsimplices
            for (const auto & simplex : simplices()) {
                // if a subsimplex is broken, the sanity check fails
                if (!simplex->sanityCheck()) {
                    // all done
                    return false;
                }
            }

            // use a set to cleanup duplicates
            std::set<const vertex_t *> vertices;
            // collect vertices of every subsimplex of this simplex
            for (const auto & simplex : simplices()) {
                simplex->vertices(vertices);
            }
            // if this simplex does not have D+1 vertices, something went wrong
            if (vertices.size() != int(D) + 1) {
                // all done
                return false;
            }

            // all done
            return true;
        }

      public:
        static constexpr int parametricDim = D + 1;
        static constexpr int nVertices = D;

      private:
        const std::shared_ptr<simplex_t<D>> _footprint;
        bool _orientation;
    };

    template <>
    class OrientedSimplex<0> : public Simplex<0> {
      public:
        OrientedSimplex() : Simplex<0>() {}
        
        virtual ~OrientedSimplex() {}

      private:
        // delete copy constructor
        OrientedSimplex(const OrientedSimplex &) = delete;

        // delete move constructor
        OrientedSimplex(const OrientedSimplex &&) = delete;

        // delete assignment operator
        const OrientedSimplex & operator=(const OrientedSimplex &) = delete;

        // delete move assignment operator
        const OrientedSimplex & operator=(const OrientedSimplex &&) = delete;

      public:
        bool sanityCheck() const { return true; }
    };
}
#endif    // mito_mesh_OrientedSimplex_h

// end of file
