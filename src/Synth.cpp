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

#include <stdlib.h>
#include "Synth.h"

Synth::Synth(void) {
}

Synth::~Synth(void) {
}

// Must be call after setSynthState
void Synth::init() {
    lfo[0] = &lfoOsc[0];
    lfo[1] = &lfoOsc[1];
    lfo[2] = &lfoOsc[2];
    lfo[3] = &lfoEnv[0];
    lfo[4] = &lfoStepSeq[0];
    lfo[5] = &lfoStepSeq[1];

    matrix.setSynthState(this->synthState);

    env1.setSynthState(this->synthState);
    env2.setSynthState(this->synthState);
    env3.setSynthState(this->synthState);
    env4.setSynthState(this->synthState);
    env5.setSynthState(this->synthState);
    env6.setSynthState(this->synthState);

    env1.init(1, &matrix);
    env2.init(2, &matrix);
    env3.init(3, &matrix);
    env4.init(4, &matrix);
    env5.init(5, &matrix);
    env6.init(6, &matrix);

    osc1.setSynthState(this->synthState);
    osc2.setSynthState(this->synthState);
    osc3.setSynthState(this->synthState);
    osc4.setSynthState(this->synthState);
    osc5.setSynthState(this->synthState);
    osc6.setSynthState(this->synthState);

    osc1.init(1, &matrix);
    osc2.init(2, &matrix);
    osc3.init(3, &matrix);
    osc4.init(4, &matrix);
    osc5.init(5, &matrix);
    osc6.init(6, &matrix);

    // OSC + ENV
    for (int k = 0; k < NUMBER_OF_LFO_OSC + NUMBER_OF_LFO_ENV; k++) {
        lfo[k]->setSynthState(this->synthState);
        lfo[k]->init(k, &this->matrix, (SourceEnum)(MATRIX_SOURCE_LFO1 + k), (DestinationEnum)(LFO1_FREQ + k));
    }
    // Step sequencer
    for (int k = 0; k< NUMBER_OF_LFO_STEP; k++) {
        int index = NUMBER_OF_LFO_OSC + NUMBER_OF_LFO_ENV+k;
        lfo[index]->setSynthState(this->synthState);
        lfo[index]->init(index, &this->matrix, (SourceEnum)(MATRIX_SOURCE_LFO5+k), (DestinationEnum)0);
    }


    for (int k = 0; k < MAX_NUMBER_OF_VOICES; k++) {
        voices[k].setSynthState(this->synthState);
        voices[k].init(&this->matrix, this->lfo, &this->env1, &this->env2,
                &this->env3, &this->env4, &this->env5, &this->env6,
                &this->osc1, &this->osc2, &this->osc3, &this->osc4,
                &this->osc5, &this->osc6);
    }

    this->recomputeNext = true;
    this->currentGate = 0;
    this->randomizerInt = 1253246184;
    this->holdPedal = false;
}

void Synth::noteOn(char note, char velocity) {
    if (note<24 || note >107) {
        return;
    }
    int numberOfNote = this->synthState->params.engine1.numberOfVoice;

    int zeroVelo = (16 - this->synthState->params.engine1.velocity) * 8;
    velocity = zeroVelo + velocity * (128 - zeroVelo) / 128;

    unsigned int indexMin = (unsigned int)2147483647;
    int voiceToUse = -1;

    for (int k = 0; k < numberOfNote; k++) {
        if (!voices[k].isPlaying()) {
            voices[k].noteOn(note, velocity, voiceIndex++);
            return;
        }
        // same note.... ?
        if (voices[k].getNote() == note) {
            voices[k].noteOnWithoutPop(note, velocity, voiceIndex++);
            return;
        }

        if (voices[k].isReleased()) {
            int indexVoice = voices[k].getIndex();
            if (indexVoice < indexMin) {
                indexMin = indexVoice;
                voiceToUse = k;
            }
        }
    }

    if (voiceToUse == -1) {
        for (int k = 0; k < numberOfNote; k++) {
            int indexVoice = voices[k].getIndex();
            if (indexVoice < indexMin) {
                indexMin = indexVoice;
                voiceToUse = k;
            }
        }
    }
    voices[voiceToUse].noteOnWithoutPop(note, velocity, voiceIndex++);
}

void Synth::noteOff(char note) {
	if (holdPedal) {
		return;
	}

    int numberOfNote = this->synthState->params.engine1.numberOfVoice;

    for (int k = 0; k < numberOfNote; k++) {
        if (voices[k].getNextNote() == 0) {
            if (voices[k].getNote() == note) {
                voices[k].noteOff();
            }
        } else {
            // if gliding and releasing first note
            if (voices[k].getNote() == note) {
                voices[k].glideNoteOff();
            }
            // if gliding and releasing next note
            if (voices[k].getNextNote() == note) {
                voices[k].glideToNote(voices[k].getNote());
                voices[k].glideNoteOff();
            }
        }
    }
}

void Synth::allNoteOff() {
    for (int k = 0; k < MAX_NUMBER_OF_VOICES; k++) {
        voices[k].noteOff();
    }
}

void Synth::allSoundOff() {
    for (int k = 0; k < MAX_NUMBER_OF_VOICES; k++) {
        voices[k].killNow();
    }
}

void Synth::setHoldPedal(int value) {
	if (value < 64) {
		holdPedal = false;
		allNoteOff();
	} else {
		holdPedal = true;
	}
}

bool Synth::isPlaying() {
    for (int k = 0; k < MAX_NUMBER_OF_VOICES; k++) {
        if (voices[k].isPlaying()) {
            return true;
        }
    }
    return false;
}

