/*
 * FloPi
 *
 * Floppy Music for Raspberry Pi
 *
 * (c) 2015 Bernd Busse
 */

#include <stdio.h>
#include <signal.h>
#include <alloca.h>
#include <sys/time.h>
#include <alsa/asoundlib.h>

#include "gpio.h"
#include "player.h"

/* setupTimer(int res)
 *
 * res: run timer each RES microseconds
 *
 * initialize the musical timer
 * Returns: nothing
 */
void setupTimer(int res) {
    struct sigaction sa;
    struct itimerval timer;

    // Set timer handler
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &playerTick; // tick()
    sigaction(SIGALRM, &sa, NULL);

    // Set timer values
    timer.it_value.tv_sec = 2; // First start after 2 seconds
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = res; // Run each RESOLUTION microseconds

    setitimer(ITIMER_REAL, &timer, NULL); // Start timer
}

/* main()
 *
 * program enty point
 */
int main() {
    // Init GPIO-PINS and FDDS
    if (setupFDD() == -1) {
        fprintf(stderr, "ERROR: failed to initialize GPIO.\n");
        return 1;
    }

    // Setup musical intervals and Timer
    initFDDPeriods();
    setupTimer(RESOLUTION);

    // Setup MIDI stuff
    snd_seq_t *seq_handle;
    int npfd;
    struct pollfd *pfd;
    seq_handle = playerGetHandle();
    npfd = snd_seq_poll_descriptors_count(seq_handle, POLLIN);
    pfd = (struct pollfd *)alloca(npfd * sizeof(struct pollfd));
    snd_seq_poll_descriptors(seq_handle, pfd, npfd, POLLIN);

    // MAIN LOOP
    while (1) {
        // Read MIDI Events and play them
        if (poll(pfd, npfd, 100000) > 0) {
            playerMidiAction(seq_handle);
        }
    }
}
