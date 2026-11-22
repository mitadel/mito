// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


#include <cmath>
#include <gtest/gtest.h>
#include <mito/simulation.h>


TEST(CommunicationManagerMPI, CollectivesAndPointToPoint)
{
    auto & simulation = mito::simulation::simulation();
    auto manager = mito::simulation::communication_manager_t(simulation.context());

    const auto n_tasks = manager.n_tasks();
    const auto task_id = manager.task_id();

    ASSERT_EQ(n_tasks, 2);

    auto sum_value = manager.reduce_sum(task_id + 1);
    EXPECT_EQ(sum_value, 3);

    auto min_value = manager.reduce_min(task_id + 10);
    EXPECT_EQ(min_value, 10);

    auto max_value = manager.reduce_max(task_id + 10);
    EXPECT_EQ(max_value, 11);

    auto norm = manager.reduce_norm(double(task_id + 1));
    EXPECT_NEAR(norm, std::sqrt(5.0), 1.e-12);

    auto [reduced_min, min_task] = manager.reduce_min_location(double(task_id + 1));
    EXPECT_DOUBLE_EQ(reduced_min, 1.0);
    EXPECT_EQ(min_task, 0);

    auto [reduced_max, max_task] = manager.reduce_max_location(double(task_id + 1));
    EXPECT_DOUBLE_EQ(reduced_max, 2.0);
    EXPECT_EQ(max_task, 1);

    constexpr int scalar_tag = 101;
    constexpr int vector_tag = 102;

    if (task_id == 0) {
        manager.send<int>(42, 1, scalar_tag);
        manager.send<double>({ 1.5, 3.0, 4.5 }, 1, vector_tag);

        auto response = manager.receive<int>(1, scalar_tag + 1);
        EXPECT_EQ(response, 84);
    } else if (task_id == 1) {
        auto value = manager.receive<int>(0, scalar_tag);
        EXPECT_EQ(value, 42);

        auto values = manager.receive<double>(3, 0, vector_tag);
        ASSERT_EQ(values.size(), 3);
        EXPECT_DOUBLE_EQ(values[0], 1.5);
        EXPECT_DOUBLE_EQ(values[1], 3.0);
        EXPECT_DOUBLE_EQ(values[2], 4.5);

        manager.send<int>(2 * value, 0, scalar_tag + 1);
    }

    manager.barrier();
}


// end of file