int* Synth::getSampleBlock() {
    return samples;
}


void Synth::nextSampleBlock() {

    this->lfo[0]->nextValueInMatrix();
    this->lfo[1]->nextValueInMatrix();
    this->lfo[2]->nextValueInMatrix();
    this->lfo[3]->nextValueInMatrix();
    this->lfo[4]->nextValueInMatrix();
    this->lfo[5]->nextValueInMatrix();

    this->matrix.computeAllFutureDestintationAndSwitch();

    // Update static members, can be called a ony voice
    this->voices[0].updateAllModulationIndexes();
    this->voices[0].updateAllMixOscs();
    // glide only appens on voice 0
    this->voices[0].glide();

    // update noise
    // http://software.intel.com/en-us/articles/fast-random-number-generator-on-the-intel-pentiumr-4-processor/
    int randomizer = rand();
    for (int k=0; k<32;) {
        randomizer = (214013 * randomizer + 2531011);
        noise[k++] = (randomizer & 0xffff) - 32768;
        randomizer = (214013 * randomizer + 2531011);
        noise[k++] = (randomizer & 0xffff) - 32768;
        randomizer = (214013 * randomizer + 2531011);
        noise[k++] = (randomizer & 0xffff) - 32768;
        randomizer = (214013 * randomizer + 2531011);
        noise[k++] = (randomizer & 0xffff) - 32768;
    }

    this->voices[0].nextBlock();
    this->voices[1].nextBlock();
    this->voices[2].nextBlock();
    this->voices[3].nextBlock();
    this->voices[4].nextBlock();
    this->voices[5].nextBlock();
    this->voices[6].nextBlock();
    this->voices[7].nextBlock();


    // Gate algo !!
    int gate = this->matrix.getDestination(MAIN_GATE) >> 4;
    for (int k=0; k<32; k++) {
        samples[k] = 0;

        if (currentGate < 256 && currentGate < gate) {
            currentGate ++;
        }
        if (currentGate > 0 && currentGate > gate) {
            currentGate --;
        }

        if (currentGate < 256) {
            samples[k] = voices[0].getSamples()[k];
            samples[k] += voices[1].getSamples()[k];
            samples[k] += voices[2].getSamples()[k];
            samples[k] += voices[3].getSamples()[k];
            samples[k] += voices[4].getSamples()[k];
            samples[k] += voices[5].getSamples()[k];
            samples[k] += voices[6].getSamples()[k];
            samples[k] += voices[7].getSamples()[k];
            samples[k] /= this->synthState->params.engine1.numberOfVoice;

            if (currentGate > 0) {
                // Shift all << 8 for better resolution in the calcul
                samples[k] =  ((samples[k] << 8) - (samples[k] * currentGate)) >> 8;
            }
        }
    }
}

void Synth::checkMaxVoice(boolean setEngineVoice) {
    int voiceMax = MAX_NUMBER_OF_VOICES;
    switch (showUp[this->synthState->params.engine1.algo].osc) {
    case 4:
        voices[6].killNow();
        voices[7].killNow();
        voiceMax = 6;
        break;
    case 6:
        voices[4].killNow();
        voices[5].killNow();
        voices[6].killNow();
        voices[7].killNow();
        voiceMax = 4;
        break;
    }
    allParameterRows.row[0]->params[ENCODER_ENGINE_VOICE].maxValue = voiceMax;

    if (setEngineVoice && this->synthState->params.engine1.numberOfVoice > 4) {
        // If not voic set to 0 then we set it to 4 when changing algorithm...
        // real propagate to send midi information
        this->synthState->setNewValue(ROW_ENGINE, ENCODER_ENGINE_VOICE, 4, true);
    }
}

void Synth::afterNewParamsLoad() {
    matrix.resetSources();
    matrix.resetAllDestination();
    env1.reloadADSR();
    env2.reloadADSR();
    env3.reloadADSR();
    env4.reloadADSR();
    env5.reloadADSR();
    env6.reloadADSR();
    for (int k=0; k<NUMBER_OF_LFO; k++) {
        lfo[k]->valueChanged(-1);
    }
    checkMaxVoice(false);
}


void Synth::newParamValue(SynthParamType type, int currentRow, int encoder, ParameterDisplay* param, int oldValue, int newValue) {
    if (type == SYNTH_PARAM_TYPE_ENGINE ) {
        if (currentRow == ROW_ENGINE) {
            if (encoder == ENCODER_ENGINE_ALGO) {
                checkMaxVoice(true);
            } else if (encoder == ENCODER_ENGINE_VOICE) {
                for (int k = newValue; k<MAX_NUMBER_OF_VOICES; k++) {
                    voices[k].killNow();
                }
            }
            /* DEBUG test hold pedal
            else if (encoder == ENCODER_ENGINE_VELOCITY) {
            	if (newValue == 3) {
            		setHoldPedal(0);
            	} else if (newValue == 4) {
            		setHoldPedal(127);
            	}
            }
            */
        } else if (currentRow == ROW_PERFORMANCE) {
            matrix.setSource((enum SourceEnum)(MATRIX_SOURCE_CC1 + encoder), newValue);
        }
    } else if (type == SYNTH_PARAM_TYPE_ENV) {
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
        // Reset old destination
        matrix.resetDestination(oldValue);
    } else if (type == SYNTH_PARAM_TYPE_LFO) {
        lfo[currentRow - ROW_LFO1]->valueChanged(encoder);
    }

}

