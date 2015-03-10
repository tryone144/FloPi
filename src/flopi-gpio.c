// FloPi - Play Midi-Files with Floppy Drives
//
// flopi-gpio.c
//
// (c) 2014 Bernd Busse

#include "flopi-gpio.h"
#include "flopi-driver.h"

#include <wiringPi.h>

const int FDD1_STP =  8;    // = GPIO  2 = PIN  3
const int FDD1_DIR =  9;    // = GPIO  3 = PIN  5
const int FDD2_STP =  7;    // = GPIO  4 = PIN  7
const int FDD2_DIR =  0;    // = GPIO 17 = PIN 11
const int FDD3_STP =  5;    // = GPIO 24 = PIN 18
const int FDD3_DIR =  6;    // = GPIO 25 = PIN 22
const int FDD4_STP = 10;    // = GPIO 08 = PIN 24
const int FDD4_DIR = 11;    // = GPIO 07 = PIN 26

int currentState[] = {
        LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
int currentPosition[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int setupFDD();
void resetFDD(int dev, int dir);
void resetAll();
void tooglePin(int dev, int dir);

int setupFDD() {
    // start wiringPi-API
    if (wiringPiSetup() == -1)
        return -1;

    pinMode(FDD1_DIR, OUTPUT);
    pinMode(FDD1_STP, OUTPUT);
    pinMode(FDD2_DIR, OUTPUT);
    pinMode(FDD2_STP, OUTPUT);
    pinMode(FDD3_DIR, OUTPUT);
    pinMode(FDD3_STP, OUTPUT);
    pinMode(FDD4_DIR, OUTPUT);
    pinMode(FDD4_STP, OUTPUT);

    resetAll();

    return 0;
}

void resetFDD(int dev, int dir) {
    digitalWrite(dir, HIGH);
    int s;
    for (s=0; s<MAX_POSITION; s+=2) {
        digitalWrite(dev, HIGH);
        digitalWrite(dev, LOW);
        delay(5);
    }

    digitalWrite(dir, LOW);
}

void resetAll() {
    currentPeriod[0] = 0;
    currentPeriod[1] = 0;
    currentPeriod[3] = 0;
    currentPeriod[4] = 0;

    resetFDD(FDD1_STP, FDD1_DIR);
    resetFDD(FDD2_STP, FDD2_DIR);
    resetFDD(FDD3_STP, FDD3_DIR);
    resetFDD(FDD4_STP, FDD4_DIR);
    
    currentPosition[FDD1_STP] = 0;
    currentState[FDD1_DIR] = 0;
    currentPosition[FDD2_STP] = 0;
    currentState[FDD2_DIR] = 0;
    currentPosition[FDD3_STP] = 0;
    currentState[FDD3_DIR] = 0;
    currentPosition[FDD4_STP] = 0;
    currentState[FDD4_DIR] = 0;
}

void tooglePin(int dev, int dir) {
    if (currentPosition[dev] >= MAX_POSITION) {
        currentState[dir] = HIGH;
        digitalWrite(dir, HIGH);
    }
    else if (currentPosition[dev] <= 0) {
        currentState[dir] = LOW;
        digitalWrite(dir, LOW);
    }

    if (currentState[dir] == HIGH) {
        currentPosition[dev]--;
    } else {
        currentPosition[dev]++;
    }

    digitalWrite(dev, currentState[dev]);
    currentState[dev] = ~currentState[dev];
}

