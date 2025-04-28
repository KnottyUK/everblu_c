#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <openssl/aes.h>
#include "cc1101.h"
#include "utils.h"

#define POLL_INTERVAL_SECONDS 5  // Time between polls

void print_hex(const char *label, const uint8_t *data, int len) {
    printf("%s:\n", label);
    for (int i = 0; i < len; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

void parse_decrypted(const uint8_t *decrypted) {
    for (int i = 0; i < 15; i++) {
        uint16_t val = (decrypted[i] << 8) | decrypted[i + 1];
        if (val >= 20000 && val <= 30000) {
            printf("🟢 Possible Reading Found: %u at bytes %d-%d\n", val, i, i + 1);
        }
    }
}

int main() {
    uint32_t serial = 482948;
    uint8_t year = 16;  // Use 16 for 2016
    uint8_t packet[64];
    uint8_t response[64];
    uint8_t decrypted[16];

    AES_KEY decrypt_key;
    uint8_t aes_key[16] = {0};

    // 🔑 Prepare AES key based on Serial + Year
    snprintf((char*)aes_key, sizeof(aes_key), "%06d%02d", serial, year);

    AES_set_decrypt_key(aes_key, 128, &decrypt_key);

    spi_init();

    while (1) {
        int packet_len = Make_Radian_Master_req(packet, year, serial);
        spi_transfer(packet, NULL, packet_len);  // Send request

        usleep(500000);  // Wait 500ms before trying to read

        spi_transfer(NULL, response, 16);  // Receive 16-byte response

        AES_decrypt(response, decrypted, &decrypt_key);

        printf("\n============================\n");
        printf("🔄 New Poll Attempt\n");
        print_hex("📥 Received", response, 16);
        print_hex("🔓 Decrypted", decrypted, 16);
        parse_decrypted(decrypted);
        printf("============================\n");

        sleep(POLL_INTERVAL_SECONDS);
    }

    spi_close();
    return 0;
}

