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

#ifndef VOICE_H_
#define VOICE_H_

#include "SynthState.h"
#include "Matrix.h"
#include "Osc.h"
#include "Env.h"
#include "Lfo.h"

extern int max;
extern int min;


class Voice : public SynthStateAware
{
public:
    Voice();
    ~Voice(void);

    void init(Matrix* matrix, Lfo** lfo, Env<1>*env1, Env<2>*env2, Env<3>*env3, Env<4>*env4, Env<5>*env5, Env<6>*env6, Osc<1>*osc1, Osc<2>*osc2, Osc<3>*osc3, Osc<4>*osc4, Osc<5>*osc5, Osc<6>*osc6 );


    int getSample() {
        return currentSample;
    }

    void calculateFrequencyWithMatrix() {
    	osc1->calculateFrequencyWithMatrix(&oscState1);
    	osc2->calculateFrequencyWithMatrix(&oscState2);
    	osc3->calculateFrequencyWithMatrix(&oscState3);
    	osc4->calculateFrequencyWithMatrix(&oscState4);
        if (showUp[this->synthState->params.engine1.algo].osc>4) {
        	osc5->calculateFrequencyWithMatrix(&oscState5);
        	osc6->calculateFrequencyWithMatrix(&oscState6);
        }
    }


    void nextSample();

    void updateModulationIndex1() {
        IM1 = this->synthState->params.engine2.modulationIndex1 + (matrix->getDestination(INDEX_MODULATION1)>>4);
    }
    void updateModulationIndex2() {
        IM2 = this->synthState->params.engine2.modulationIndex2 + (matrix->getDestination(INDEX_MODULATION2)>>4);
    }
    void updateModulationIndex3() {
        IM3 = this->synthState->params.engine2.modulationIndex3 + (matrix->getDestination(INDEX_MODULATION3)>>4);
    }
    void updateModulationIndex4() {
        IM4 = this->synthState->params.engine2.modulationIndex4 + (matrix->getDestination(INDEX_MODULATION4)>>4);
    }
    void updateMixOsc1() {
        MIX1 = this->synthState->params.engine3.mixOsc1 + (matrix->getDestination(MIX_OSC1)>>4);
    }
    void updateMixOsc2() {
        MIX2 = this->synthState->params.engine3.mixOsc2 + (matrix->getDestination(MIX_OSC2)>>4);
    }
    void updateMixOsc3() {
        MIX3 = this->synthState->params.engine3.mixOsc3 + (matrix->getDestination(MIX_OSC3)>>4);
    }
    void updateMixOsc4() {
        MIX4 = this->synthState->params.engine3.mixOsc4 + (matrix->getDestination(MIX_OSC4)>>4);
    }

    void endNoteOfBeginNextOne() {
        if (newNotePending) {
            noteOn(nextNote, nextVelocity, index);
            this->newNotePending = false;
        } else {
            playing = false;
        }
    }

    void noteOnWithoutPop(short note, char velocity, unsigned int index);
    void noteOn(short note, char velocity, unsigned int index);
    void glideToNote(short newNote);
    void killNow();
    void noteOff();
    void glideNoteOff();
    void glide();

    bool isReleased() { return this->released; }
    bool isPlaying() { return this->playing; }
    bool isGliding() { return this->gliding; }
    unsigned int getIndex() { return this->index; }
    char getNote() { return this->note; }
    char getNextNote() { return this->nextNote; }

private:
    // voice status
    int frequency;
    bool released;
    bool playing;
    unsigned int index;
    char note;
    char velocity;
    int currentSample;

    Matrix* matrix;
    // optimization
    static int IM1, IM2, IM3, IM4;
    static int MIX1, MIX2, MIX3, MIX4;

    EnvData envState1;
    EnvData envState2;
    EnvData envState3;
    EnvData envState4;
    EnvData envState5;
    EnvData envState6;

    OscState oscState1;
    OscState oscState2;
    OscState oscState3;
    OscState oscState4;
    OscState oscState5;
    OscState oscState6;

    Osc<1>* osc1;
    Osc<2>* osc2;
    Osc<3>* osc3;
    Osc<4>* osc4;
    Osc<5>* osc5;
    Osc<6>* osc6;
    Env<1>* env1;
    Env<2>* env2;
    Env<3>* env3;
    Env<4>* env4;
    Env<5>* env5;
    Env<6>* env6;

    // Fixing the "plop" when all notes are buisy...
    bool newNotePending;
    char nextNote;
    char nextVelocity;
    unsigned int nextIndex;

    // Gliding ?
    bool gliding;
    int glideStep;

    // lfos
    Lfo **lfo;
};

#endif


