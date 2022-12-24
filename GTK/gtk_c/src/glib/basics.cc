#include <glib.h>

#include <gtest/gtest.h>

#include <typeinfo>

struct mystruct
{
  int A;
  double B;
};

TEST (BASIC_TYPES, OFFSET_STRUCT)
{
  glong offset = G_STRUCT_OFFSET (mystruct, B);

  EXPECT_EQ (offset, 8);
}

TEST (BASIC_TYPES, GBoolean)
{
  EXPECT_EQ (0, FALSE);
  EXPECT_EQ (1, TRUE);

  EXPECT_EQ (typeid (gboolean), typeid (int));
}

TEST (BASIC_TYPES, Alignment) { EXPECT_EQ (8, G_MEM_ALIGN); }

TEST (BASIC_TYPES, FloatingPointDecomposition)
{
  gfloat f = 1;
  GFloatIEEE754 fdecomposed = { .v_float = f };
  EXPECT_EQ (0, fdecomposed.mpn.sign);
  EXPECT_EQ (0, fdecomposed.mpn.mantissa);
  EXPECT_EQ (127, fdecomposed.mpn.biased_exponent);
}

TEST(BASIC_TYPES, GString)
{
  GString *s = g_string_new(NULL); // an empty string

  EXPECT_STREQ(s->str, "");

  g_string_append(s, "ABCD中文");
  EXPECT_EQ(6, g_utf8_strlen(s->str, s->len));

  g_string_free(s, TRUE);
  s = NULL;
}