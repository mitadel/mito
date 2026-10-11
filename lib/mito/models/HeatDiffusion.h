// code guard
#pragma once


namespace mito::models {

    template <class materialT>
    class HeatDiffusion {

      private:
        // the material type
        using material_type = materialT;

      public:
        // the archetype of this model
        using archetype_type = ParabolicPDE;

        // the constructor
        constexpr HeatDiffusion(const material_type & material) : material(material) {};

        // get the block associated to a term of the archetype
        template <std::same_as<archetype_type::Stiffness> Tag>
        auto getBlock() const
        {
            return grad_grad_block(material.conductivity());
        }

        template <std::same_as<archetype_type::Capacitance> Tag>
        auto getBlock() const
        {
            return value_value_block(material.heat_capacity());
        }

        template <std::same_as<archetype_type::Load> Tag>
        auto getBlock() const
        {
            return value_block(material.heat_source());
        }

      private:
        // the material
        const material_type & material;
    };
}


// end of file
