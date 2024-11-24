#include <array>
#include <utility>

// https://stackoverflow.com/questions/60434033/how-do-i-expand-a-compile-time-stdarray-into-a-parameter-pack

// template class with parameter pack
template <typename T, class packingT, int... I>
class Tensor {
public:
    static constexpr int N = sizeof...(I);
    // dimensions (the maximum index value for each index)
    static constexpr std::array<int, N> dims { I... };
    static constexpr int size = dims.size();
};

template <
    typename T, class packingT, auto arr,
    typename IS = decltype(std::make_index_sequence<arr.size()>())>
struct generator;

template <typename T, class packingT, auto arr, std::size_t... I>
struct generator<T, packingT, arr, std::index_sequence<I...>> {
    using type = Tensor<T, packingT, arr[I]...>;
};

class bogus {};

int main() {
    constexpr std::array<int, 3> dims { 1, 2, 3 };

    constexpr generator<double, bogus, dims>::type tensor;

    static_assert(tensor.size == 3);

    return 0;
}