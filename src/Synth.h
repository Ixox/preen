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

#ifndef SYNTH_H_
#define SYNTH_H_

#include "Voice.h"
#include "Matrix.h"
#include "Lfo.h"
#include "Env.h"
#include "SynthParamListener.h"
#include "wirish.h"
#include "LiquidCrystal.h"

#define NUMBER_OF_VOICES 5
#define NUMBER_OF_LFOS 4
#define NUMBER_OF_ENV 4

#define UINT_MAX  4294967295

class Synth : public SynthParamListener
{
public:
	Synth(void);
	~Synth(void);
	void noteOn(char note, char velocity);
	void noteOff(char note);
	void allNoteOff();
	bool isPlaying();
	Matrix* getMatrix() { return &matrix; }
	int getSample();
	void nextSample();

	void newParamValueFromExternal(SynthParamListenerType type, int currentRow, int encoder, ParameterDisplay* param, int oldValue, int newValue) {
		newParamValue(type, currentRow, encoder, param, oldValue, newValue);
	}

	void newParamValue(SynthParamListenerType type, int currentRow, int encoder, ParameterDisplay* param, int oldValue, int newValue) {
		if (type == SYNTH_PARAM_ENVELOPE_LISTENER) {
			if (currentRow == 5) {
				env1.loadADSR();
			}
			if (currentRow == 6) {
				env2.loadADSR();
			}
			if (currentRow == 7) {
				env3.loadADSR();
			}
			if (currentRow == 8) {
				env4.loadADSR();
			}
		}
	}

    void newcurrentRow(int newcurrentRow)  {
    	// Nothing to do
    }

private:
	Matrix matrix;
	Voice voices[NUMBER_OF_VOICES];
	unsigned int voiceIndex;
	int cpt;

	Lfo lfo[NUMBER_OF_LFOS];

	// 4 oscillators Max
	Osc<1> osc1;
	Osc<2> osc2;
	Osc<3> osc3;
	Osc<4> osc4;
	// And their 4 envelopes
	Env<1> env1;
	Env<2> env2;
	Env<3> env3;
	Env<4> env4;
};



#endif

