#define _BSD_SOURCE
#define _XOPEN_SOURCE

#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/time/time.h"

// gets current utc time
int utc_now(time_t *now)
{
  time_t now_return = NULL;

  int exit_code = 0;

  check_not_null(now);

  now_return = time(NULL);
  check((int)now_return != -1, "now_return: %d",
    (int)now_return);

  *now = now_return;

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  return exit_code;
}

// parses utc time
int parse_utc_date(char *string, time_t *time)
{
  time_t time_return = NULL;

  int exit_code = 0;

  check_not_null(string);
  check_not_null(time);

  struct tm tm;

  char *strptime_result = strptime(string, "%Y-%m-%d", &tm);
  check_not_null(strptime_result);
  check(strptime_result[0] == '\0', "strptime_result[0]: %c",
    strptime_result[0]);

  tm.tm_hour = 0;
  tm.tm_min = 0;
  tm.tm_sec = 0;

  time_return = timegm(&tm);
  check((int)time_return != -1, "time_return: %d",
    (int)time_return);

  *time = time_return;

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  return exit_code;
}

// parses utc time
int parse_utc_date_time(char *string, time_t *time)
{
  time_t time_return = NULL;

  int exit_code = 0;

  check_not_null(string);
  check_not_null(time);

  struct tm tm;

  char *strptime_result = strptime(string, "%Y-%m-%dT%H:%M:%SZ", &tm);
  check_not_null(strptime_result);
  check(strptime_result[0] == '\0', "strptime_result[0]: %c",
    strptime_result[0]);

  time_return = timegm(&tm);
  check((int)time_return != -1, "time_return: %d",
    (int)time_return);

  *time = time_return;

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  return exit_code;
}

// parses utc time
int parse_utc_time(char *string, time_t *time)
{
  time_t time_return = NULL;

  int exit_code = 0;

  check_not_null(string);
  check_not_null(time);

  struct tm tm;

  char *strptime_result = strptime(string, "%H:%M:%S", &tm);
  check_not_null(strptime_result);
  check(strptime_result[0] == 0, "strptime_result[0]: %c",
    strptime_result[0]);

  tm.tm_year = 70;
  tm.tm_mon = 0;
  tm.tm_mday = 1;

  time_return = timegm(&tm);
  check((int)time_return != -1, "time_return: %d",
    (int)time_return);

  *time = time_return;

  goto cleanup;

error:

  exit_code = -1;

cleanup:

  return exit_code;
}

// formats utc time
char *format_utc_date(time_t time)
{
  char *format_utc_date_return = NULL;

  struct tm tm;

  struct tm *gm_time_result = gmtime_r(&time, &tm);
  check_not_null(gm_time_result);

  format_utc_date_return = malloc(sizeof(char) * 100);
  check_mem(format_utc_date_return);

  check_result_not(strftime(format_utc_date_return, 100, "%Y-%m-%d", &tm), 0);

  goto cleanup;

error:

  if (format_utc_date_return != NULL) { free(format_utc_date_return); }

  format_utc_date_return = NULL;

cleanup:

  return format_utc_date_return;
}

// formats utc time
char *format_utc_date_time(time_t time)
{
  char *format_utc_date_time_return = NULL;

  struct tm tm;

  struct tm *gm_time_result = gmtime_r(&time, &tm);
  check_not_null(gm_time_result);

  format_utc_date_time_return = malloc(sizeof(char) * 100);
  check_mem(format_utc_date_time_return);

  check_result_not(strftime(format_utc_date_time_return, 100, "%Y-%m-%dT%H:%M:%SZ", &tm), 0);

  goto cleanup;

error:

  if (format_utc_date_time_return != NULL) { free(format_utc_date_time_return); }

  format_utc_date_time_return = NULL;

cleanup:

  return format_utc_date_time_return;
}

// formats utc time
char *format_utc_time(time_t time)
{
  char *format_utc_time_return = NULL;

  struct tm tm;

  struct tm *gm_time_result = gmtime_r(&time, &tm);
  check_not_null(gm_time_result);

  format_utc_time_return = malloc(sizeof(char) * 100);
  check_mem(format_utc_time_return);

  check_result_not(strftime(format_utc_time_return, 100, "%H:%M:%S", &tm), 0);

  goto cleanup;

error:

  if (format_utc_time_return != NULL) { free(format_utc_time_return); }

  format_utc_time_return = NULL;

cleanup:

  return format_utc_time_return;
}
