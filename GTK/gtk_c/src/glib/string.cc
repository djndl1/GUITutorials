#include <glib.h>

#include <gtest/gtest.h>

TEST(STRING, Strip)
{
  char a[] = " \tdfasd\t    ";
  const char *result = "dfasd";

  g_strstrip(a);

  EXPECT_STREQ(result, a);
}