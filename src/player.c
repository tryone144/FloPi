/*
 * FloPi
 *
 * Floppy Music for Raspberry Pi
 *
 * (c) 2015 Bernd Busse
 */

#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <alsa/asoundlib.h>

#include "player.h"
#include "gpio.h"

// Sleep interval for FDD-Stepping
static int fddPeriod[128];
// Musical interval of notes
static int microPeriod[] = {
        0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0, // C  - B
        0,     0,     0,     0,     0,     0,     0,     0,     0, 36364, 34323, 32396, // C0 - B0
    30578, 28861, 27242, 25713, 24270, 22909, 21622, 20409, 19263, 18182, 17161, 16198, // C1 - B1
    15289, 14436, 13621, 12856, 12135, 11454, 10811, 10205,  9632,  9091,  8581,  8099, // C2 - B2
     7645,  7218,  6811,  6428,  6068,  5727,  5406,  5103,  4816,  4546,  4291,  4050, // C3 - B3
     3823,  3609,  3406,  3214,  3034,  2864,  2703,  2552,  2408,  2273,  2146,  2025, // C4 - B4
     1911,  1804,  1703,  1607,  1517,  1432,  1357,  1276,  1204,  1136,  1073,  1012, // C5 - B5
      956,   902,   851,   803,   758,   716,   676,   638,   602,   568,   536,   506, // C6 - B6
      478,   451,   426,   402,     0,     0,     0,     0,     0,     0,     0,     0};// C7 - B7

// Current note interval (4 Channels)
static int currentPeriod[] = {0, 0, 0, 0};
// Step counter on current note interval (4 Channels)
static int currentTick[] = {0, 0, 0, 0};

/* initFddPeriods()
 *
 * calculate intervals for FDD-Stepping
 * Returns: nothing
 */
void initFddPeriods() {
    for (int i = 0; i < 128; i++) {
        fddPeriod[i] = microPeriod[i] / (RESOLUTION * 2);
    }
}

/* resetFDDPeriods()
 *
 * Reset step counter for each track
 * Returns: nothing
 */
void resetPeriods() {
    // Reset each Track
    for (int i = 0; i < MAX_TRACKS; i++) {
        currentPeriod[i] = 0;
    }
}

/* playerTick(int signum)
 *
 * signum: SIGNAL
 *
 * Timer Signal handler: step FDDs according to the notes
 * Returns: nothing
 */
void playerTick(int signum) {
    // Check each Track / FDD
    for (int i = 0; i < MAX_TRACKS; i++) {
        if (currentPeriod[i]) { // Playing a note
            currentTick[i]++; // increment tick counter
            if (currentTick[i] >= currentPeriod[i]) { // Note interval over -> make noise!
                toogleFDD(i); // Do 1 Step => MUSIC!!!
                currentTick[i] = 0; // Start again
            }
        }
    }
}

/* open_seq()
 *
 * Open MIDI Sequence
 * Returns: sequence handle
 */
snd_seq_t* playerGetHandle() {
    snd_seq_t *seq_handle;
    int portid;
    int err;

    // Open ALSA sequencer
    if (err = snd_seq_open(&seq_handle, "default", SND_SEQ_OPEN_INPUT, 0) < 0) {
        fprintf(stderr, "ERROR: can't open ALSA sequencer: %s\n", snd_strerror(err));
        exit(1);
    }

    // Set client name
    snd_seq_set_client_name(seq_handle, PLAYER_NAME);

    // Open Listener-Port
    if ((portid = snd_seq_create_simple_port(seq_handle, PLAYER_NAME,
            SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
            SND_SEQ_PORT_TYPE_APPLICATION)) < 0) {
        fprintf(stderr, "ERROR: can't create sequencer port: %s\n", snd_strerror(portid));
        exit(1);
    }

    return(seq_handle);
}

/* playerMidiAction(snd_seq_t *seq_handle)
 *
 * seq_handle: sequence handle with events
 * Set note state for our tracks according to the midi events
 * Returns: nothing
 */
void playerMidiAction(snd_seq_t *seq_handle) {
    snd_seq_event_t *ev;
    do {
        snd_seq_event_input(seq_handle, &ev); // Get event
        switch (ev->type) {
            case SND_SEQ_EVENT_NOTEOFF:
                fprintf(stdout, "Note Off event on Channel %2d: %5d\n",
                        ev->data.control.channel, ev->data.note.note);
                currentPeriod[ev->data.control.channel] = 0;
                break;
            case SND_SEQ_EVENT_NOTEON:
                fprintf(stdout, "Note On event on Channel %2d: %5d with vel: %5d\n",
                        ev->data.control.channel, ev->data.note.note, ev->data.note.velocity);
                if (ev->data.note.velocity == 0) {
                    currentPeriod[ev->data.control.channel] = 0;
                } else {
                    currentPeriod[ev->data.control.channel] = fddPeriod[ev->data.note.note];
                }
                break;
            case SND_SEQ_EVENT_PITCHBEND:
                fprintf(stdout, "Pitchbender event on Channel %2d: %5d\n",
                        ev->data.control.channel, ev->data.control.value);
                break;
        }
        snd_seq_free_event(ev);
    } while (snd_seq_event_input_pending(seq_handle, 0) > 0);
}
