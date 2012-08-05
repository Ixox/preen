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


#include "SynthStateAware.h"
#include "Matrix.h"
#include "math.h"

extern const short int sinTable[];

extern const int frequenciesX8[];

extern short noise[];

struct OscState {
    int index;
    int frequency;
    int mainFrequencyPlusMatrix;
    int mainFrequency;
    int fromFrequency;
    int nextFrequency;
};

struct WaveTable {
	const short* table;
	int max;
	int rightShift;
	int useFreq;
	int intToAdd;
};

extern struct WaveTable waveTables[];


class Osc : public SynthStateAware
{
public:
    Osc() {};
    virtual ~Osc() {};

    void init(int number, Matrix* matrix);

    void newNote(struct OscState& oscState, int note);
    void glideToNote(struct OscState& oscState, int note);
    void glideStep(struct OscState& oscState, int step);

    void calculateFrequencyWithMatrix(struct OscState *oscState) {
		oscState->mainFrequencyPlusMatrix = oscState->mainFrequency + ((oscState->mainFrequency   * (this->matrix->getDestination(destFreq) + this->matrix->getDestination(ALL_OSC_FREQ))) >> 14);
    }


   	int getNextSample(struct OscState *oscState)  {
   		const struct WaveTable* waveTable = &waveTables[oscillator->shape];

   		oscState->index +=  oscState->frequency * waveTable->useFreq + waveTable->intToAdd;
    	oscState->index &= waveTable->max;
		return waveTable->table[oscState->index >> waveTable->rightShift];
   	}

   	int* getNextBlock(struct OscState *oscState)  {
   		int max = waveTables[oscillator->shape].max;
   		int rs = waveTables[oscillator->shape].rightShift;
   		int index = oscState->index;
   		int freq = oscState->frequency * waveTables[oscillator->shape].useFreq + waveTables[oscillator->shape].intToAdd;
   		const short *wave = waveTables[oscillator->shape].table;

   		for (int k=0; k<32; k++) {
            index +=  freq;
            index &= max;
    		oscValues[k] = wave[index >> rs];
   		}
    	oscState->index = index;
    	return oscValues;
    };


private:
    DestinationEnum destFreq;
    Matrix* matrix;
    int oscValues[32];
    OscillatorParams* oscillator;
};


