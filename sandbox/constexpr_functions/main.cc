#include <type_traits>
#include <memory>

template <typename T>
struct lambda_traits : public lambda_traits<decltype(&T::operator())> {};

template <typename ClassT, typename ReturnT, typename ArgumentT>
struct lambda_traits<ReturnT (ClassT::*)(ArgumentT) const> {
    typedef ReturnT result_type;
    typedef ArgumentT argument_type;
};

template <class X, class Y>
class Function {
  public:
    template <class F>
    constexpr Function(F f) : ptr { std::make_unique<implementation<F>>(f) }
    {}

    constexpr ~Function() {}

    constexpr auto operator()(X x) const -> Y { return ptr->get(x); }

  private:
    struct interface {
        constexpr virtual auto get(X) -> Y = 0;
        constexpr virtual ~interface() = default;
    };

    template <class F>
    struct implementation final : interface {
        constexpr explicit(true) implementation(F f) : _f { f } {}
        constexpr auto get(X x) -> Y { return _f(x); }
        constexpr ~implementation() {};

      private:
        F _f;
    };

    std::unique_ptr<interface> ptr;
};

template <class F>
constexpr auto
function(F f)
{
    return Function<
        typename lambda_traits<F>::argument_type, typename lambda_traits<F>::result_type>(f);
}


consteval auto
test_capture()
{
    auto f = function([&](int i) { return 2 * i; });
    return f(2);
}

int
main()
{
    // auto f = function([&](int i) { return 2 * i; });
    // static_assert(4 == f(2));

    static_assert(4 == test_capture());
    return 0;
}
