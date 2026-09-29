// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::math_backend {

    // concept of {T} having the properties of a matrix-like object
    template <class matrixT, class indexT, class scalarT>
    concept matrix_like_c =
        requires(matrixT m, const matrixT& cm, indexT i, indexT j, scalarT s, scalarT & s_ref) {
            { cm.label() } -> std::convertible_to<std::string>;
            { m.assemble() } -> std::same_as<void>;
            { m.assembleFlush() } -> std::same_as<void>;
            { m.insert_value(i, j, s) } -> std::same_as<void>;
            { m.add_value(i, j, s) } -> std::same_as<void>;
            { cm.get_value(i, j, s_ref) } -> std::same_as<void>;
            { cm.get_value(i, j) } -> std::same_as<scalarT>;
            { cm.n_equations() } -> std::convertible_to<int>;
            { cm.print() } -> std::same_as<void>;
        };

    // concept of {T} having the properties of a vector-like object
    template <class vectorT, class indexT, class scalarT>
    concept vector_like_c =
        requires(vectorT v, const vectorT& cv, indexT i, scalarT s, scalarT & s_ref) {
            { cv.label() } -> std::convertible_to<std::string>;
            { v.assemble() } -> std::same_as<void>;
            { v.insert_value(i, s) } -> std::same_as<void>;
            { v.add_value(i, s) } -> std::same_as<void>;
            { cv.n_equations() } -> std::convertible_to<int>;
            { cv.print() } -> std::same_as<void>;
        };

    template <class backendT>
    concept valid_backend_c =
        requires {
            typename backendT::index_type;
            typename backendT::scalar_type;
            typename backendT::matrix_type;
            typename backendT::vector_type;
        }
        && matrix_like_c<
            typename backendT::matrix_type, typename backendT::index_type,
            typename backendT::scalar_type>
        && vector_like_c<
            typename backendT::vector_type, typename backendT::index_type,
            typename backendT::scalar_type>;
}


// end of file