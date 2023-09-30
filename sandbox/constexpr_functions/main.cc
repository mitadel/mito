#include <type_traits>

template <typename T>
struct lambda_traits : public lambda_traits<decltype(&T::operator())> {};

template <typename ClassT, typename ReturnT, typename ArgumentT>
struct lambda_traits<ReturnT (ClassT::*)(ArgumentT) const> {
    typedef ReturnT result_type;
    typedef ArgumentT argument_type;
};

// helper typedef to remove the reference to F&&
template <class F>
using remove_reference_lambda = typename std::remove_reference_t<F>;

template <class F>
class Function {

    using traits = lambda_traits<remove_reference_lambda<F>>;
    using X = typename std::remove_reference_t<typename traits::argument_type>;
    using Y = typename std::remove_reference_t<typename traits::result_type>;

  public:
    constexpr Function(F f) : _f { f } {}
    constexpr auto operator()(X x) const -> Y { return _f(x); }

    F _f;
};

// make function from lambda function
template <class F>
constexpr auto
function(F && f)
{
    return Function<F>(f);
}

int
main()
{
    constexpr auto f = function([](double i) -> int { return 2 * i; });
    static_assert(f(1.0) == 2);
}