#include <glib.h>

#include <gtest/gtest.h>

TEST(ASSERTION, G_RETURN_IF_FAIL)
{
  g_return_if_fail(false);
}