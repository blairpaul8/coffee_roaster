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
  
  atexit(temp_deinit);
  atexit(pin_deinit);

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
  pin_init(27);
  

  // This is for the pre-heat stage press enter once beans are dropped
  getchar();

  while (1) {

    struct timeval curr_time = {0, 0};
    gettimeofday(&curr_time, NULL);
    double temp = temp_read() * 9 / 5 + 32;

    if (temp > ROAST_TEMP) {
      toggle_heat(false);
    } else if (temp < ROAST_TEMP) {
      toggle_heat(true);
    }

    fprintf(file, "%d,%f,%s\n", sec, temp, heat ? "On" : "Off");
    printf("%d,%f,%s\n", sec, temp, heat ? "On" : "Off");

    sleep(1);
    //sec += 1;
  }

  pin_deinit();
  temp_deinit();
  fclose(file);
  return EXIT_SUCCESS;
}
