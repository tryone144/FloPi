/*
 * FloPi
 *
 * Floppy Music for Raspberry Pi
 *
 * (c) 2015 Bernd Busse
 */

#include <wiringPi.h>

#include "gpio.h"
#include "player.h"

// Current position of Step-PINS (16 PINS)
int curStepPosition[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// Current state of Direction-PINS (16 PINS)
int curDirection[] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};

static void resetFDD(int dev, int dir);
static void resetAll();

/* setupFDD()
 *
 * Init GPIO PINS (set to zero)
 * Returns: 0  on success
 *          -1 on failure
 */
int setupFDD() {
    // start wiringPi-API
    if (wiringPiSetup() == -1) {
        return -1;
    }

    // Set FDD-Pins to OUTPUT Mode
    pinMode(FDD1_DIR, OUTPUT);
    pinMode(FDD1_STP, OUTPUT);
    pinMode(FDD2_DIR, OUTPUT);
    pinMode(FDD2_STP, OUTPUT);
    pinMode(FDD3_DIR, OUTPUT);
    pinMode(FDD3_STP, OUTPUT);
    pinMode(FDD4_DIR, OUTPUT);
    pinMode(FDD4_STP, OUTPUT);

    // Reset to zero
    resetAll();

    return 0;
}

/* resetFDD(int dev, int dir)
 *
 * dev: GPIO-PIN for device stepping
 * dir: GPIO-PIN for device direction
 *
 * Move FDD back to zero position and set state registers to zero
 * Returns: nothing
 */
static void resetFDD(int dev, int dir) {
    // Move to zero position
    digitalWrite(dir, HIGH); // Backward stepping

    for (int s = 0; s < MAX_POSITION; s += 2) {
        // Step back to zero position
        digitalWrite(dev, HIGH);
        digitalWrite(dev, LOW);
        delay(5);
    }

    digitalWrite(dir, LOW); // Forward stepping

    // Set state registers to zero
    curStepPosition[dev] = 0;
    curDirection[dir] = LOW;
}

/* resetAll()
 *
 * Move all FDDs to zero position and set state registers to zero
 * Returns: nothing
 */
static void resetAll() {
    // Move to zero position and set state to zero
    resetFDD(FDD1_STP, FDD1_DIR);
    resetFDD(FDD2_STP, FDD2_DIR);
    resetFDD(FDD3_STP, FDD3_DIR);
    resetFDD(FDD4_STP, FDD4_DIR);

    // Set note interval counter to zero
    resetFDDPeriods();
}

void toggleFDD(int trackId) {
    int dev;
    int dir;

    // Map track number to FDD-PINS
    if (trackId == TRACK01) {
        dev = FDD1_STP;
        dir = FDD1_DIR;
    } else if (trackId == TRACK02) {
        dev = FDD2_STP;
        dir = FDD2_DIR;
    } else if (trackId == TRACK03) {
        dev = FDD3_STP;
        dir = FDD3_DIR;
    } else if (trackId == TRACK04) {
        dev = FDD4_STP;
        dir = FDD4_DIR;
    }

    // Change direction if necessary
    if (curStepPosition[dev] >= MAX_POSITION) {
        curDirection[dir] = HIGH;
        digitalWrite(dir, HIGH); // Backward stepping
    } else if (curStepPosition[dev] <= 0) {
        curDirection[dir] = LOW;
        digitalWrite(dir, LOW); // Forward stepping
    }

    // Change position according to current direction
    if (curDirection[dir] == HIGH) {
        curStepPosition[dev]--; // Go backward
    } else {
        curStepPosition[dev]++; // Go forward
    }

    // Step FDD GPIO-PIN
    digitalWrite(dev, curDirection[dev]);
    curDirection[dev] = ~curDirection[dev];
}
