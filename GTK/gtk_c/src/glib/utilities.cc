#include <glib.h>
#include <gtest/gtest.h>

TEST(UUID, GUUID)
{
  gchar *uuid = g_uuid_string_random();

  g_uuid_string_is_valid(uuid);

  g_free(uuid);
  uuid = NULL;
}