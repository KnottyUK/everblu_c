#include <stdio.h>
#include "radian_trx.h"

int main() {
    printf("Starting EverBlu Meter Reader...\n");
    radian_init();

    // Assuming radian_read() handles reading and outputs result
    while (1) {
        radian_read();
        sleep(5);  // Wait before next read
    }

    return 0;
}

