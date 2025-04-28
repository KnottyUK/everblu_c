#ifndef RADIAN_TRX_H
#define RADIAN_TRX_H

#include <stdint.h>

void init_crc_tab(void);
uint16_t crc_kermit(const unsigned char *input_ptr, size_t num_bytes);
int encode2serial_1_3(uint8_t *inputBuffer, int inputBufferLen, uint8_t *outputBuffer);
int Make_Radian_Master_req(uint8_t *outputBuffer, uint8_t year, uint32_t serial);

#endif // RADIAN_TRX_H

