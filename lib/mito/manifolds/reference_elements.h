
namespace mito::manifolds {

    template <class elementT>
    constexpr int parametric_dim();

    template <>
    constexpr int parametric_dim<segment_t>()
    {
        return 2;
    }

    template <>
    constexpr int parametric_dim<triangle_t>()
    {
        return 3;
    }

    template <>
    constexpr int parametric_dim<tetrahedron_t>()
    {
        return 4;
    }
}