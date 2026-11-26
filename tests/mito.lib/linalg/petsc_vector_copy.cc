// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


#include <gtest/gtest.h>
#include <mito.h>

#include <utility>
#include <vector>

#include <type_traits>    // add to the includes

// moves of a named vector are real moves, not copies in disguise
static_assert(std::is_nothrow_move_constructible_v<mito::linalg::petsc::vector_t>);
static_assert(std::is_nothrow_move_assignable_v<mito::linalg::petsc::vector_t>);

// the size of the vectors in this test
constexpr int N = 5;


// set entry {i} of {vector} to {f(i)} and assemble the vector
auto
fill_vector(auto & vector, auto && f) -> void
{
    // set vector entries
    for (int i = 0; i < N; i++) {
        vector.insert_value(i, f(i));
    }

    // assemble the vector
    vector.assemble();

    // all done
    return;
}

// check that entry {i} of {vector} is {i + 1}
auto
expect_one_to_n(const auto & vector) -> void
{
    // read the vector
    auto result = std::vector<double>(N);
    vector.get_vector(result);

    // check the entries
    for (int i = 0; i < N; i++) {
        EXPECT_DOUBLE_EQ(result[i], double(i + 1)) << "entry " << i;
    }

    // all done
    return;
}


TEST(Math, PETScVectorCopyConstructor)
{
    // x = (1, 2, 3, 4, 5)
    auto x = mito::linalg::petsc::vector("x", N);
    fill_vector(x, [](int i) { return double(i + 1); });

    // copy-construct {y} from {x}
    auto y = x;

    // check names
    EXPECT_EQ(x.name(), "x");
    EXPECT_EQ(y.name(), "x");

    // the copy owns a different PETSc vector of the same size
    EXPECT_NE(y.get(), x.get());
    EXPECT_EQ(y.size(), N);

    // modify the original
    x.scale(-1.0);

    // the copy keeps the original values
    expect_one_to_n(y);

    // all done
    return;
}


TEST(Math, PETScVectorCopyAssignment)
{
    // x = (1, 2, 3, 4, 5)
    auto x = mito::linalg::petsc::vector("x", N);
    fill_vector(x, [](int i) { return double(i + 1); });

    // a vector of a different size, to be overwritten
    auto y = mito::linalg::petsc::vector("y", 2 * N);

    // copy-assign {x} to {y}
    y = x;

    // check names
    EXPECT_EQ(x.name(), "x");
    EXPECT_EQ(y.name(), "x");

    // the copy owns a different PETSc vector, with the size of {x}
    EXPECT_NE(y.get(), x.get());
    EXPECT_EQ(y.size(), N);

    // modify the original
    x.scale(-1.0);

    // the copy keeps the original values
    expect_one_to_n(y);

    // all done
    return;
}


TEST(Math, PETScVectorMoveConstructor)
{
    // x = (1, 2, 3, 4, 5)
    auto x = mito::linalg::petsc::vector("x", N);
    fill_vector(x, [](int i) { return double(i + 1); });

    // the PETSc handle owned by {x}
    const auto handle = x.get();

    // move-construct {y} from {x}
    auto y = std::move(x);

    // check names
    EXPECT_EQ(y.name(), "x");

    // {y} took over the handle, {x} was left empty (inspecting the moved-from state on purpose)
    EXPECT_EQ(y.get(), handle);
    EXPECT_EQ(x.get(), nullptr);

    // the values moved along
    expect_one_to_n(y);

    // all done (destroying the moved-from {x} must be harmless)
    return;
}


TEST(Math, PETScVectorMoveAssignment)
{
    // x = (1, 2, 3, 4, 5)
    auto x = mito::linalg::petsc::vector("x", N);
    fill_vector(x, [](int i) { return double(i + 1); });

    // a vector with other values, whose PETSc vector must be released by the assignment
    auto y = mito::linalg::petsc::vector("y", N);
    fill_vector(y, [](int) { return 42.0; });

    // the PETSc handle owned by {x}
    const auto handle = x.get();

    // move-assign {x} to {y}
    y = std::move(x);

    // check names
    EXPECT_EQ(y.name(), "x");

    // {y} took over the handle, {x} was left empty (inspecting the moved-from state on purpose)
    EXPECT_EQ(y.get(), handle);
    EXPECT_EQ(x.get(), nullptr);

    // the values moved along
    expect_one_to_n(y);

    // a moved-from vector can be assigned to again
    x = y;

    // check names
    EXPECT_EQ(y.name(), "x");

    EXPECT_NE(x.get(), y.get());
    expect_one_to_n(x);

    // all done
    return;
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