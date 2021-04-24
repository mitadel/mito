#ifndef __MITO_SIMPLEX__
#define __MITO_SIMPLEX__

#include "../mito.h"
#include <set>
#include <unordered_map>

namespace mito {

    template <dim_t D>
    class Simplex {
      public:
        Simplex(std::array<Simplex<D - 1> *, D + 1> && entities) : _entities(entities)
        {
            // sort the entities (using the address of the entities) so that two simplices
            // having the same entities will result in two identical instances of class Simplex
            std::sort(_entities.begin(), _entities.end());
        }

        ~Simplex() {}

        // delete default constructor
        Simplex() = delete;

        // delete copy constructor
        Simplex(const Simplex &) = delete;

        // delete assignment operator
        Simplex & operator=(const Simplex &) = delete;

        const auto & entities() const { return _entities; }

        void getVertices(std::set<const Simplex<DIM0> * /* vertex_t* */> & vertices) const
        {
            for (const auto & entity : entities()) {
                entity->getVertices(vertices);
            }
        }

        bool sanityCheck() const
        {
            // check the subentities
            for (const auto & entity : entities()) {
                // if a subentity is broken, the sanity check fails
                if (!entity->sanityCheck()) {
                    // all done
                    return false;
                }
            }

            // use a set to cleanup duplicates
            std::set<const Simplex<DIM0> * /* vertex_t* */> vertices;
            // collect vertices of every subentity of this simplex
            for (const auto & entity : entities()) {
                entity->getVertices(vertices);
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
        static constexpr dim_t parametricDim = D + 1;
        static constexpr int nVertices = D;

      private:
        std::array<Simplex<D - 1> *, D + 1> _entities;
    };

    template <>
    class Simplex<DIM0> {
      public:
        Simplex() {}
        ~Simplex() {}

        // delete copy constructor
        Simplex(const Simplex &) = delete;

        // delete assignment operator
        Simplex & operator=(const Simplex &) = delete;

        void getVertices(std::set<const Simplex<DIM0> * /* vertex_t* */> & vertices) const
        {
            // insert this vertex
            vertices.insert(this);
            // all done
            return;
        }

        bool sanityCheck() const { return true; }
    };

    template <dim_t D>
    class OrientedSimplex : public Simplex<D> {
      public:
        OrientedSimplex(std::array<Simplex<D - 1> *, D + 1> && entities, bool orientation) :
            Simplex<D>(entities),
            _orientation(orientation)
        {}

        ~OrientedSimplex() {}

        // delete default constructor
        OrientedSimplex() = delete;

        // delete copy constructor
        OrientedSimplex(const OrientedSimplex &) = delete;

        // delete assignment operator
        OrientedSimplex & operator=(const OrientedSimplex &) = delete;

      private:
        bool _orientation;
    };

    using vertex_t = Simplex<DIM0>;
    using segment_t = Simplex<DIM1>;
    using triangle_t = Simplex<DIM2>;
    using tetrahedron_t = Simplex<DIM3>;

}    // namespace mito

// overload operator<< for simplices
template <mito::dim_t D>
std::ostream &
operator<<(std::ostream & os, const mito::Simplex<D> & s)
{
    os << &s << " composed of:" << std::endl;
    for (const auto & entity : s.entities()) {
        std::cout << "\t" << *entity << std::endl;
    }
    return os;
}

// overload operator<< specialization for simplices with D = 0 (vertices)
template <>
std::ostream &
operator<<(std::ostream & os, const mito::Simplex<mito::DIM0> & s)
{
    os << &s;
    return os;
}

#endif    //__MITO_SIMPLEX__
