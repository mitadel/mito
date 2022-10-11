#include <gtest/gtest.h>
#include <mito/utilities.h>

#include <memory>

TEST(Shareable, DefaultConstructor)
{
    auto shared_ptr1 = std::make_shared<int>(1);

    mito::utilities::Shareable shareable1;

    EXPECT_EQ(shareable1.references(), shared_ptr1.use_count());
}

TEST(Shareable, CopyConstructor)
{
    auto shared_ptr1 = std::make_shared<int>(1);
    std::shared_ptr<int> shared_ptr2 = shared_ptr1;

    mito::utilities::Shareable shareable1;
    mito::utilities::Shareable shareable2 = shareable1;

    EXPECT_EQ(shareable1.references(), shared_ptr1.use_count());
    EXPECT_EQ(shareable2.references(), shared_ptr2.use_count());
}

TEST(Shareable, MoveConstructor)
{
    auto shared_ptr1 = std::make_shared<int>(1);
    std::shared_ptr<int> shared_ptr2(std::move(shared_ptr1));

    mito::utilities::Shareable shareable1;
    mito::utilities::Shareable shareable2(std::move(shareable1));

    EXPECT_EQ(shareable2.references(), shared_ptr2.use_count());
}

TEST(Shareable, AssignmentOperator)
{
    auto shared_ptr1 = std::make_shared<int>(1);
    auto shared_ptr2 = std::make_shared<int>(1);
    shared_ptr2 = shared_ptr1;

    mito::utilities::Shareable shareable1;
    mito::utilities::Shareable shareable2;
    shareable2 = shareable1;

    EXPECT_EQ(shareable1.references(), shared_ptr1.use_count());
    EXPECT_EQ(shareable2.references(), shared_ptr2.use_count());
}

TEST(Shareable, MoveAssignmentOperator)
{
    auto shared_ptr1 = std::make_shared<int>(1);
    auto shared_ptr2 = std::make_shared<int>(1);
    shared_ptr2 = std::move(shared_ptr1);

    mito::utilities::Shareable shareable1;
    mito::utilities::Shareable shareable2;
    shareable2 = std::move(shareable1);

    EXPECT_EQ(shareable2.references(), shared_ptr2.use_count());
}

TEST(Shareable, ThreeWayAssignment)
{
    auto shared_ptr1 = std::make_shared<int>(1);
    std::shared_ptr<int> shared_ptr2 = shared_ptr1;
    auto shared_ptr3 = std::make_shared<int>(1);
    shared_ptr2 = shared_ptr3;

    mito::utilities::Shareable shareable1;
    mito::utilities::Shareable shareable2 = shareable1;
    mito::utilities::Shareable shareable3;
    shareable2 = shareable3;

    EXPECT_EQ(shareable1.references(), shared_ptr1.use_count());
    EXPECT_EQ(shareable2.references(), shared_ptr2.use_count());
    EXPECT_EQ(shareable3.references(), shared_ptr3.use_count());
}

TEST(Shareable, Destructor)
{
    std::shared_ptr<int> shared_ptr2;
    {
        auto shared_ptr1 = std::make_shared<int>(1);
        shared_ptr2 = shared_ptr1;
    }

    mito::utilities::Shareable shareable2;
    {
        mito::utilities::Shareable shareable1;
        shareable2 = shareable1;
    }

    EXPECT_EQ(shareable2.references(), shared_ptr2.use_count());
}