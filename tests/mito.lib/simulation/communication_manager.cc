// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


#include <gtest/gtest.h>
#include <mito/simulation.h>


TEST(CommunicationManager, Basics)
{
    auto & simulation = mito::simulation::simulation();
    auto manager = mito::simulation::communication_manager_t(simulation.context());

    EXPECT_EQ(manager.task_id(), simulation.context().task_id());
    EXPECT_EQ(manager.n_tasks(), simulation.context().n_tasks());

    EXPECT_EQ(manager.reduce_sum(7), 7);
    EXPECT_DOUBLE_EQ(manager.reduce_sum(3.5), 3.5);
    EXPECT_EQ(manager.reduce_min(-12), -12);
    EXPECT_EQ(manager.reduce_max(11), 11);
    EXPECT_DOUBLE_EQ(manager.reduce_norm(4.0), 4.0);

    auto [min_value, min_task] = manager.reduce_min_location(2.0);
    EXPECT_DOUBLE_EQ(min_value, 2.0);
    EXPECT_EQ(min_task, manager.task_id());

    auto [max_value, max_task] = manager.reduce_max_location(5.0);
    EXPECT_DOUBLE_EQ(max_value, 5.0);
    EXPECT_EQ(max_task, manager.task_id());

    auto tag = mito::simulation::communication_manager_t::encode_tag(3, 17);
    auto [origin, destination] = mito::simulation::communication_manager_t::decode_tag(tag);
    EXPECT_EQ(origin, 3);
    EXPECT_EQ(destination, 17);

    manager.barrier();
}


// end of file
