#include <iostream>

struct ScalarScalarBlock {
    std::string name = "ScalarScalarBlock";
};

struct ScalarBlock {
    std::string name = "ScalarBlock";
};

struct GradGradBlock {
    std::string name = "GradGradBlock";
};

struct HyperbolicModel {
    struct Stiffness {};
    struct Inertia {};
    struct Load {};
};

template <typename Tag, typename Block>
struct Term {
    Block _block;

    template <typename T>
    requires(std::is_same_v<T, Tag>)
    auto do_something()
    {
        std::cout << "I am a term that contains a " << _block.name << std::endl;
    }
};

template <typename... Terms>
class SemiDiscreteWeakform : public Terms... {
  public:
    using Terms::do_something...;

    SemiDiscreteWeakform(const Terms &... terms) : Terms(terms)... {}
};


int
main()
{
    using Stiffness = HyperbolicModel::Stiffness;
    using Inertia = HyperbolicModel::Inertia;
    using Load = HyperbolicModel::Load;

    auto semiDiscreteWeakForm = SemiDiscreteWeakform(
        Term<Stiffness, ScalarScalarBlock>{}, Term<Inertia, ScalarBlock>{},
        Term<Load, GradGradBlock>{});

    semiDiscreteWeakForm.template do_something<Stiffness>();
    semiDiscreteWeakForm.template do_something<Inertia>();
    semiDiscreteWeakForm.template do_something<Load>();
}