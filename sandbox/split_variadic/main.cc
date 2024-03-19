#include <iostream>
#include <tuple>


template <int...>
struct pack_indices {};

template <int Sp, typename IntPack, int Ep>
struct make_indices_imp;

template <int Sp, int Ep, int... Indices>
struct make_indices_imp<Sp, pack_indices<Indices...>, Ep> {
    typedef typename make_indices_imp<Sp + 1, pack_indices<Indices..., Sp>, Ep>::type type;
};

template <int Ep, int... Indices>
struct make_indices_imp<Ep, pack_indices<Indices...>, Ep> {
    typedef pack_indices<Indices...> type;
};

template <int Ep, int Sp = 0>
struct make_pack_indices {
    static_assert(Sp <= Ep, "make_tuple_indices input error");
    typedef typename make_indices_imp<Sp, pack_indices<>, Ep>::type type;
};

template <typename Tuple, int... index>
auto
extract(pack_indices<index...>, Tuple && args)
{
    return std::forward_as_tuple(std::get<index>(args)...);
}

template <int begin, int end, typename... Args>
auto
extract(Args &&... args)
{
    typename make_pack_indices<end, begin>::type indices;
    return extract(indices, std::forward_as_tuple(std::forward<Args>(args)...));
}

template <typename... Args>
void
print(Args &&... args)
{
    ((std::cout << args << " "), ...);
    std::cout << std::endl;
}

template <int... Is>
void
print_tuple(const auto & tuple, std::integer_sequence<int, Is...>)
{
    return print(std::get<Is>(tuple)...);
}

void
print_tuple(const auto & tuple)
{
    constexpr int size = std::tuple_size_v<std::decay_t<decltype(tuple)>>;
    return print_tuple(tuple, std::make_integer_sequence<int, size>());
}

template <typename... Args>
void
print_split(Args &&... args)
{
    print_tuple(extract<0, 1>(std::forward<Args>(args)...));
    print_tuple(extract<1, 3>(std::forward<Args>(args)...));
}

int
main()
{
    print_split(1, 2, 3);
}