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

#define MAX_NUMBER_OF_VOICES 4

#define UINT_MAX  4294967295

class Synth : public SynthParamListener
{
public:
    Synth(void);
    ~Synth(void);
    void noteOn(char note, char velocity);
    void noteOff(char note);
    void allNoteOff();
    void allSoundOff();
    bool isPlaying();
    Matrix* getMatrix() { return &matrix; }
    int getSample();
    void nextSample();

    void newParamValueFromExternal(SynthParamType type, int currentRow, int encoder, ParameterDisplay* param, int oldValue, int newValue) {
        newParamValue(type, currentRow, encoder, param, oldValue, newValue);
    }

    void newParamValue(SynthParamType type, int currentRow, int encoder, ParameterDisplay* param, int oldValue, int newValue) {
        if (type == SYNTH_PARAM_TYPE_ENV) {
            switch (currentRow) {
            case ROW_ENV1:
                env1.reloadADSR();
                break;
            case ROW_ENV2:
                env2.reloadADSR();
                break;
            case ROW_ENV3:
                env3.reloadADSR();
                break;
            case ROW_ENV4:
                env4.reloadADSR();
                break;
            case ROW_ENV5:
                env5.reloadADSR();
                break;
            case ROW_ENV6:
                env6.reloadADSR();
                break;
            }
        } else if (type == SYNTH_PARAM_TYPE_MATRIX && encoder == ENCODER_MATRIX_DEST) {
            // Reset all destination
            matrix.resetAllDestination();
        } else if (type == SYNTH_PARAM_TYPE_LFO && encoder == ENCODER_LFO_KSYNC) {
            lfo[currentRow - ROW_LFO1].reloadRamp();
        }
    }

    void newcurrentRow(int newcurrentRow)  {
        // Nothing to do
    }
    void beforeNewParamsLoad() {
        // Stop all voices
        allSoundOff();
    };


private:
    Matrix matrix;
    Voice voices[MAX_NUMBER_OF_VOICES];
    unsigned int voiceIndex;
    int cpt;

    Lfo lfo[NUMBER_OF_LFOS];

    // 4 oscillators Max
    Osc<1> osc1;
    Osc<2> osc2;
    Osc<3> osc3;
    Osc<4> osc4;
    Osc<5> osc5;
    Osc<6> osc6;
    // And their 4 envelopes
    Env<1> env1;
    Env<2> env2;
    Env<3> env3;
    Env<4> env4;
    Env<5> env5;
    Env<6> env6;
};



#endif

