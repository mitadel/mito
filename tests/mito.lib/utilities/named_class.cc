// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/utilities.h>


using namespace mito::utilities;


// a simple class to test NamedClass
class Foo {
  public:
    // my type
    using foo_type = Foo;

  public:
    // constructor
    Foo(int foo) : _foo(foo) {}

    // accessor
    int foo() const { return _foo; }

  private:
    // an attribute
    int _foo;
};


// the resource type
using foo_t = NamedClass<Foo>;


TEST(Utilities, NamedClass)
{
    // instantiate a named class
    auto foo = foo_t(42);

    // assert that the name is "unnamed"
    EXPECT_EQ(foo.name(), "unnamed");

    // instantiate a named class with a name
    auto bar = foo_t("bar", 43);
    EXPECT_EQ(bar.name(), "bar");
}
