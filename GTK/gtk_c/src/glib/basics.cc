extern "C" {
#include <glib.h>
}

#include <gtest/gtest.h>

#include <typeinfo>

struct mystruct {
    int A;
    double B;
};

TEST(BASIC_TYPES, OFFSET_STRUCT) {
    glong offset = G_STRUCT_OFFSET(mystruct, B);

    EXPECT_EQ(offset, 8);
}

TEST(BASIC_TYPES, GBoolean)
{
    EXPECT_EQ(0, FALSE);
    EXPECT_EQ(1, TRUE);

    EXPECT_EQ(typeid(gboolean), typeid(int));
}

TEST(BASIC_TYPES, Alignment) {
    EXPECT_EQ(8, G_MEM_ALIGN);
}