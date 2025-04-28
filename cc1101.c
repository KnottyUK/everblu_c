#include "cc1101.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

int spi_fd;  // Now only defined here

void spi_init(void) {
    spi_fd = open("/dev/spidev0.0", O_RDWR);
    if (spi_fd < 0) {
        perror("Failed to open SPI device");
        return;
    }

    uint8_t mode = SPI_MODE_0;
    ioctl(spi_fd, SPI_IOC_WR_MODE, &mode);
    uint8_t bits = 8;
    ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    uint32_t speed = 500000;
    ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
}

void spi_close(void) {
    if (spi_fd >= 0) {
        close(spi_fd);
    }
}

int spi_transfer(unsigned char *tx_buf, unsigned char *rx_buf, int len) {
    struct spi_ioc_transfer transfer = {
        .tx_buf = (unsigned long) tx_buf,
        .rx_buf = (unsigned long) rx_buf,
        .len = len,
        .delay_usecs = 0,
        .speed_hz = 500000,
        .bits_per_word = 8,
    };

    return ioctl(spi_fd, SPI_IOC_MESSAGE(1), &transfer);
}

