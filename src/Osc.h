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

#pragma once

#include "Matrix.h"

#define SAMPLE_RATE_x_8 264144


extern short sinTable[];
extern int frequenciesx8[];

struct OscState {
	int index;
	int frequency;
	int mainFrequency;
};

template <int number>
class Osc
{
public:
	Osc();
	~Osc(void);
	void init(Matrix* matrix);

	void newNote(struct OscState& oscState, int note);

/*
	int getSample(struct OscState &oscState) {
		int rest = oscState.index & 0x7f;
		int div128 = oscState.index >> 7;
		int div128plus1 = (div128 + 1) & 0x7ff;
		return (sinTable[div128]* (128-rest) + sinTable[div128plus1]*rest) >> 8;
	}
*/

	int getSample(struct OscState &oscState) {
	    switch(oscillator->shape) {
	    case 0:
	        return sinTable[oscState.index >> 7]; // * ((1024 + this->matrix->getDestination(destAmp)) >> 10) ;
	    case 1:
	    {
	        int s = sinTable[oscState.index >> 7];
	        return (s*s) >> 16;
	    }
        case 2:
            if (oscState.index<0x1ffff) {
                return sinTable[oscState.index >> 7]; // * ((1024 + this->matrix->getDestination(destAmp)) >> 10) ;
            } else {
                return 0;
            }
            break;
        case 3:
            oscState.index &= 0x1ffff;
            return sinTable[oscState.index >> 7];
            break;
	    }
	    return 0;
	};

	void nextSample(struct OscState &oscState) {
		// oscState.index = (oscState.index + (((oscState.frequency + (this->matrix->getDestination(destFreq)>>4)) << 16) / SAMPLE_RATE_x_8 )) & 0xffff;
		int jmp = this->matrix->getDestination(destFreq);
		jmp >>= 4;
		jmp += oscState.frequency;
/*
		jmp <<= 12;
		jmp /= SAMPLE_RATE_x_8;
		jmp <<= 4;
  	        Every thing canceled if we go 4 times further
		*/
		oscState.index += jmp;
		oscState.index &= 0x3ffff;
	}


private:
	DestinationEnum destFreq;
	DestinationEnum destAmp;
	Matrix* matrix;
	OscillatorParams* oscillator;
};



template <int number>
Osc<number>::Osc()
{
	switch (number-1) {
	case 0:
		this->destFreq = OSC1_FREQ;
//		this->destAmp = OSC1_AMP;
		break;
	case 1:
		this->destFreq = OSC2_FREQ;
//		this->destAmp = OSC2_AMP;
		break;
	case 2:
		this->destFreq = OSC3_FREQ;
//		this->destAmp = OSC3_AMP;
		break;
	case 3:
		this->destFreq = OSC4_FREQ;
//		this->destAmp = OSC4_AMP;
		break;
	}
	OscillatorParams * o = (OscillatorParams *)(&(synthState.params.osc1));
	oscillator = &o[number-1];
}

template <int number>
Osc<number>::~Osc()
{
}

template <int number>
void Osc<number>::init(Matrix* matrix) {
	this->matrix = matrix;
}

template <int number>
void Osc<number>::newNote(struct OscState& oscState, int note) {
	oscState.index = 1 << number;
	switch (oscillator->frequencyType) {
	case OSC_FT_KEYBOARD:
		oscState.mainFrequency = (frequenciesx8[note] * (oscillator->frequencyMul) >> 4) + oscillator->detune;
		break;
	case OSC_FT_FIXE:
		oscState.mainFrequency = (oscillator->frequencyMul << 7) + oscillator->detune;
		break;
	}
	oscState.frequency = oscState.mainFrequency;
}
