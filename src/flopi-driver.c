// FloPi - Play Midi-Files with Floppy Drives
//
// flopi-driver.c
//
// (c) 2014 Bernd Busse

#include "flopi-driver.h"
#include "flopi-gpio.h"
#include "flopi-midi.h"

#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include <alsa/asoundlib.h>
#include <wiringPi.h>

int MAX_POSITION = 158;

int currentPeriod[] = {
        0, 0, 0, 0};
int currentTick[] = {
        0, 0, 0, 0};

void tick(int signum) {
    if (currentPeriod[0]) {
        currentTick[0]++;
        if (currentTick[0] >= currentPeriod[0]) {
            tooglePin(FDD1_STP, FDD1_DIR);
            currentTick[0] = 0;
        }
    }
    if (currentPeriod[1]) {
        currentTick[1]++;
        if (currentTick[1] >= currentPeriod[1]) {
            tooglePin(FDD2_STP, FDD2_DIR);
            currentTick[1] = 0;
        }
    }
    if (currentPeriod[2]) {
        currentTick[2]++;
        if (currentTick[2] >= currentPeriod[2]) {
            tooglePin(FDD3_STP, FDD3_DIR);
            currentTick[2] = 0;
        }
    }
    if (currentPeriod[3]) {
        currentTick[3]++;
        if (currentTick[3] >= currentPeriod[3]) {
            tooglePin(FDD4_STP, FDD4_DIR);
            currentTick[3] = 0;
        }
    }
}

void setupTimer(int res) {
    struct sigaction sa;
    struct itimerval timer;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &tick;
    sigaction(SIGALRM, &sa, NULL);

    timer.it_value.tv_sec = 2;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = res;

    setitimer(ITIMER_REAL, &timer, NULL);
}

int main() {
    if (setupFDD() == -1) {
        printf("ERROR: failed to initialize GPIO.\n");
        return 1;
    }

    initFddPeriods();

    setupTimer(RESOLUTION);
    
    snd_seq_t *seq_handle;
    int npfd;
    struct pollfd *pfd;
    
    seq_handle = open_seq();
    npfd = snd_seq_poll_descriptors_count(seq_handle, POLLIN);
    pfd = (struct pollfd *)alloca(npfd * sizeof(struct pollfd));
    snd_seq_poll_descriptors(seq_handle, pfd, npfd, POLLIN);

    while (1) {
        if (poll(pfd, npfd, 100000) > 0) {
            midi_action(seq_handle);
        }
    }
}

