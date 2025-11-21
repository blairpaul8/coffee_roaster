#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define SPI_DEVICE "/dev/spidev0.0"
#define SPI_MODE SPI_MODE_0
#define SPI_BITS_PER_WORD 8
#define SPI_SPEED 1000000UL

static struct spi_ioc_transfer tr;
static int fd;
static uint8_t tx[] = {0x00, 0x00};
static uint8_t rx[] = {0, 0};

int temp_init() {

  fd = open(SPI_DEVICE, O_RDWR);
  if (fd < 0) {
    perror("Failed to open SPI_DEVICE");
    return 1;
  }

  uint8_t mode = SPI_MODE;
  if (ioctl(fd, SPI_IOC_WR_MODE, &mode) == -1) {
    perror("Cant set SPI mode");
  }

  uint8_t bits = SPI_BITS_PER_WORD;
  if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) == -1) {
    perror("Cant set max speed");
    return 1;
  }

  uint32_t speed = SPI_SPEED;
  if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed)) {
    perror("Cant set max speed");
  }

  tr = (struct spi_ioc_transfer){
      .tx_buf = (unsigned long)tx,
      .rx_buf = (unsigned long)rx,
      .len = 2,
      .speed_hz = speed,
      .bits_per_word = bits,
  };

  return 0;
}

double temp_read() {
  if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 0) {
    perror("SPI Transfer failed\n");
    close(fd);
    return 1;
  }

  const int16_t value = ((rx[0] << 8) | rx[1]) >> 3;

  const double temp_c = value * 0.25;

  return temp_c;
}

void temp_deinit() { close(fd); }
