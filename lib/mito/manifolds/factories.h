// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_manifolds_factories_h)
#define mito_manifolds_factories_h


namespace mito::manifolds {

    // factory manifold
    template <class cellT, geometry::coordinates_c coordsT, class volumeFormT>
    constexpr auto manifold(
        const mesh::mesh_t<cellT> & mesh,
        const geometry::coordinate_system_t<coordsT> & coordinate_system,
        const volumeFormT & volume_form)
    {
        return manifold_t<cellT, coordsT, volumeFormT>(mesh, coordinate_system, volume_form);
    }

    // factory of manifolds from a mesh and a coordinate system
    template <class cellT, geometry::coordinates_c coordsT>
    constexpr auto manifold(
        const mesh::mesh_t<cellT> & mesh,
        const geometry::coordinate_system_t<coordsT> & coordinate_system)
    {
        // the mesh type
        using mesh_type = mesh::mesh_t<cellT>;

        // assert that the manifold is of the highest dimension
        static_assert(mesh_type::dim == mesh_type::order);

        // get the metric space
        constexpr auto space = geometry::metric_space<coordsT>();

        return manifold(mesh, coordinate_system, space.w());
    }

    // factory of submanifolds from a mesh, a coordinate system and set of normal fields
    template <class cellT, geometry::coordinates_c coordsT, mito::fields::vector_field_c... fieldsT>
    constexpr auto submanifold(
        const mesh::mesh_t<cellT> & mesh,
        const geometry::coordinate_system_t<coordsT> & coordinate_system,
        const fieldsT &... normal_field)
    {
        // the mesh type
        using mesh_type = mesh::mesh_t<cellT>;

        // assert that the number of normal fields provided corresponds to the dimension of the
        // physical space minus the order of the manifold
        static_assert(mesh_type::dim - mesh_type::order == sizeof...(fieldsT));

        // get the metric space
        constexpr auto space = geometry::metric_space<coordsT>();

        // get the metric volume form
        constexpr auto w = space.w();

        // strip namespace from the placeholder for forms contractions
        using mito::tensor::_;

        // assert that the submanifold is either a line or a surface
        // the general case would require the expansion of the {_} for {mesh_type::order} times in
        // the contraction of the metric volume form
        static_assert(mesh_type::order == 1 || mesh_type::order == 2);

        // if the submanifold is a line (either in 2D or 3D) then use one placeholder {_}
        if constexpr (mesh_type::order == 1) {
            // the restriction of the metric volume form to the submanifold
            auto wS = fields::field(
                [w, normal_field...](const coordsT & x) { return w(x)(normal_field(x)..., _); });

            // return the manifold
            return manifold(mesh, coordinate_system, wS);
        }

        // if the submanifold is a surface (in 3D) then use two placeholders {_, _}
        if constexpr (mesh_type::order == 2) {
            // the restriction of the metric volume element to the submanifold
            auto wS = fields::field(
                [w, normal_field...](const coordsT & x) { return w(x)(normal_field(x)..., _, _); });

            // return the manifold
            return manifold(mesh, coordinate_system, wS);
        }
    }
}


#endif

// end of file
