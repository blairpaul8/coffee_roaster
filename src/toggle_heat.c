#include <fcntl.h>
#include <linux/gpio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

struct gpiohandle_request req;

int pin_init(int pin) {
  int chip_fd = open("/dev/gpiochip0", O_RDWR);
  if (chip_fd < 0) {
    printf("failed to open gpiochip0\n");
    return 1;
  }

  memset(&req, 0, sizeof(req));

  req.lineoffsets[0] = pin;
  req.flags = GPIOHANDLE_REQUEST_OUTPUT;
  req.default_values[0] = 0;
  req.lines = 1;

  if (ioctl(chip_fd, GPIO_GET_LINEHANDLE_IOCTL, &req) < 0) {
    printf("set up gpio pin ioctl failed\n");
    close(chip_fd);
    return 1;
  }
  close(chip_fd);
  return 0;
}

void toggle_heat(bool heat) {
  struct gpiohandle_data data;

  data.values[0] = heat;
  if (ioctl(req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data) < 0) {
    printf("Failed to toggle heat\n");
    exit(1);
  }
}

void pin_deinit() { close(req.fd); }
