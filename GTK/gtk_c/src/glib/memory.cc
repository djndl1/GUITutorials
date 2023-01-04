#include <glib.h>

#include <gtest/gtest.h>

TEST(Memory, ClearPointer)
{
  int *five_ints = g_new(int, 5);
  for (int i = 0; i < 5; i++) {
    five_ints[i] = i;
  }

  g_clear_pointer(&five_ints, g_free);
  five_ints = NULL;
}

TEST(Memory, ClearNullPointer)
{
  int *null_pointer = NULL;
  gpointer pointer_to_null = &null_pointer; // where null_pointer resized on the stack
  g_clear_pointer((gpointer*)pointer_to_null, g_free);
  null_pointer = NULL;
}

TEST(Memory, StealPointerSaveTyping)
{
  int *five_ints = g_new(int, 5);
  for (int i = 0; i < 5; i++) {
    five_ints[i] = i;
  }

  int *stealing = (int*)g_steal_pointer(&five_ints);
  EXPECT_EQ(five_ints, nullptr);

  g_clear_pointer(&stealing, g_free);
  stealing = NULL;
}