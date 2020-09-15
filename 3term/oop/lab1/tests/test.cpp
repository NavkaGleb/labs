#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ClassName.hpp"

namespace {

    class Declaration : public testing::Test {
    public:
        ClassName obj;

        Declaration() {

            obj;

        }

    };

}

TEST_F(Declaration, test1) {

    ASSERT_EQ("", "");

}

TEST_F(Declaration, test2) {

    ASSERT_EQ("", "1");

}