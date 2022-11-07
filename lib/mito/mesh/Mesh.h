// code guard
#if !defined(mito_mesh_Mesh_h)
#define mito_mesh_Mesh_h


namespace mito::mesh {

    // TOFIX: it would be great to rename this {elementT} in a way that would not imply any
    //          of the baggage of the "finite-element element"
    //          For example, a Mesh<D, simplex_t> should be thought as a mesh of simplicial
    //          topology, not of simplicial elements.
    //          (e.g. simplexT would be ok but it would be too specific to meshes of simplices,
    //          excluding quads, hex, ...)
    template <int D, template <int> class elementT>
    class Mesh {

      private:
        template <int I>
        using element_t = elementT<I>;

        // typedef for a collection of elements of dimension I
        template <size_t I>
        using element_collection = element_set_t<element_t<I>>;

        // element_collection<I>... expands to:
        // element_set_t<element_t<1>>, ..., element_set_t<element_t<D>>
        template <typename = std::make_index_sequence<D + 1>>
        struct element_tuple;

        template <size_t... I>
        struct element_tuple<std::index_sequence<I...>> {
            using type = std::tuple<element_collection<I>...>;
        };

        // this expands to:
        // tuple<element_set_t<element_t<0>>,
        //      element_set_t<element_t<1>>, ...,
        //      element_set_t<element_t<D>>
        using element_tuple_t = typename element_tuple<>::type;

      public:
        // default constructor
        inline Mesh() : _elements() {};

        inline ~Mesh() {}

        // move constructor
        inline Mesh(Mesh &&) = default;

      private:
        // delete copy constructor
        Mesh(const Mesh &) = delete;

        // delete assignment operator
        const Mesh & operator=(const Mesh &) = delete;

        // delete move assignment operator
        const Mesh & operator=(const Mesh &&) = delete;

      public:
        inline auto sanityCheck() -> bool
        {
#if 0
            // print summary
            std::cout << "Mesh composition: " << std::endl;
            std::cout << "0: " << std::get<0>(_elements).size() << " elements " << std::endl;
            std::cout << "1: " << std::get<1>(_elements).size() << " elements " << std::endl;
            std::cout << "2: " << std::get<2>(_elements).size() << " elements " << std::endl;
#endif

            // sanity check: each element is self-consistent
            for (const auto & element : std::get<D>(_elements)) {
                if (!element->sanityCheck()) {
                    return false;
                }
            }

            return true;
        }

        template <int I>
        inline auto nElements() const -> int
        requires(I <= D)
        {
            // all done
            return std::get<I>(_elements).size();
        }

        template <int I>
        inline auto elements() const -> const auto & requires(I <= D) {
                                                         // all done
                                                         return std::get<I>(_elements);
                                                     }

        /**
         * @brief Returns an element set with all boundary elements of dimension I
         */
        // QUESTION: I don't like the asymmetry of elements returning a const reference and boundary
        //  elements returning an instance. Either:
        //  1) say that these methods will make copies of the elements for the client to use, or
        //  2) say that boundary_elements will create a new data structure at run time and return a
        //      (const) reference for the client to use.
        // TOFIX: this method should return an iterator
        template <int I>
        inline auto boundary_elements() const -> auto
        requires(I<D && I> 0)
        {
            // instantiate an element collection
            element_collection<I> boundary_elements;

            // loop on the (I+1) dimensional elements
            for (const auto & element : std::get<I + 1>(_elements)) {
                for (const auto & subelement : element->composition()) {
                    // if the element footprint has only one occurrence then it is on the boundary
                    if (!exists_flipped(subelement)) {
                        // add this (D-1) dimensional element to the set of boundary elements
                        boundary_elements.insert(subelement);
                    }
                }
            }

            // return the boundary elements
            return boundary_elements;
        }

        template <int I>
        inline auto erase(const element_t<I> & element) -> void
        requires(I > 0 && I <= D)
        {
            // QUESTION: can we wrap elements in a way that the reference count can be called
            //  incidence?

            // erase the element from the mesh
            std::get<I>(_elements).erase(element);

            // TOFIX
            // cleanup oriented element factory around this element
            // mito::topology::Topology<I>::cleanup(element);

            // // TOFIX: synchronize with the geometry, check whether any point should be erased
            // // in the cloud of points
            // mito::geometry::PointCloud<D>::cleanup(element);

            // all done
            return;
        }

      public:
        // TOFIX: should be called addElement
        template <int I>
        inline auto addSimplex(const element_t<I> & element) -> void
        requires(I > 0 && I <= D)
        {
            // add the element to the set of elements with same dimension
            std::get<I>(_elements).insert(element);

            // all done
            return;
        }

      private:
        // container to store D+1 containers of d dimensional elements with d = 0, ..., D
        element_tuple_t _elements;
    };

}    // namespace mito


#endif    // mito_mesh_Mesh_h

// end of file
