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

Synth::Synth(void)
{
	osc1.init(&matrix);
	osc2.init(&matrix);
	osc3.init(&matrix);
	osc4.init(&matrix);

	env1.loadADSR();
	env2.loadADSR();
	env3.loadADSR();
	env4.loadADSR();

	for (int k=0; k<NUMBER_OF_LFOS; k++) {
		lfo[k].init(k, &this->matrix, (SourceEnum)(LFO1 + k));
	}
	for (int k=0; k<NUMBER_OF_VOICES; k++) {
		voices[k].init(&this->matrix, &this->env1, &this->env2, &this->env3, &this->env4, &this->osc1, &this->osc2, &this->osc3, &this->osc4);
	}
}


Synth::~Synth(void)
{
}


void Synth::noteOn(char note, char velocity) {
	int numberOfNote = NUMBER_OF_VOICES;
	if (synthState.params.engine.algo >= ALGO4) {
		numberOfNote --;
	}
	int freeNote = -1;

	for (int k=0; k<numberOfNote && freeNote==-1; k++) {
		if (!voices[k].isPlaying()) {
			freeNote = k;
		}
	}
	if (freeNote >= 0) {
		voices[freeNote].noteOn(note, velocity, voiceIndex++);
	} else {
		unsigned int indexMin =  4294967295;
		for (int k=0; k<numberOfNote; k++) {
			if (voices[k].getNote()==note || voices[k].isReleased()) {
				indexMin = 0;
				freeNote = k;
			}
			if (voices[k].getIndex()<indexMin) {
				indexMin = voices[k].getIndex();
				freeNote = k;
			}
		}
		voices[freeNote].noteOnWithoutPop(note, velocity, voiceIndex++);
	}
}

void Synth::noteOff(char note) {
	for (int k=0; k<NUMBER_OF_VOICES; k++) {
		if (voices[k].getNote() == note) {
			voices[k].noteOff();
		}
	}
}

void Synth::allNoteOff() {
	for (int k=0; k<NUMBER_OF_VOICES; k++) {
		voices[k].noteOff();
	}
}

bool Synth::isPlaying() {
	for (int k=0; k<NUMBER_OF_VOICES; k++) {
		if (voices[k].isPlaying()) {
			return true;
		}
	}
	return false;
}


int Synth::getSample() {
	int sample = 0;
	for (int k=0; k<NUMBER_OF_VOICES; k++) {
		sample += voices[k].getSample();
	}
	return sample / NUMBER_OF_VOICES;
}

void Synth::nextSample() {

	// step 32 : update lfo1 , step 33 update lfo2 etc....
		int step32 = cpt & 0x1f;
		switch (step32) {
			case 0:
			case 1:
			case 2:
			case 3:
				this->lfo[step32].nextValue();
				break;
			case 4:
				this->matrix.resetUsedDestination();
				break;
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
				this->matrix.computeFutureDestintation(step32 - 5);
				break;
			case 11:
				this->matrix.useNewValues();
				break;
			case 12:
				// Update static members, can be called a ony voice
				this->voices[0].updateModulationIndex1();
				break;
			case 13:
				this->voices[0].updateModulationIndex2();
				break;
			case 14:
				this->voices[0].updateModulationIndex3();
				break;
			default:
				break;

		}

		// Compute sample
	for (int k=0; k<NUMBER_OF_VOICES; k++) {
		this->voices[k].nextSample();
	}
	cpt++;
}

