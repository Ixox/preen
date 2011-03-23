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

#include "Synth.h"
#include "RingBuffer.h"

enum AllControlChange {
	CC_MODWHEEL = 1,
	CC_IM1,
	CC_IM2,
	CC_IM3,
	CC_MATRIXROW1_MUL,
	CC_MATRIXROW2_MUL,
	CC_MATRIXROW3_MUL,
	CC_MATRIXROW4_MUL,
	CC_MATRIXROW5_MUL,
	CC_MATRIXROW6_MUL,
	CC_ENV1_ATTACK,
	CC_ENV2_ATTACK,
	CC_ENV3_ATTACK,
	CC_ENV4_ATTACK,
	CC_ENV1_SUSTAIN,
	CC_ENV2_SUSTAIN,
	CC_ENV3_SUSTAIN,
	CC_ENV4_SUSTAIN,
	CC_ENV1_DECAY,
	CC_ENV2_DECAY,
	CC_ENV3_DECAY,
	CC_ENV4_DECAY,
	CC_OSC1_FREQ,
	CC_OSC2_FREQ,
	CC_OSC3_FREQ,
	CC_OSC4_FREQ,
	CC_OSC1_DETUNE,
	CC_OSC2_DETUNE,
	CC_OSC3_DETUNE,
	CC_OSC4_DETUNE,
	CC_LFO1_FREQ,
	CC_LFO2_FREQ,
	CC_LFO3_FREQ,
	CC_LFO4_FREQ,
};

struct ControlChange {
	uint8 control;
	uint8 value;
};

class MidiDecoder : public SynthParamListener
{
public:
	MidiDecoder();
	~MidiDecoder();
	void newByte(unsigned char byte);
	void sendMidiEvent();
	void setSynth(Synth* synth);
	void newParamValueFromExternal(SynthParamListenerType type, int currentrow, int encoder, ParameterDisplay* param, int oldValue, int newValue);
    void newParamValue(SynthParamListenerType type, int currentrow, int encoder, ParameterDisplay* param, int oldValue, int newValue);
    void newcurrentRow(int newcurrentRow) {}
    void sendOneMidiEvent();

private:
	unsigned char currentEvent[3];
	bool newEvent;
	short index;
	short numberOfBytes;
	unsigned char channel;
	Synth* synth;
	Matrix* matrix;
	RingBuffer<ControlChange, 16> midiToSend;
	int maxInBuffer;
	int maxInARow;
};

#endif /* MIDIDECODER_H_ */
