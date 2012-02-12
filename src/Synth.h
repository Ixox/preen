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
#include "LfoOsc.h"
#include "LfoEnv.h"
#include "LfoStepSeq.h"
#include "Env.h"
#include "SynthParamListener.h"
#include "wirish.h"

#define MAX_NUMBER_OF_VOICES 4

#define UINT_MAX  4294967295

class Synth : public SynthParamListener, public SynthStateAware
{
public:
    Synth(void);
    ~Synth(void);

	void setSynthState(SynthState* sState) {
		SynthStateAware::setSynthState(sState);
		init();
	}

    void noteOn(char note, char velocity);
    void noteOff(char note);
    void allNoteOff();
    void allSoundOff();
    bool isPlaying();
    Matrix* getMatrix() { return &matrix; }
    int getSample();
    void nextSample();


    // Overide SynthParamListener
    void playNote(char note, char velocity) {
    	noteOn(note, velocity);
    }
    void stopNote(char note) {
    	noteOff(note);
    }


    void newParamValueFromExternal(SynthParamType type, int currentRow, int encoder, ParameterDisplay* param, int oldValue, int newValue) {
        newParamValue(type, currentRow, encoder, param, oldValue, newValue);
    }

    void newParamValue(SynthParamType type, int currentRow, int encoder, ParameterDisplay* param, int oldValue, int newValue);
    void checkMaxVoice(boolean setEngineVoice);

    void newcurrentRow(int newcurrentRow)  {
        // Nothing to do
    }
    void beforeNewParamsLoad() {
        // Stop all voices
        allSoundOff();
    };

    void afterNewParamsLoad();

    void midiClockSetSongPosition(int songPosition) {
    	// nothing to do
    }

    void midiClockContinue(int songPosition) {
    	for (int k=0; k<NUMBER_OF_LFO; k++) {
    		lfo[k]->midiClock(songPosition, false);
    	}
    	this->recomputeNext = ((songPosition&0x1)==0);
    }


    void midiClockStart() {
    	for (int k=0; k<NUMBER_OF_LFO; k++) {
    		lfo[k]->midiContinue();
    	}
    	this->recomputeNext = true;
    }

    void midiClockStop() {
    }

    void midiClockSongPositionStep(int songPosition) {
		for (int k=0; k<NUMBER_OF_LFO; k++) {
			lfo[k]->midiClock(songPosition, this->recomputeNext);
		}
		if ((songPosition & 0x1)==0) {
			this->recomputeNext = true;
		}
    }

private:
    // Called by setSynthState
    void init();

    Matrix matrix;
    Voice voices[MAX_NUMBER_OF_VOICES];
    unsigned int voiceIndex;
    int cpt;

    LfoOsc lfoOsc[NUMBER_OF_LFO_OSC];
    LfoEnv lfoEnv[NUMBER_OF_LFO_ENV];
    LfoStepSeq lfoStepSeq[NUMBER_OF_LFO_STEP];

    Lfo* lfo[NUMBER_OF_LFO];

    // Must recompute LFO steps ?
    boolean recomputeNext;

    int currentGate;

    // 6 oscillators Max
    Osc<1> osc1;
    Osc<2> osc2;
    Osc<3> osc3;
    Osc<4> osc4;
    Osc<5> osc5;
    Osc<6> osc6;
    // And their 6 envelopes
    Env<1> env1;
    Env<2> env2;
    Env<3> env3;
    Env<4> env4;
    Env<5> env5;
    Env<6> env6;
};



#endif

