#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

extern char *tzname[];

int main() {
    time_t now;
    struct tm *sp;
    putenv("TZ=America/Los_Angeles");
    (void) time( &now );

    printf("%s", ctime(&now));

    sp = localtime(&now);

    printf("%d/%d/%02d %d:%02d %s\n",
    sp->tm_mon + 1, sp->tm_mday,
    sp->tm_year + 1900, sp->tm_hour,
    sp->tm_min, tzname[sp->tm_isdst]);
    exit(0);
}
      /*struct tm {
      int tm_sec;     seconds after minute [0,61]
      int tm_min;     minutes after the hour [0,59]
      int tm_hour;    hour since midnight [0,23]
      int tm_mday;    day of the month [1,31]
      int tm_mon;     months since January [0,11]
      int tm_year;    years since 1900
      int tm_wday;    days since Sunday [0,6]
      int tm_yday;    days since January 1 [0,365]
      int tm_isdst;   flag for alternate daylight savings time
      };*/

