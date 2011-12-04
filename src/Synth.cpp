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

	osc1.setSynthState(this->synthState);
	osc2.setSynthState(this->synthState);
	osc3.setSynthState(this->synthState);
	osc4.setSynthState(this->synthState);
	osc5.setSynthState(this->synthState);
	osc6.setSynthState(this->synthState);
	osc1.init(&matrix);
	osc2.init(&matrix);
	osc3.init(&matrix);
	osc4.init(&matrix);
	osc5.init(&matrix);
	osc6.init(&matrix);

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

	cpt = 0;
}

void Synth::noteOn(char note, char velocity) {
	if (note<24 || note >107) {
		return;
	}
	int numberOfNote = this->synthState->params.engine1.numberOfVoice;

	int zeroVelo = (16 - this->synthState->params.engine1.velocity) * 8;
	velocity = zeroVelo + velocity * (128 - zeroVelo) / 128;

	int freeNote = -1;

	for (int k = 0; k < numberOfNote && freeNote == -1; k++) {
		if (!voices[k].isPlaying()) {
			freeNote = k;
		}
	}
	if (freeNote >= 0) {
		voices[freeNote].noteOn(note, velocity, voiceIndex++);
	} else {
		unsigned int indexMin = (unsigned int)4294967295;
		for (int k = 0; k < numberOfNote; k++) {
			if (voices[k].getNote() == note || voices[k].isReleased()) {
				indexMin = 0;
				freeNote = k;
			}
			if (voices[k].getIndex() < indexMin) {
				indexMin = voices[k].getIndex();
				freeNote = k;
			}
		}
		voices[freeNote].noteOnWithoutPop(note, velocity, voiceIndex++);
	}
}

void Synth::noteOff(char note) {
	for (int k = 0; k < MAX_NUMBER_OF_VOICES; k++) {
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

bool Synth::isPlaying() {
	for (int k = 0; k < MAX_NUMBER_OF_VOICES; k++) {
		if (voices[k].isPlaying()) {
			return true;
		}
	}
	return false;
}

int Synth::getSample() {
		int sample = voices[0].getSample();
		sample += voices[1].getSample();
		sample += voices[2].getSample();
		sample += voices[3].getSample();
		return sample / this->synthState->params.engine1.numberOfVoice;
}

void Synth::nextSample() {

	osc1.updateRandomNumber();

	// step 32 : update lfo1 , step 33 update lfo2 etc....
	int step32 = cpt & 0x1f;
	switch (step32) {
	case 0:
	case 1:
	case 2:
	case 3:
		this->lfo[step32]->nextValueInMatrix();
		break;
	case 4:
		// Value must be reset because the matrix only add value to destination.
		this->matrix.resetUsedFuturDestination();
		break;
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
		this->matrix.computeFutureDestintation(step32 - 5);
		break;
	case 13:
		this->matrix.useNewValues();
		break;
	case 14:
		// Update static members, can be called a ony voice
		this->voices[0].updateModulationIndex1();
		break;
	case 15:
		this->voices[0].updateModulationIndex2();
		break;
	case 16:
		this->voices[0].updateModulationIndex3();
		break;
	case 17:
		this->voices[0].updateModulationIndex4();
		break;
	case 18:
		this->voices[0].updateMixOsc1();
		break;
	case 19:
		this->voices[0].updateMixOsc2();
		break;
	case 20:
		this->voices[0].updateMixOsc3();
		break;
	case 21:
		this->voices[0].updateMixOsc4();
		break;
	case 22:
		// glide can happens only on Voice 0
		this->voices[0].glide();
		break;
	case 23:
	case 24:
	case 25:
	case 26:
		this->voices[step32 - 23].calculateFrequencyWithMatrix();
		break;
		//
	case 27:
		// update step sequencer
		this->lfoStepSeq[0].nextValueInMatrix();
		break;
	case 28:
		// update step sequencer
		this->lfoStepSeq[1].nextValueInMatrix();
		break;
	default:
		break;

	}

	for (int k=0; k<4; k++) {
		this->voices[k].nextSample();
	}

	cpt++;
}

void Synth::checkMaxVoice() {
	int voiceMax = MAX_NUMBER_OF_VOICES;
	switch (showUp[this->synthState->params.engine1.algo].osc) {
	case 4:
		voices[3].killNow();
		voiceMax = 3;
		break;
	case 6:
		voices[2].killNow();
		voices[3].killNow();
		voiceMax = 2;
		break;
	}
	if (this->synthState->params.engine1.numberOfVoice > voiceMax) {
		this->synthState->setNewValue(ROW_ENGINE, ENCODER_ENGINE_VOICE, voiceMax);
	}
	allParameterRows.row[0]->params[ENCODER_ENGINE_VOICE].maxValue = voiceMax;
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
	checkMaxVoice();
}


void Synth::newParamValue(SynthParamType type, int currentRow, int encoder, ParameterDisplay* param, int oldValue, int newValue) {
	if (type == SYNTH_PARAM_TYPE_ENGINE && encoder == ENCODER_ENGINE_ALGO) {
		checkMaxVoice();
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

