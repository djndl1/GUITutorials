extern "C" {
    #include <glib.h>
}

#include <gtest/gtest.h>

#include <filesystem>

TEST(ENV, CWD) {
    const gchar *cwd = g_get_current_dir();

    std::filesystem::path current_path = std::filesystem::current_path();

    g_message("%s\n", cwd);
    g_message("%s\n", current_path.c_str());

    EXPECT_STREQ(current_path.c_str(), cwd);
}

TEST(TIME, REAL_TIME) {
    gint64 current_timestamp = g_get_real_time(); // UTC time

    GDateTime* current_datetime = g_date_time_new_from_unix_utc(current_timestamp / 1000'000);
    GDateTime* current_localdatetime = g_date_time_new_from_unix_local(current_timestamp / 1000'000);

    gchar *isotime = g_date_time_format_iso8601(current_datetime);
    gchar *isolocaltime = g_date_time_format_iso8601(current_localdatetime);

    g_message("Current UTC time: %s, %ld\n", isotime, current_timestamp);
    g_message("Current local time: %s at %s\n", isolocaltime, g_date_time_get_timezone_abbreviation(current_localdatetime));

    g_free(isotime);
    isotime = NULL;
    g_free(isolocaltime);
    isolocaltime = NULL;
    g_date_time_unref(current_datetime);
    current_datetime = NULL;
    g_date_time_unref(current_localdatetime);
    current_localdatetime = NULL;
}

TEST(TIME, GDateTime) {
    GTimeZone *tz = g_time_zone_new_local();
    GDateTime *current_datetime = g_date_time_new_now(tz);
    g_time_zone_unref(tz);
    tz = NULL;
    // tz still available since it's refcounted
    GTimeZone *current_timezone = g_date_time_get_timezone(current_datetime);
    EXPECT_NE(current_timezone, (std::nullptr_t)NULL);
    g_message("Current Timezone: %s\n", g_time_zone_get_identifier(current_timezone));

    gchar *isotime = g_date_time_format_iso8601(current_datetime);

    const gchar* tz_abbr = g_date_time_get_timezone_abbreviation(current_datetime);
    g_message("Current local time: %s %s\n", isotime, tz_abbr);

    g_free(isotime);
    isotime = NULL;

    GDateTime *current_utc = g_date_time_new_now_utc();
    gchar *iso_utctime = g_date_time_format_iso8601(current_utc);
    g_message("Current UTC time: %s\n", iso_utctime);

    g_free(iso_utctime);
    iso_utctime = NULL;

    EXPECT_GE(g_date_time_compare(current_utc, current_datetime), 0);

    g_date_time_unref(current_datetime);
    current_datetime = NULL;
    g_date_time_unref(current_utc);
    current_utc = NULL;
}

TEST(TIME, MONO_TIME) {
    // basically the uptime
    gint64 then = g_get_monotonic_time();

    g_usleep(2000);

    gint64 now = g_get_monotonic_time();

    g_message("Monotonic time: %lf hours\n", now / ((gdouble)G_USEC_PER_SEC) / 3600);

    EXPECT_GT(now - then, 2000);
}

TEST(DATE, GDATE) {
    GDate *date = g_date_new_dmy(1, G_DATE_NOVEMBER, 2022);

    gchar formatted_date[15] = { 0 };
    g_date_strftime(formatted_date, 15, "%F", date);
    g_message("Date: %s\n",formatted_date);

    GDate *old_date = g_date_copy(date);

    g_date_add_days(date, 10);
    g_date_strftime(formatted_date, 15, "%F", date);
    g_message("Date: %s\n",formatted_date);

    gint days = g_date_days_between(old_date, date);
    g_message("Days between: %d\n", days);

    EXPECT_GE(g_date_compare(date, old_date), 0);

    g_date_free(date);
    date = NULL;
    g_date_free(old_date);
    old_date = NULL;
}

TEST(DATETIME, TimeZone) {
    GTimeZone *timezone = g_time_zone_new_local();

    gint64 current_utc = g_get_real_time();
    gint tz_interval = g_time_zone_find_interval(timezone, G_TIME_TYPE_UNIVERSAL, current_utc);

    const gchar *tz_id = g_time_zone_get_identifier(timezone);
    const gchar *tz_abbr = g_time_zone_get_abbreviation(timezone, tz_interval);
    gint32 tz_offset = g_time_zone_get_offset(timezone, tz_interval);
    gboolean is_dst = g_time_zone_is_dst(timezone, tz_interval);


    g_message("Timezone at %s %s with offset %f hours, %s\n",
        tz_id, tz_abbr, tz_offset / 3600.0,
        is_dst ? "Day Light Saving" : "non-dst");


    g_time_zone_unref(timezone);
    timezone = NULL;
}