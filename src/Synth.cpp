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
	setListenerType(SYNTH_PARAM_ENVELOPE_LISTENER);

}


Synth::~Synth(void)
{
}


void Synth::noteOn(char note, char velocity) {
	int freeNote = -1;
	for (int k=0; k<NUMBER_OF_VOICES && freeNote==-1; k++) {
		if (!voices[k].isPlaying() || voices[k].getNote()==note) {
			freeNote = k;
		}
	}
	// If no free note, take the older one
	if (freeNote==-1) {
		unsigned int indexMin =  4294967295;
		for (int k=0; k<NUMBER_OF_VOICES; k++) {
			if (voices[k].isReleased()) {
				indexMin = 0;
				freeNote = k;
			}
			if (voices[k].getIndex()<indexMin) {
				indexMin = voices[k].getIndex();
				freeNote = k;
			}
		}
	}
	voices[freeNote].noteOn(note, velocity, voiceIndex++);
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
	if ((cpt & 0x1c) == 0) {
		this->lfo[cpt & 3].nextValue();
	}

	// Update Matrix every 32 steps
	if ((cpt & 0x1f) == 4) {
		this->matrix.resetDestination();
		this->matrix.computeDestintation();
		// Update static members, can be called a ony voice
		this->voices[0].updateModulationIndex1();
		this->voices[0].updateModulationIndex2();
		this->voices[0].updateModulationIndex3();
	}

	// Compute sample
	for (int k=0; k<NUMBER_OF_VOICES; k++) {
		this->voices[k].nextSample();
	}
	cpt++;
}

