// code guard
#if !defined(mito_mesh_Simplex_h)
#define mito_mesh_Simplex_h

#include "../mito.h"
#include <set>
#include <unordered_map>

namespace mito {

    template <int D>
    class Simplex {

      public:
        Simplex(const std::array<Simplex<D - 1> *, D + 1> & simplices) : _simplices(simplices)
        {
            // initialize object
            _initialize();

            // all done
            return;
        }

        Simplex(std::array<Simplex<D - 1> *, D + 1> && simplices) : _simplices(simplices)
        {
            // initialize object
            _initialize();

            // all done
            return;
        }

        ~Simplex() {}

      private:
        // delete default constructor
        Simplex() = delete;

        // delete copy constructor
        Simplex(const Simplex &) = delete;

        // delete move constructor
        Simplex(const Simplex &&) = delete;

        // delete assignment operator
        const Simplex & operator=(const Simplex &) = delete;

        // delete move assignment operator
        const Simplex & operator=(const Simplex &&) = delete;

      private:
        void _initialize()
        {
            // sort the simplices (using the address of the simplices) so that two simplices
            // having the same simplices will result in two identical instances of class Simplex
            std::sort(_simplices.begin(), _simplices.end());

            // all done
            return;
        }

      public:
        const auto & simplices() const { return _simplices; }

        void getVertices(std::set<const Simplex<0> * /* vertex_t* */> & vertices) const
        {
            for (const auto & simplex : simplices()) {
                simplex->getVertices(vertices);
            }
        }

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
            std::set<const Simplex<0> * /* vertex_t* */> vertices;
            // collect vertices of every subsimplex of this simplex
            for (const auto & simplex : simplices()) {
                simplex->getVertices(vertices);
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
        std::array<Simplex<D - 1> *, D + 1> _simplices;
    };

    template <>
    class Simplex<0> {
      public:
        Simplex() {}
        ~Simplex() {}

      private:
        // delete copy constructor
        Simplex(const Simplex &) = delete;

        // delete move constructor
        Simplex(const Simplex &&) = delete;

        // delete assignment operator
        const Simplex & operator=(const Simplex &) = delete;

        // delete move assignment operator
        const Simplex & operator=(const Simplex &&) = delete;

      public:
        void getVertices(std::set<const Simplex<0> * /* vertex_t* */> & vertices) const
        {
            // insert this vertex
            vertices.insert(this);
            // all done
            return;
        }

        bool sanityCheck() const { return true; }
    };

    template <int D>
    class OrientedSimplex : public Simplex<D> {
      public:
        OrientedSimplex(std::array<Simplex<D - 1> *, D + 1> && simplices, bool orientation) :
            Simplex<D>(simplices),
            _orientation(orientation)
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

      private:
        bool _orientation;
    };

    using vertex_t = Simplex<0>;
    using segment_t = Simplex<1>;
    using triangle_t = Simplex<2>;
    using tetrahedron_t = Simplex<3>;

}    // namespace mito

// overload operator<< for simplices
template <int D>
std::ostream &
operator<<(std::ostream & os, const mito::Simplex<D> & s)
{
    os << &s << " composed of:" << std::endl;
    for (const auto & simplex : s.simplices()) {
        std::cout << "\t" << *simplex << std::endl;
    }
    return os;
}

// overload operator<< specialization for simplices with D = 0 (vertices)
template <>
std::ostream &
operator<<(std::ostream & os, const mito::Simplex<0> & s)
{
    os << &s;
    return os;
}

#endif    // mito_mesh_Simplex_h

// end of file
