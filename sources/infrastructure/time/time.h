#ifndef TIME_H
#define TIME_H

#include <time.h>

// parses utc time
int parse_utc_date(char *string, time_t *time);
int parse_utc_date_time(char *string, time_t *time);
int parse_utc_time(char *string, time_t *time);

// formats utc time
char *format_utc_date(time_t time);
char *format_utc_date_time(time_t time);
char *format_utc_time(time_t time);

#endif
