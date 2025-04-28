#ifndef CC1101_H
#define CC1101_H

#include <stdint.h>

extern int spi_fd;  // External declaration of spi_fd

void spi_init(void);
void spi_close(void);
int spi_transfer(unsigned char *tx_buf, unsigned char *rx_buf, int len);

#endif
