/*****************************************************************************************************************
 *
 *    Copyrigit (C) Thornbird Raid!
 *
 *    Module:  change time to tm function!
 *
 *    File: time2tm.c
 *
 *    Author: thornbird blue
 *
 *    Date: 2012-10-18
 *
 *    Email: thornbird.blue@gmail.com
 *
 *******************************************************************************************************************/

/*******************************************************************************************************************
 *
 *    Abstract:
 *		change kernel rtc time to year,month,day,hour,min,sec
 *
 *    Compile:
 *		CC time2tm.c
 *
 ********************************************************************************************************************/

/********************************************************************************************************************
 *
 *    History:
 *
 *	Name			Date		Ver		Action
 *    -----------------------------------------------------------------------------------------------------------
 *	thornbird.blue	   	2012-10-18	1.0		create
 *
 ********************************************************************************************************************/

/** headers */
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#define LEAPS_THRU_END_OF(y) ((y)/4 - (y)/100 + (y)/400)


/** struct */
struct rtc_time {
        int tm_sec;
        int tm_min;
        int tm_hour;
        int tm_mday;
        int tm_mon;
        int tm_year;
        int tm_wday;
        int tm_yday;
        int tm_isdst;
};

static const unsigned char rtc_days_in_month[] = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};


/** functions */
static int is_leap_year(unsigned int year)
{
        return (!(year % 4) && (year % 100)) || !(year % 400);
}

static int rtc_month_days(unsigned int month, unsigned int year)
{
        return rtc_days_in_month[month] + (is_leap_year(year) && month == 1);
}

/************************************************************************************************************************
 *
 *    Main function
 *
 *    Do format: time2tm  time
 *
 ************************************************************************************************************************/
int main(int argc,char *argv[])
{
	unsigned long time;
	struct rtc_time *tm,rt;
	unsigned int month, year;
        int days;
	tm = &rt;

	if(argc < 2 || argc >2)
	{
		printf("Error format!!!\nFormat is: time2tm time\n\n");
		return -1;
	}	

#ifdef T2T_DEBUG	
	printf("\ninput time is:%s!\n",argv[1]);
#endif
	time = atol(argv[1]);
#ifdef T2T_DEBUG	
	printf("time is %ld!\n",time);
#endif
        days = time / 86400;
        time -= (unsigned int) days * 86400;

        /* day of the week, 1970-01-01 was a Thursday */
        tm->tm_wday = (days + 4) % 7;

        year = 1970 + days / 365;
        days -= (year - 1970) * 365
                + LEAPS_THRU_END_OF(year - 1)
                - LEAPS_THRU_END_OF(1970 - 1);
        if (days < 0) {
                year -= 1;
                days += 365 + is_leap_year(year);
        }
        tm->tm_year = year - 1900;
        tm->tm_yday = days + 1;

        for (month = 0; month < 11; month++) {
                int newdays;

                newdays = days - rtc_month_days(month, year);
                if (newdays < 0)
                        break;
                days = newdays;
        }
        tm->tm_mon = month;
        tm->tm_mday = days + 1;

        tm->tm_hour = time / 3600;
        time -= tm->tm_hour * 3600;
        tm->tm_min = time / 60;
        tm->tm_sec = time - tm->tm_min * 60;
	printf("\nyear is %d,month is %d,day is %d,hour is %d,min is %d,sec is %d!\n\n",tm->tm_year+1900,tm->tm_mon,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
	return 0;
}
