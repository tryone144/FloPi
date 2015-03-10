// FloPi - Play Midi-Files with Floppy Drives
//
// flopi-driver.h
//
// (c) 2014 Bernd Busse

#ifndef FLOPI_DRIVER_H
#define FLOPI_DRIVER_H

#define RESOLUTION 20
extern int MAX_POSITION;

extern int currentPeriod[4];
extern int currentTick[4];

void tick(int signum);
void setupTimer(int res);

#endif // FLOPI_DRIVER_H

