#include <glib.h>
#include <glib/gprintf.h>

#include <gtest/gtest.h>

static void short_blocking_func(gpointer data, gpointer user_data)
{
  GHashTable *thread_use = (GHashTable*)user_data;

  GThread *self = g_thread_self();
  int *count_p = NULL;
  if (!(count_p = (int*)g_hash_table_lookup(thread_use, self))) {
    count_p = g_new0(int, 1);
    *count_p = 1;
    g_hash_table_insert(thread_use, self, count_p);
  } else {
    (*count_p)++;
  }

  gsize sleep_seconds = GPOINTER_TO_SIZE(data);

  g_usleep(sleep_seconds * G_USEC_PER_SEC);

  g_printf("Thread %p %d-th time: Blocking operation finished!\n", self, *count_p);
}

TEST(Threading, ThreadPool)
{
  GHashTable *thread_use = g_hash_table_new_full(NULL, NULL, NULL, g_free);
  GThreadPool *pool = g_thread_pool_new(short_blocking_func, thread_use, 10, FALSE, NULL);

  for (int i = 0; i < 1000; i++) {
    g_thread_pool_push(pool, GINT_TO_POINTER(i % 2 + 1), NULL);
  }

  while (g_thread_pool_get_num_threads(pool) != 0) {
    sleep(1);
  }

  const GList *values = g_hash_table_get_values(thread_use);
  int cnt = 0;
  for (const GList *tmp = values; tmp != NULL; tmp = tmp->next) {
    cnt += *((int*)tmp->data);
  }

  g_thread_pool_free(pool, TRUE, TRUE);
  pool = NULL;
  g_hash_table_destroy(thread_use);
  thread_use = NULL;

  EXPECT_EQ(1000, cnt) << "Incorrect Thread running count";
}