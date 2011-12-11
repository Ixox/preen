/*
 * Copyright 2011 Xavier Hosxe
 *
 * Author: Xavier Hosxe (xavier.hosxe@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MIDIDECODER_H_
#define MIDIDECODER_H_

#include "SynthStateAware.h"
#include "Synth.h"
#include "RingBuffer.h"

enum AllControlChange {
    CC_BANK_SELECT = 0,
    CC_MODWHEEL = 1,
    CC_VOICE = 9,
    CC_GLIDE = 11,
    CC_IM1 = 16,
    CC_IM2  ,
    CC_IM3,
    CC_IM4,
    CC_MIX1 = 20,
    CC_MIX2,
    CC_MIX3,
    CC_MIX4,
    CC_MATRIXROW1_MUL = 24,
    CC_MATRIXROW2_MUL,
    CC_MATRIXROW3_MUL,
    CC_MATRIXROW4_MUL,
    CC_MATRIXROW5_MUL,
    CC_MATRIXROW6_MUL,
    CC_MATRIXROW7_MUL,
    CC_MATRIXROW8_MUL,
    CC_ENV1_ATTACK = 46,
    CC_ENV2_ATTACK,
    CC_ENV3_ATTACK,
    CC_ENV4_ATTACK,
    CC_ENV5_ATTACK,
    CC_ENV6_ATTACK,
    CC_ENV1_RELEASE = 52,
    CC_ENV2_RELEASE,
    CC_ENV3_RELEASE,
    CC_ENV4_RELEASE,
    CC_ENV5_RELEASE,
    CC_ENV6_RELEASE,
    CC_OSC1_FREQ = 58,
    CC_OSC2_FREQ,
    CC_OSC3_FREQ,
    CC_OSC4_FREQ,
    CC_OSC5_FREQ,
    CC_OSC6_FREQ,
    CC_OSC1_DETUNE = 102,
    CC_OSC2_DETUNE,
    CC_OSC3_DETUNE,
    CC_OSC4_DETUNE,
    CC_OSC5_DETUNE,
    CC_OSC6_DETUNE,
    CC_LFO1_FREQ = 108,
    CC_LFO2_FREQ,
    CC_LFO3_FREQ,
    CC_STEPSEQ5_BPM,
    CC_STEOSEQ6_BPM,
    CC_STEPSEQ5_GATE,
    CC_STEPSEQ6_GATE,
    CC_MATRIX_SOURCE_CC1 = 115,
    CC_MATRIX_SOURCE_CC2,
    CC_MATRIX_SOURCE_CC3,
    CC_MATRIX_SOURCE_CC4,
    // 119 is empty
    CC_ALL_SOUND_OFF = 120,
    CC_ALL_NOTES_OFF = 123

};

struct Nrpn {
    uint8 paramLSB;
    uint8 paramMSB;
    uint8 valueLSB;
    uint8 valueMSB;
    bool readyToSend;
};

enum EventType {
    MIDI_NOTE_OFF = 0x80,
    MIDI_NOTE_ON = 0x90,
    MIDI_CONTROL_CHANGE = 0xb0,
    MIDI_PROGRAM_CHANGE =0xc0,
    MIDI_AFTER_TOUCH = 0xd0,
    MIDI_PITCH_BEND = 0xe0
};

enum EventState {
	MIDI_EVENT_NEW = 0,
    MIDI_EVENT_IN_PROGRESS ,
	MIDI_EVENT_COMPLETE
};

struct MidiEventState {
    EventState eventState;
    uint8 numberOfBytes;
    uint8 index;
};

struct MidiEvent {
	uint8 channel;
	EventType eventType;
	uint8 value[2];
};

class MidiDecoder : public SynthParamListener, public SynthStateAware
{
public:
    MidiDecoder();
    ~MidiDecoder();
    void newByte(unsigned char byte);
    void midiEventReceived(MidiEvent midiEvent);
    void controlChange(MidiEvent& midiEvent);
    void decodeNrpn();
    void setSynth(Synth* synth);
    void newParamValueFromExternal(SynthParamType type, int currentrow, int encoder, ParameterDisplay* param, int oldValue, int newValue);
    void newParamValue(SynthParamType type, int currentrow, int encoder, ParameterDisplay* param, int oldValue, int newValue);
    void newcurrentRow(int newcurrentRow) {}
    void beforeNewParamsLoad() {};
    void afterNewParamsLoad() {};
    void sendMidiOut();
    void readSysex();
    void playNote(char note, char velocity) {};
    void stopNote(char note) {};

    boolean hasMidiToSend() {
        return (midiToSend.getCount()>0);
    }

private:
    struct MidiEventState currentEventState;
    struct MidiEvent currentEvent;
    Synth* synth;
    Matrix* matrix;
    RingBuffer<MidiEvent, 16> midiToSend;
    struct Nrpn currentNrpn;
};

#endif /* MIDIDECODER_H_ */
