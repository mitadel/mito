// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


#include <gtest/gtest.h>
#include "utilities/petsc_assembly.h"

#include <utility>


using namespace mito::tests::linalg::petsc;

// moves of a named matrix are real moves, not copies in disguise
static_assert(std::is_nothrow_move_constructible_v<mito::linalg::petsc::matrix_t>);
static_assert(std::is_nothrow_move_assignable_v<mito::linalg::petsc::matrix_t>);

// a matrix of format {format} filled through set_pattern and set_values with the elements
// {local}, scaled by {factor}
auto
assembled_matrix(
    const char * name, const std::vector<element_t> & local, index_t N, format_t format,
    scalar_t factor) -> mito::linalg::petsc::matrix_t
{
    // flatten the elements into COO lists and block segments
    auto lists = flatten(local, factor);

    // create the matrix, set its pattern and values, and assemble it
    auto A = mito::linalg::petsc::matrix_t(name, N, format);
    A.set_pattern(lists.rows, lists.cols, lists.segments);
    A.set_values(lists.vals);
    A.assemble();

    // all done
    return A;
}

// the reference: the elements {local}, scaled by {factor}, assembled entry by entry
auto
reference(const std::vector<element_t> & local, index_t N, scalar_t factor)
    -> mito::linalg::petsc::matrix_t
{
    // create the reference matrix and fill it
    auto R = mito::linalg::petsc::matrix_t("R", N);
    fill_reference(R, local, factor);

    // all done
    return R;
}


// a copy holds the values and the assembly data of its source, and is independent of it
auto
check_copy_constructor(format_t format) -> void
{
    // the elements owned by this rank, and the number of equations
    const auto local = local_elements(test_elements());
    const index_t N = n_equations(local);

    // K with its pattern and values
    auto K = assembled_matrix("K", local, N, format, 1.0);

    // copy-construct {C} from {K}
    auto C = K;

    // check names
    EXPECT_EQ(K.name(), "K");
    EXPECT_EQ(C.name(), "K");

    // the copy owns a different PETSc matrix
    EXPECT_NE(C.get(), K.get());

    // the copy holds the values of K
    expect_equal(C, reference(local, N, 1.0), N);

    // the copy keeps the assembly data, so it can be refilled through set_values
    C.set_values(flatten(local, 3.0).vals);
    C.assemble();
    expect_equal(C, reference(local, N, 3.0), N);

    // K is unchanged (catches a copy that writes into its source)
    expect_equal(K, reference(local, N, 1.0), N);

    // all done
    return;
}


// copy assignment replaces format, pattern and values of the target
auto
check_copy_assignment(format_t format) -> void
{
    // the elements owned by this rank, and the number of equations
    const auto local = local_elements(test_elements());
    const index_t N = n_equations(local);

    // K with its pattern and values
    auto K = assembled_matrix("K", local, N, format, 1.0);

    // a matrix with the default format, no pattern and other values, to be overwritten
    auto C = reference(local, N, 5.0);

    // copy-assign {K} to {C}
    C = K;

    // check names
    EXPECT_EQ(K.name(), "K");
    EXPECT_EQ(C.name(), "K");

    // the copy owns a different PETSc matrix
    EXPECT_NE(C.get(), K.get());

    // the copy holds the values of K
    expect_equal(C, reference(local, N, 1.0), N);

    // the copy took over the format and the assembly data, so set_values works on it
    C.set_values(flatten(local, 3.0).vals);
    C.assemble();
    expect_equal(C, reference(local, N, 3.0), N);

    // K is unchanged
    expect_equal(K, reference(local, N, 1.0), N);

    // all done
    return;
}


// a move transfers the PETSc handle and the assembly data, and leaves the source empty
auto
check_move_constructor(format_t format) -> void
{
    // the elements owned by this rank, and the number of equations
    const auto local = local_elements(test_elements());
    const index_t N = n_equations(local);

    // K with its pattern and values
    auto K = assembled_matrix("K", local, N, format, 1.0);

    // the PETSc handle owned by {K}
    const auto handle = K.get();

    // move-construct {M} from {K}
    auto M = std::move(K);

    // check names
    EXPECT_EQ(M.name(), "K");

    // {M} took over the handle, {K} was left empty (inspecting the moved-from state on purpose)
    EXPECT_EQ(M.get(), handle);
    EXPECT_EQ(K.get(), nullptr);

    // the values moved along
    expect_equal(M, reference(local, N, 1.0), N);

    // the assembly data moved along, so set_values works on {M}
    M.set_values(flatten(local, 3.0).vals);
    M.assemble();
    expect_equal(M, reference(local, N, 3.0), N);

    // all done (destroying the moved-from {K} must be harmless)
    return;
}


// move assignment releases the target's matrix and takes over the source's
auto
check_move_assignment(format_t format) -> void
{
    // the elements owned by this rank, and the number of equations
    const auto local = local_elements(test_elements());
    const index_t N = n_equations(local);

    // K with its pattern and values
    auto K = assembled_matrix("K", local, N, format, 1.0);

    // a matrix with the default format, no pattern and other values, to be overwritten
    auto M = reference(local, N, 5.0);

    // the PETSc handle owned by {K}
    const auto handle = K.get();

    // move-assign {K} to {M}
    M = std::move(K);

    // check names
    EXPECT_EQ(M.name(), "K");

    // {M} took over the handle, {K} was left empty (inspecting the moved-from state on purpose)
    EXPECT_EQ(M.get(), handle);
    EXPECT_EQ(K.get(), nullptr);

    // the values and the assembly data moved along
    expect_equal(M, reference(local, N, 1.0), N);
    M.set_values(flatten(local, 3.0).vals);
    M.assemble();
    expect_equal(M, reference(local, N, 3.0), N);

    // a moved-from matrix can be assigned to again
    K = M;

    // check names
    EXPECT_EQ(M.name(), "K");

    EXPECT_NE(K.get(), M.get());
    expect_equal(K, reference(local, N, 3.0), N);

    // all done
    return;
}


TEST(Math, PETScMatrixCopyConstructor)
{
    for (const auto & [name, format] : formats) {
        // set a trace for this format, so any failure will report the format name
        SCOPED_TRACE(name);
        check_copy_constructor(format);
    }
}

TEST(Math, PETScMatrixCopyAssignment)
{
    for (const auto & [name, format] : formats) {
        // set a trace for this format, so any failure will report the format name
        SCOPED_TRACE(name);
        check_copy_assignment(format);
    }
}

TEST(Math, PETScMatrixMoveConstructor)
{
    for (const auto & [name, format] : formats) {
        // set a trace for this format, so any failure will report the format name
        SCOPED_TRACE(name);
        check_move_constructor(format);
    }
}

TEST(Math, PETScMatrixMoveAssignment)
{
    for (const auto & [name, format] : formats) {
        // set a trace for this format, so any failure will report the format name
        SCOPED_TRACE(name);
        check_move_assignment(format);
    }
}


int
main(int argc, char ** argv)
{
    // initialize PETSc
    auto environment = mito::linalg::petsc::environment();

    // initialize Google Test
    ::testing::InitGoogleTest(&argc, argv);

    // run all tests
    auto result = RUN_ALL_TESTS();

    // all done
    return result;
}


// end of file