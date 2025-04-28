#ifndef WIRINGPISPI_H
#define WIRINGPISPI_H

int wiringPiSPISetup(int channel, int speed);
int wiringPiSPIDataRW(int channel, unsigned char *data, int len);

#endif // WIRINGPISPI_H

