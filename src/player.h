/*
 * FloPi
 *
 * Floppy Music for Raspberry Pi
 *
 * (c) 2015 Bernd Busse
 */

#ifndef __PLAYER_H
#define __PLAYER_H

#include <alsa/asoundlib.h>

#define PLAYER_NAME     "FloPi MIDI-Player"

#define RESOLUTION     20   // Resolution for interval calulation
#define MAX_TRACKS      4   // Numer of tracks we can play (#FDDs)

// Tracknumber mapping
enum Tracks {TRACK01, TRACK02, TRACK03, TRACK04};

extern void initFDDPeriods();
extern void resetFDDPeriods();
extern void playerTick(int);

extern snd_seq_t* playerGetHandle();
extern void playerMidiAction(snd_seq_t*);

#endif // __PLAYER_H
