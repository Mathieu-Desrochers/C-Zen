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
  check_not_null(now);

  time_t time_result = time(NULL);
  check((int)time_result != -1, "time_result: %d",
    (int)time_result);

  *now = time_result;

  return 0;

error:

  return -1;
}

// parses utc time
int parse_utc_date(char *string, time_t *time)
{
  check_not_null(string);
  check_not_null(time);

  struct tm tm;

  char *strptime_result = strptime(string, "%Y-%m-%d", &tm);
  check(strptime_result != NULL, "strptime_result: NULL");
  check(strptime_result[0] == 0, "strptime_result[0]: %c",
    strptime_result[0]);

  tm.tm_hour = 0;
  tm.tm_min = 0;
  tm.tm_sec = 0;

  time_t timegm_result = timegm(&tm);
  check((int)timegm_result != -1, "timegm_result: %d",
    (int)timegm_result);

  *time = timegm_result;

  return 0;

error:

  return -1;
}

// parses utc time
int parse_utc_date_time(char *string, time_t *time)
{
  check_not_null(string);
  check_not_null(time);

  struct tm tm;

  char *strptime_result = strptime(string, "%Y-%m-%dT%H:%M:%SZ", &tm);
  check(strptime_result != NULL, "strptime_result: NULL");
  check(strptime_result[0] == 0, "strptime_result[0]: %c",
    strptime_result[0]);

  time_t timegm_result = timegm(&tm);
  check((int)timegm_result != -1, "timegm_result: %d",
    (int)timegm_result);

  *time = timegm_result;

  return 0;

error:

  return -1;
}

// parses utc time
int parse_utc_time(char *string, time_t *time)
{
  check_not_null(string);
  check_not_null(time);

  struct tm tm;

  char *strptime_result = strptime(string, "%H:%M:%S", &tm);
  check(strptime_result != NULL, "strptime_result: NULL");
  check(strptime_result[0] == 0, "strptime_result[0]: %c",
    strptime_result[0]);

  tm.tm_year = 70;
  tm.tm_mon = 0;
  tm.tm_mday = 1;

  time_t timegm_result = timegm(&tm);
  check((int)timegm_result != -1, "timegm_result: %d",
    (int)timegm_result);

  *time = timegm_result;

  return 0;

error:

  return -1;
}

// formats utc time
char *format_utc_date(time_t time)
{
  struct tm tm;

  struct tm *gm_time_result = gmtime_r(&time, &tm);
  check_not_null(gm_time_result);

  char *formatted_time = malloc(sizeof(char) * 100);
  check_mem(formatted_time);

  check_result_not(strftime(formatted_time, 100, "%Y-%m-%d", &tm), 0);

  return formatted_time;

error:

  if (formatted_time != NULL) { free(formatted_time); }

  return NULL;
}

// formats utc time
char *format_utc_date_time(time_t time)
{
  struct tm tm;

  struct tm *gm_time_result = gmtime_r(&time, &tm);
  check_not_null(gm_time_result);

  char *formatted_time = malloc(sizeof(char) * 100);
  check_mem(formatted_time);

  check_result_not(strftime(formatted_time, 100, "%Y-%m-%dT%H:%M:%SZ", &tm), 0);

  return formatted_time;

error:

  if (formatted_time != NULL) { free(formatted_time); }

  return NULL;
}

// formats utc time
char *format_utc_time(time_t time)
{
  struct tm tm;

  struct tm *gm_time_result = gmtime_r(&time, &tm);
  check_not_null(gm_time_result);

  char *formatted_time = malloc(sizeof(char) * 100);
  check_mem(formatted_time);

  check_result_not(strftime(formatted_time, 100, "%H:%M:%S", &tm), 0);

  return formatted_time;

error:

  if (formatted_time != NULL) { free(formatted_time); }

  return NULL;
}
