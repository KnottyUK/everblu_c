#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <openssl/aes.h>
#include "cc1101.h"
#include "utils.h"

void parse_and_display_reading(uint8_t *decrypted) {
    for (int i = 0; i < 15; i++) {
        uint16_t value = (decrypted[i] << 8) | decrypted[i+1];
        if (value >= 20000 && value <= 30000) {
            printf("🟢 Possible Reading Found: %u at bytes %d-%d\n", value, i, i+1);
        }
    }
}

int main() {
    uint32_t serial = 482948; // Your physical serial
    uint8_t year = 16;        // Last two digits of year

    printf("Serial: %u\nYear: %u\n", serial, year);

    spi_init();  // Init CC1101

    // Build Poll Packet
    uint8_t packet[64];
    int packet_len = Make_Radian_Master_req(packet, year, serial);

    // Send Poll
    spi_transfer(packet, NULL, packet_len);

    // Read Response
    uint8_t response[16];
    spi_transfer(NULL, response, 16);

    printf("Received:\n");
    for (int i = 0; i < 16; i++) {
        printf("%02X ", response[i]);
    }
    printf("\n");

    // Decrypt
    AES_KEY decrypt_key;
    uint8_t aes_key[16];
    snprintf((char *)aes_key, 17, "%u%02u", serial, year);

    AES_set_decrypt_key(aes_key, 128, &decrypt_key);

    uint8_t decrypted[16];
    AES_decrypt(response, decrypted, &decrypt_key);

    printf("Decrypted:\n");
    for (int i = 0; i < 16; i++) {
        printf("%02X ", decrypted[i]);
    }
    printf("\n");

    // Auto-Parse
    parse_and_display_reading(decrypted);

    spi_close();
    return 0;
}

