#include <iostream>
#include <typeinfo>

// values for template parameter packing
template <int N>
class Canonical {};

template <int N>
class Symmetric {};

template <int N>
class Diagonal {};

template <class T, class S>
struct repacking;

template <int N>
struct repacking<Canonical<N>, Canonical<N>> {
    using packing_type = Canonical<N>;
};

template <int N>
struct repacking<Canonical<N>, Symmetric<N>> {
    using packing_type = Canonical<N>;
};

template <int N>
struct repacking<Symmetric<N>, Canonical<N>> {
    using packing_type = Canonical<N>;
};

template <int N>
struct repacking<Symmetric<N>, Symmetric<N>> {
    using packing_type = Symmetric<N>;
};

template <int N>
struct repacking<Diagonal<N>, Canonical<N>> {
    using packing_type = Canonical<N>;
};

template <int N>
struct repacking<Canonical<N>, Diagonal<N>> {
    using packing_type = Canonical<N>;
};

template <int N>
struct repacking<Diagonal<N>, Diagonal<N>> {
    using packing_type = Diagonal<N>;
};

template <int N>
struct repacking<Diagonal<N>, Symmetric<N>> {
    using packing_type = Symmetric<N>;
};

template <int N>
struct repacking<Symmetric<N>, Diagonal<N>> {
    using packing_type = Symmetric<N>;
};

int
main()
{

    repacking<Symmetric<1>, Symmetric<1>>::packing_type a;

    std::cout << typeid(a).name() << std::endl;

    return 0;
}