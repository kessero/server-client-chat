#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include"m_time.h"

char* time_send()
{

  time_t tmt = time(NULL);
  struct tm tm = *localtime(&tmt);
  int hour = tm.tm_hour;
  int min = tm.tm_min;
  int sec = tm.tm_sec;

  char hour_c[15];
  char min_c[15];
  char sec_c[15];
  sprintf(hour_c, "%d", hour);
  sprintf(min_c, "%d", min);
  sprintf(sec_c, "%d", sec);
  char time_all[80];
  strcpy(time_all, hour_c);
  strcat(time_all, ":");
  strcat(time_all, min_c);
  strcat(time_all, ":");
  strcat(time_all, sec_c);
  char *recv_time = malloc(80);
  strcpy(recv_time, time_all);
return recv_time;
}
