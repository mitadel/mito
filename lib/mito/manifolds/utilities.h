// code guard
#if !defined(mito_manifold_utilities_h)
#define mito_manifold_utilities_h


namespace mito::manifolds {

    // type trait for the dimension of the input of function {F}
    template <class F>
    struct input {
        // input of function {F}
        using input_type = typename mito::math::function_t<F>::X;
        // output of function {F}
        using output_type = typename mito::math::function_t<F>::Y;
        // strip from {X} the cv-qualifiers and references, and get the size of the input
        static constexpr int input_dim = mito::size<mito::utilities::base_type<input_type>>();
        // strip from {X} the cv-qualifiers and references, and get the rank of the input
        static constexpr int input_rank = mito::rank<mito::utilities::base_type<input_type>>();
        // strip from {X} the cv-qualifiers and references, and get the size of the output
        static constexpr int output_dim = mito::size<mito::utilities::base_type<output_type>>();
        // strip from {Y} the cv-qualifiers and references, and get the rank of the output
        static constexpr int output_rank = mito::rank<mito::utilities::base_type<output_type>>();
    };


    // concept of a field {FIELD} being a scalar field on a D-dimensional manifold
    template <class FIELD>
    // {FIELD} is a scalar field if:
    concept is_scalar_field =
        // ... it takes in input a mito::geometry::coordinates_t<D>, D = FIELD::dim
        std::is_same_v<
            mito::utilities::base_type<typename input<typename FIELD::function_type>::input_type>,
            mito::geometry::coordinates_t<FIELD::dim>>
        // ... and returns a mito::scalar_t
        && std::is_same_v<
            mito::utilities::base_type<typename input<typename FIELD::function_type>::output_type>,
            mito::scalar_t>;


    // concept of a field {FIELD} being a vector field on a D-dimensional manifold
    template <class FIELD>
    // {FIELD} is a vector field if:
    concept is_vector_field =
        // ... it takes in input a mito::geometry::coordinates_t<D>, D = FIELD::dim
        std::is_same_v<
            mito::utilities::base_type<typename input<typename FIELD::function_type>::input_type>,
            mito::geometry::coordinates_t<FIELD::dim>>
        // ... and returns a mito::vector_t<N>, N = FIELD::output_dim
        && std::is_same_v<
            mito::utilities::base_type<typename input<typename FIELD::function_type>::output_type>,
            mito::vector_t<input<typename FIELD::function_type>::output_dim>>;


    // concept of a field {FIELD} being a tensor field on a D-dimensional manifold
    template <class FIELD>
    // {FIELD} is a tensor field on a D-dimensional manifold if:
    concept is_tensor_field =
        // ... it takes in input a mito::geometry::coordinates_t<D>, D = FIELD::dim
        std::is_same_v<
            mito::utilities::base_type<typename input<typename FIELD::function_type>::input_type>,
            mito::geometry::coordinates_t<FIELD::dim>>
        // ... and returns a matrix
        && input<typename FIELD::function_type>::output_rank == 2;


    // concept of a field {FIELD} being a symmetric tensor field on a D-dimensional manifold
    template <class FIELD>
    // {FIELD} is a symmetric tensor field on a D-dimensional manifold if:
    concept is_symmetric_tensor_field =
        // ... it takes in input a mito::geometry::coordinates_t<D>, D = FIELD::dim
        std::is_same_v<
            mito::utilities::base_type<typename input<typename FIELD::function_type>::input_type>,
            mito::geometry::coordinates_t<FIELD::dim>>
        // ... and returns a matrix...
        && input<typename FIELD::function_type>::output_rank == 2
        // ... that has symmetric packing
        && input<typename FIELD::function_type>::output_type::symmetric;


    // concept of a field {FIELD} being a diagonal tensor field on a D-dimensional manifold
    template <class FIELD>
    // {FIELD} is a diagonal tensor field on a D-dimensional manifold if:
    concept is_diagonal_tensor_field =
        // ... it takes in input a mito::geometry::coordinates_t<D>, D = FIELD::dim
        std::is_same_v<
            mito::utilities::base_type<typename input<typename FIELD::function_type>::input_type>,
            mito::geometry::coordinates_t<FIELD::dim>>
        // ... and returns a matrix...
        && input<typename FIELD::function_type>::output_rank == 2
        // ... that has diagonal packing
        && input<typename FIELD::function_type>::output_type::diagonal;

}


#endif

// end of file
