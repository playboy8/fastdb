#pragma once
#include <vector>
#include <string>
#include<iostream>

typedef struct 
{
    int iSec;
    int iMin;
    int iHour;

    int iDay;
    int iMon;
    int iYear;
    
    int iWday;

    int iMillisec;
}UTIL_Time, *UTIL_TimePtr;

#ifndef __cplusplus
#define bool int
#define false 0
#define true 1
#endif

bool PUTIL_GetLocalTime(UTIL_TimePtr pTime);
// 将SYSTEMTIME格式转化为 YYYYMMDDHHMMSSMMM
long long PUTIL_SystemTimeToDateTime(UTIL_TimePtr stTime);

int diff_ms(long long time1, long long time2);

bool GetModulePath(char* Path, int PathLen);




class diff_count
{
private:
		UTIL_Time time;
        std::vector<long long>timestap;
        long long  start_t;
        long long sum_t;

    /* data */
public:
    bool start();
    bool stop();
    bool add_snap();
    int show_diff();
    int show_diff(int& totle_delay , int& core_delay, bool show=false);

    diff_count(/* args */);
    ~diff_count();
};
