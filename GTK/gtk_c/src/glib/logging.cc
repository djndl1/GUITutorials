#include <glib.h>

#include <gtest/gtest.h>

TEST (LOGGING, info) { g_info ("%s\n", "This is info logging"); }

TEST (LOGGING, debug) { g_debug ("%s\n", "This is debug logging"); }

TEST (LOGGING, message) { g_message ("%s\n", "This is message logging"); }

TEST (LOGGING, Warning) { g_warning ("%s\n", "This is warning logging"); }

TEST (LOGGING, Critical) { g_critical ("%s\n", "This is critical logging"); }