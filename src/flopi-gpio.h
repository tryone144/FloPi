// FloPi - Play Midi-Files with Floppy Drives
//
// flopi-gpio.h
//
// (c) 2014 Bernd Busse

#ifndef FLOPI_GPIO_H
#define FLOPI_GPIO_H

extern const int FDD1_DIR;
extern const int FDD1_STP;
extern const int FDD2_DIR;
extern const int FDD2_STP;
extern const int FDD3_DIR;
extern const int FDD3_STP;
extern const int FDD4_DIR;
extern const int FDD4_STP;

extern int currentState[16];
extern int currentPosition[16];

int setupFDD();
void resetFDD(int dev, int dir);
void resetAll();
void tooglePin(int dev, int dir);

#endif // FLOPI_GPIO_H

