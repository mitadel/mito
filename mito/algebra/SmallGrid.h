// code guard
#if !defined(mito_algebra_SmallGrid_h)
#define mito_algebra_SmallGrid_h

#include <pyre/grid.h>

namespace mito {

    template <typename... Args>
    constexpr auto multiply(Args &&... args)
    {
        int one = 1;
        return one * (args * ...);    // should return 1 for empty pack (scalar case)
    }

    template <typename T, int... I>
    class SmallGrid {

      public:    // TODO: make these private
        // compute the number of indices of the
        static constexpr int N = sizeof...(I);    // zero if empty parameter pack
        // compute the number of cells (size of the container)
        static constexpr int S = multiply(I...);    // one if empty parameter pack

      private:
        // conventionally packed grid
        using pack_t = pyre::grid::canonical_t<N>;
        // of T on the heap
        using storage_t = pyre::memory::stack_t<S, T>;
        // putting it all together
        using grid_t = pyre::grid::grid_t<pack_t, storage_t>;
        // index
        using index_t = pack_t::index_type;
        // data_type
        using data_t = grid_t;

      public:
        // store the underlying type
        using type = T;
        // store the container size TOFIX
        // static constexpr int size = S;

      public:
        // default constructor
        inline SmallGrid() : _data(pack_t { { I... } })
        {
            // initialize memory
            reset();

            // all done
            return;
        }

      public:
        // constructor with underlying data type
        inline SmallGrid(const data_t & data) : _data(data) {}

        // constructor with underlying data type (need this for return value optimization)
        inline SmallGrid(const data_t && data) : _data(std::forward(data)) {}

        // constructor from brace-enclosed initializer list
        template <class... T2>
        SmallGrid(T2... args) requires(sizeof...(T2) == S) : SmallGrid()
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
        SmallGrid & operator=(SmallGrid && rhs) = default;

        // destructor
        inline ~SmallGrid()
        {
            std::cout << "~SmallGrid()" << std::endl;
            return;
        }

      public:
        inline const T & operator[](index_t i) const { return _data[i]; }
        inline T & operator[](index_t i) { return _data[i]; }

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

        // enable cast to underlying data type
        operator data_t() const { return _data; }

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
        data_t _data;
    };

}    // namespace mito

#endif    // mito_algebra_SmallGrid_h

// end of file
