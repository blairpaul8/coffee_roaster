#include <bits/types/struct_timeval.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define ROAST_TEMP 150

#include "read_temp.h"
#include "toggle_heat.h"

int main(int argc, char *argv[]) {

  int increment = 5;
  struct timeval start_time = {0, 0};
  gettimeofday(&start_time, NULL);

  struct tm tmr;
  const time_t tmt = time(NULL);

  localtime_r(&tmt, &tmr);
  char filename[256];

  sprintf(filename, "roast_%02d%02d_%02d%02d.csv", tmr.tm_mday, tmr.tm_mon,
          tmr.tm_hour, tmr.tm_sec);
  FILE *file = fopen(filename, "w");

  int sec = 1;
  bool heat = true;

  temp_init();

  // This is for the pre-heat stage press enter once beans are dropped
  getchar();

  while (1) {

    struct timeval curr_time = {0, 0};
    gettimeofday(&curr_time, NULL);
    double temp = temp_read();

    if (temp > ROAST_TEMP) {
      toggle_heat(false);
    } else if (temp < ROAST_TEMP) {
      toggle_heat(true);
    }

    fprintf(file, "%d,%f,%s\n", sec, temp, heat ? "On" : "Off");

    sleep(1);
    sec += 1;
  }
  fclose(file);
  return EXIT_SUCCESS;
}
