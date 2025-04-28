#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <time.h>

// CRC Kermit Table (used for packet validation if needed)
static uint16_t crc_tab[256];

// Initialize CRC Table (optional, if CRC used)
void init_crc_tab(void) {
    uint16_t i, j, crc, c;

    for (i = 0; i < 256; i++) {
        crc = 0x0000;
        c = i;
        for (j = 0; j < 8; j++) {
            if ((crc ^ c) & 0x0001) {
                crc = (crc >> 1) ^ 0x8408;
            } else {
                crc = crc >> 1;
            }
            c = c >> 1;
        }
        crc_tab[i] = crc;
    }
}

// Optional CRC calculation function
uint16_t crc_kermit(const unsigned char *input_ptr, size_t num_bytes) {
    uint16_t crc;
    const unsigned char *ptr;
    size_t a;

    crc = 0x0000;
    ptr = input_ptr;

    for (a = 0; a < num_bytes; a++) {
        crc = (crc >> 8) ^ crc_tab[(crc ^ *ptr++) & 0xff];
    }

    return crc;
}

// Packet Encoder (dummy function for now, for serial encoding)
int encode2serial_1_3(uint8_t *inputBuffer, int inputBufferLen, uint8_t *outputBuffer) {
    if (!inputBuffer || !outputBuffer || inputBufferLen <= 0) return 0;

    // Just copy input to output for now
    memcpy(outputBuffer, inputBuffer, inputBufferLen);
    return inputBufferLen;
}

// Print Time (for debug)
void print_time(void) {
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    printf("[%s] ", buffer);
}

// Build Radian Master Request Packet
int Make_Radian_Master_req(uint8_t *outputBuffer, uint8_t year, uint32_t serial) {
    if (!outputBuffer) return 0;

    // Example: Construct a 16-byte packet
    outputBuffer[0] = (serial >> 16) & 0xFF;
    outputBuffer[1] = (serial >> 8) & 0xFF;
    outputBuffer[2] = serial & 0xFF;
    outputBuffer[3] = year & 0xFF;

    // Fill the rest with 0xAA for testing
    for (int i = 4; i < 16; i++) {
        outputBuffer[i] = 0xAA;
    }

    return 16;  // Return packet length
}

