
namespace mito::manifolds {

    template <class elementT>
    constexpr auto parametric_dim() -> int;

    template <>
    constexpr auto parametric_dim<segment_t>() -> int
    {
        return 2;
    }

    template <>
    constexpr auto parametric_dim<triangle_t>() -> int
    {
        return 3;
    }

    template <>
    constexpr auto parametric_dim<tetrahedron_t>() -> int
    {
        return 4;
    }
}