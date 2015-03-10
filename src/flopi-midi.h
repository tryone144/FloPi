// FloPi - Play Midi-Files with Floppy Drives
//
// flopi-midi.h
//
// (c) 2014 Bernd Busse

#include <alsa/asoundlib.h>

#ifndef FLOPI_MIDI_H
#define FLOPI_MIDI_H

extern char FLOPI_SEQ_NAME[];

extern int microPeriod[128];
extern int fddPeriod[128];

snd_seq_t *open_seq();
void midi_action(snd_seq_t *seq_handle);

#endif // FLOPI_MIDI_H


