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

    using Terms = std::tuple<Stiffness, Inertia, Load>;
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

    SemiDiscreteWeakform(const Terms&... terms) : Terms(terms)... {}
};


class MyCustomPhysicalModel {
  public:
    using Archetype = HyperbolicModel;

    template <typename Tag>
    auto getBlock() const
    {
        if constexpr (std::is_same_v<Tag, HyperbolicModel::Stiffness>) {
            return GradGradBlock{};
        } else if constexpr (std::is_same_v<Tag, HyperbolicModel::Inertia>) {
            return ScalarScalarBlock{};
        } else if constexpr (std::is_same_v<Tag, HyperbolicModel::Load>) {
            return ScalarBlock{};
        }
    }
};


template <typename Tag, typename Block>
auto
make_term(Block block)
{
    return Term<Tag, Block>{ block };
}


template <typename ModelType>
auto
make_SemiDiscreteWeakformTuple(const ModelType & model)
{
    using Archetype = typename ModelType::Archetype;

    return std::apply(
        [&]<typename... Tags>(Tags...) {
            return std::make_tuple(make_term<Tags>(model.template getBlock<Tags>())...);
        },
        typename Archetype::Terms{});
}


template <typename PhysicalModel>
auto
make_SemiDiscreteWeakform(const PhysicalModel & physical_model)
{
    return std::apply(
        [](const auto &... terms) { return SemiDiscreteWeakform(terms...); },
        make_SemiDiscreteWeakformTuple(physical_model));
}


int
main()
{
    using Stiffness = HyperbolicModel::Stiffness;
    using Inertia = HyperbolicModel::Inertia;
    using Load = HyperbolicModel::Load;

    MyCustomPhysicalModel physical_model;

    auto semiDiscreteWeakForm = make_SemiDiscreteWeakform(physical_model);

    semiDiscreteWeakForm.template do_something<Stiffness>();
    semiDiscreteWeakForm.template do_something<Inertia>();
    semiDiscreteWeakForm.template do_something<Load>();
}