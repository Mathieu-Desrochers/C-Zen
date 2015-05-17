#define _BSD_SOURCE
#define _XOPEN_SOURCE

#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#include "../../infrastructure/dbg/dbg.h"
#include "../../infrastructure/time/time.h"

// parses utc time
int parse_utc_date(char *string, time_t *time)
{
  check(string != NULL, "string: NULL");
  check(time != NULL, "time: NULL");

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
  check(string != NULL, "string: NULL");
  check(time != NULL, "time: NULL");

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
  check(string != NULL, "string: NULL");
  check(time != NULL, "time: NULL");

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
  check(gm_time_result != NULL, "gm_time_result: NULL");

  char *formatted_time = malloc(sizeof(char) * 100);
  check_mem(formatted_time);

  size_t strftime_result = strftime(formatted_time, 100, "%Y-%m-%d", &tm);
  check(strftime_result != 0, "strftime_result: %d",
    (int)strftime_result);

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
  check(gm_time_result != NULL, "gm_time_result: NULL");

  char *formatted_time = malloc(sizeof(char) * 100);
  check_mem(formatted_time);

  size_t strftime_result = strftime(formatted_time, 100, "%Y-%m-%dT%H:%M:%SZ", &tm);
  check(strftime_result != 0, "strftime_result: %d",
    (int)strftime_result);

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
  check(gm_time_result != NULL, "gm_time_result: NULL");

  char *formatted_time = malloc(sizeof(char) * 100);
  check_mem(formatted_time);

  size_t strftime_result = strftime(formatted_time, 100, "%H:%M:%S", &tm);
  check(strftime_result != 0, "strftime_result: %d",
    (int)strftime_result);

  return formatted_time;

error:

  if (formatted_time != NULL) { free(formatted_time); }

  return NULL;
}
