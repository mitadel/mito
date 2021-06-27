#include <valarray>

namespace mito {

    template <typename... Args>
    constexpr auto multiply(Args &&... args)
    {
        return (args * ...);
    }

    template <typename T, int... I>
    class SmallGrid {

      public:
        // compute the number of indices of the
        static constexpr int N = sizeof...(I);
        // compute the number of cells (size of the container)
        static constexpr int S = multiply(I...);
        // store the underlying type
        using type = T;

      public:
        // default constructor
        inline SmallGrid() : _data(S) {}

        // constructor from brace-enclosed initializer list
        template <class... T2, typename std::enable_if<sizeof...(T2) == S, int>::type = 0>
        SmallGrid(T2... args) : SmallGrid()
        {
            // initialize with input arguments
            _initialize(std::make_index_sequence<S> {}, args...);

            // all done
            return;
        }

        // copy constructor
        SmallGrid(const SmallGrid &) = default;

        // move constructor
        SmallGrid(SmallGrid &&) = default;

        // copy assignment operator
        SmallGrid & operator=(const SmallGrid &) = default;

        // move assignment operator
        SmallGrid & operator=(SmallGrid &&) = default;

        // destructor
        inline ~SmallGrid() {}

      public:
        // inline const T & operator[](index_t i) const { return _grid[i]; }
        // inline T & operator[](index_t i) { return _grid[i]; }

        inline const T & operator[](int i) const { return _data[i]; }
        inline T & operator[](int i) { return _data[i]; }

        inline void operator+=(const SmallGrid<T, I...> & rhs)
        {
            // component-wise operator+=
            _operatorPlusEqual(std::make_index_sequence<S> {}, rhs);

            // all done
            return;
        }

        // enable cast to underlying type if S = 1 (scalar grid)
        operator T() const requires(S == 1) { return _data[0]; }

        // reset to zero
        inline void reset()
        {
            // reset to zero all entries
            _reset(std::make_index_sequence<S> {});

            // all done
            return;
        }

      private:
        template <size_t... J, class... T2>
        void _initialize(std::index_sequence<J...>, T2... args)
        {
            ((_data[J] = args), ...);
        }

        template <size_t... J>
        void _reset(std::index_sequence<J...>)
        {
            ((_data[J] = T()), ...);
        }

        template <size_t... J>
        void _operatorPlusEqual(std::index_sequence<J...>, const SmallGrid<T, I...> & rhs)
        {
            ((_data[J] += rhs[J]), ...);
        }

      public:
        constexpr auto size() { return S; }

      private:
        // data
        std::valarray<T> _data;
    };

}    // namespace mito

// end of file
