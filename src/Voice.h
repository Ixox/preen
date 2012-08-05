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

#define BLOCK_SIZE 32

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

    void init(Matrix* matrix, Lfo** lfo, Env*env1, Env*env2, Env*env3, Env*env4, Env*env5, Env*env6, Osc*osc1, Osc*osc2, Osc*osc3, Osc*osc4, Osc*osc5, Osc*osc6 );


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


    void nextBlock();

    void updateAllModulationIndexes() {
        IM1 = this->synthState->params.engine2.modulationIndex1 + (matrix->getDestination(INDEX_MODULATION1)>>4);
        IM2 = this->synthState->params.engine2.modulationIndex2 + (matrix->getDestination(INDEX_MODULATION2)>>4);
        IM3 = this->synthState->params.engine2.modulationIndex3 + (matrix->getDestination(INDEX_MODULATION3)>>4);
        IM4 = this->synthState->params.engine2.modulationIndex4 + (matrix->getDestination(INDEX_MODULATION4)>>4);
    }

    void updateAllMixOscs() {
        MIX1 = this->synthState->params.engine3.mixOsc1 + (matrix->getDestination(MIX_OSC1)>>4);
        MIX2 = this->synthState->params.engine3.mixOsc2 + (matrix->getDestination(MIX_OSC2)>>4);
        MIX3 = this->synthState->params.engine3.mixOsc3 + (matrix->getDestination(MIX_OSC3)>>4);
        MIX4 = this->synthState->params.engine3.mixOsc4 + (matrix->getDestination(MIX_OSC4)>>4);
    }

    void endNoteOrBeginNextOne() {
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

    int* getSamples() { return currentSamples; }


private:
    // voice status
    int frequency;
    bool released;
    bool playing;
    unsigned int index;
    char note;
    char velocity;
    int currentSamples[32];
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

    Osc* osc1;
    Osc* osc2;
    Osc* osc3;
    Osc* osc4;
    Osc* osc5;
    Osc* osc6;
    Env* env1;
    Env* env2;
    Env* env3;
    Env* env4;
    Env* env5;
    Env* env6;

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

    // env Value
    unsigned int env1ValueMem;
    unsigned int env2ValueMem;
    unsigned int env3ValueMem;
    unsigned int env4ValueMem;
    unsigned int env5ValueMem;
    unsigned int env6ValueMem;
};

#endif


