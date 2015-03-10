/*
 * FloPi
 *
 * Floppy Music for Raspberry Pi
 *
 * (c) 2015 Bernd Busse
 */

#ifndef GPIO_H
#define GPIO_H

#define FDD1_STP    8   // = GPIO  2 = PIN  3
#define FDD1_DIR    9   // = GPIO  3 = PIN  5
#define FDD2_STP    7   // = GPIO  4 = PIN  7
#define FDD2_DIR    0   // = GPIO 17 = PIN 11
#define FDD3_STP    5   // = GPIO 24 = PIN 18
#define FDD3_DIR    6   // = GPIO 25 = PIN 22
#define FDD4_STP   10   // = GPIO 08 = PIN 24
#define FDD4_DIR   11   // = GPIO 07 = PIN 26

#define MAX_POSITION  158   // Maximum steps of FDD

extern int setupFDD();
extern void toggleFDD(int);

#endif // GPIO_H
