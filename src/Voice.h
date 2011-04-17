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

#include "Matrix.h"
#include "Osc.h"
#include "Env.h"

extern int max;
extern int min;


class Voice
{
public:
	Voice();
	~Voice(void);

	void init(Matrix* matrix, Env<1>*env1, Env<2>*env2, Env<3>*env3, Env<4>*env4, Osc<1>*osc1, Osc<2>*osc2, Osc<3>*osc3, Osc<4>*osc4 );


	int getSample() {
		return currentSample;
	}


	void nextSample() {
		if (playing) {
			switch (synthState.params.engine1.algo) {
			case ALGO1:
				/*

				 	 .---.  .---.  ^
				 	 | 2 |  | 3 |  | Feedback
				 	 '---'  '---'  v
				       |IM1   |IM2
				       '------'
				           |
				         .---.
				         | 1 |
				         '---'

				 */
				{
				// FEEDBACK on osciallator 3
				// >> 3 for the modulationFeedBack and >> 4 to cut -32000/32000 to -2000 / 2000 (may have to be adjusted)
				oscState3.frequency =  ((synthState.params.engine2.modulationFeedback * this->feedback) >> 7) + oscState3.mainFrequency;
				osc2->nextSample(oscState2);
				env2->nextSample(envState2);
				osc3->nextSample(oscState3);
				env3->nextSample(envState3);

				int freq = osc2->getSample(oscState2) * env2->getAmp(envState2);
				freq >>= 19;
				freq *= oscState2.mainFrequency;
				freq >>= 15;
				freq *= IM1;

				int freq2 = osc3->getSample(oscState3) * env3->getAmp(envState3);
				freq2 >>= 19;
				freq2 *= oscState3.mainFrequency;
				freq2 >>= 15;
                this->feedback = freq2;
				freq2 *= IM2;
				oscState1.frequency =  freq + freq2 + oscState1.mainFrequency;
				env1->nextSample(envState1);
				osc1->nextSample(oscState1);
				currentSample = env1->getAmp(envState1); //*env1->getAmp(envState1);

				currentSample *= osc1->getSample(oscState1);
				currentSample  >>= 15;

				currentSample *= velocity;
				currentSample >>=7;

				if (env1->isDead(envState1)) {
					endNoteOfBeginNextOne();
				}
				break;
				}
			case ALGO2:
				/*
				         .---.
				         | 3 |
				         '---'
				           |
				       .------.
				       |IM1   |IM2
				 	 .---.  .---.
				 	 | 1 |  | 2 |
				 	 '---'  '---'
                       |Mix1  |Mix2
				 */
				{
				osc3->nextSample(oscState3);
				env3->nextSample(envState3);
				int freq = osc3->getSample(oscState3) * env3->getAmp(envState3);
				freq >>= 19;
				freq *= oscState3.mainFrequency;
				freq >>= 15;
				freq *= IM1;

				oscState2.frequency =  freq + oscState2.mainFrequency;
				env2->nextSample(envState2);
				osc2->nextSample(oscState2);

				oscState1.frequency =  freq + oscState1.mainFrequency;
				env1->nextSample(envState1);
				osc1->nextSample(oscState1);

				int currentSample2 = osc2->getSample(oscState2)*env2->getAmp(envState2);
                currentSample2  >>= 7; //  7 for mixOsc2
				currentSample2 *= synthState.params.engine3.mixOsc2;
				currentSample2  >>= 15;

				currentSample = osc1->getSample(oscState1)*env1->getAmp(envState1);
                currentSample  >>= 7; // 7 for mixOsc1
				currentSample *= synthState.params.engine3.mixOsc1;
				currentSample  >>= 15;
				currentSample += currentSample2;

				currentSample *= velocity;
				currentSample >>=8; // >>7 >> 1(we added 2 samples)
				if (env1->isDead(envState1) && env2->isDead(envState2)) {
					endNoteOfBeginNextOne();
				}
				}
				break;
			case ALGO3:
				/*
				         .---.
				         | 3 |
				         '---'
				           |IM2
				         .---.
				         | 2 |
				         '---'
				           |IM1
				         .---.
				         | 1 |
				         '---'

				 */
				{
				osc3->nextSample(oscState3);
				env3->nextSample(envState3);
				int freq = osc3->getSample(oscState3) * env3->getAmp(envState3);
				freq >>= 19;
				freq *= oscState3.mainFrequency;
				freq >>= 15;
				freq *= IM2;
				oscState2.frequency =  freq + oscState2.mainFrequency;
				osc2->nextSample(oscState2);
				env2->nextSample(envState2);
				freq = osc2->getSample(oscState2) * env2->getAmp(envState2);
				freq >>= 19;
				freq *= oscState2.mainFrequency; // Convertion in Hertz
				freq >>= 15;
				freq *= IM1;
				oscState1.frequency =  freq + oscState1.mainFrequency;
				env1->nextSample(envState1);
				osc1->nextSample(oscState1);
				currentSample = osc1->getSample(oscState1)*env1->getAmp(envState1);
				currentSample  >>= 15;
				currentSample *= velocity;
				currentSample >>=7;
				}
				if (env1->isDead(envState1)) {
					endNoteOfBeginNextOne();
				}
				break;
			case ALGO4:
				/*
				 	 .---.  .---.
				 	 | 2 |  | 4 |
				 	 '---'  '---'
                       |IM1   |IM2
				 	 .---.  .---.
				 	 | 1 |  | 2 |
				 	 '---'  '---'
                       |Mix1  |Mix2

				 */
				{
				osc3->nextSample(oscState3);
				env3->nextSample(envState3);
				int freq = osc3->getSample(oscState3) * env3->getAmp(envState3);
				freq >>= 19;
				freq *= oscState3.mainFrequency;
				freq >>= 15;
				freq *= IM1;

				oscState1.frequency =  freq + oscState1.mainFrequency;
				env1->nextSample(envState1);
				osc1->nextSample(oscState1);

				osc4->nextSample(oscState4);
				env4->nextSample(envState4);
				freq = osc4->getSample(oscState4) * env4->getAmp(envState4);
				freq >>= 19;
				freq *= oscState4.mainFrequency;
				freq >>= 15;
				freq *= IM2;

				oscState2.frequency =  freq + oscState2.mainFrequency;
				env2->nextSample(envState2);
				osc2->nextSample(oscState2);

				int currentSample2 = osc2->getSample(oscState2)*env2->getAmp(envState2);
                currentSample2  >>= 7; // 7 for mixOsc2
				currentSample2 *= synthState.params.engine3.mixOsc2;
				currentSample2  >>= 15;

				currentSample = osc1->getSample(oscState1)*env1->getAmp(envState1);
                currentSample  >>= 7; // 7 for mixOsc2
				currentSample *= synthState.params.engine3.mixOsc1;
				currentSample  >>= 15;
				currentSample += currentSample2;

				currentSample *= velocity;
				currentSample >>=8; // >>7 >> 1(we added 2 samples)
				if (env1->isDead(envState1) && env2->isDead(envState2)) {
					endNoteOfBeginNextOne();
				}


				}
				break;
			case ALGO5:
				/*
				         .---.
				         | 4 |
				         '---'
				           |IM3
				         .---.
				         | 3 |
				         '---'
				           |IM2
				         .---.
				         | 2 |
				         '---'
				           |IM1
				         .---.
				         | 1 |
				         '---'

				 */
				{
				osc4->nextSample(oscState4);
				env4->nextSample(envState4);
				int freq = osc4->getSample(oscState4) * env4->getAmp(envState4);
				freq >>= 19;
				freq *= oscState4.mainFrequency;
				freq >>= 15;
				freq *= IM3;

				oscState3.frequency =  freq + oscState3.mainFrequency;
				osc3->nextSample(oscState3);
				env3->nextSample(envState3);
				freq = osc3->getSample(oscState3) * env3->getAmp(envState3);
				freq >>= 19;
				freq *= oscState3.mainFrequency;
				freq >>= 15;
				freq *= IM2;

				oscState2.frequency =  freq + oscState2.mainFrequency;
				osc2->nextSample(oscState2);
				env2->nextSample(envState2);
				freq = osc2->getSample(oscState2) * env2->getAmp(envState2);
				freq >>= 19;
				freq *= oscState2.mainFrequency; // Convertion in Hertz
				freq >>= 15;
				freq *= IM1;

				oscState1.frequency =  freq + oscState1.mainFrequency;
				env1->nextSample(envState1);
				osc1->nextSample(oscState1);
				currentSample = osc1->getSample(oscState1)*env1->getAmp(envState1);
				currentSample  >>= 15;
				currentSample *= velocity;
				currentSample >>=7;
				}
				if (env1->isDead(envState1)) {
					endNoteOfBeginNextOne();
				}
				break;
			}
		}
	}


	void updateModulationIndex1() {
		IM1 = synthState.params.engine2.modulationIndex1 + (matrix->getDestination(INDEX_MODULATION1)>>4);
	}
	void updateModulationIndex2() {
		IM2 = synthState.params.engine2.modulationIndex2 + (matrix->getDestination(INDEX_MODULATION2)>>4);
	}
	void updateModulationIndex3() {
		IM3 = synthState.params.engine2.modulationIndex3 + (matrix->getDestination(INDEX_MODULATION3)>>4);
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
	void noteOff();
	void TESTASM();

	bool isReleased() { return this->released; }
	bool isPlaying() { return this->playing; }
	unsigned int getIndex() { return this->index; }
	char getNote() { return this->note; }

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
	static int IM1, IM2, IM3;
	// feedback is per voice
	int feedback;

	EnvData envState1;
	EnvData envState2;
	EnvData envState3;
	EnvData envState4;

	OscState oscState1;
	OscState oscState2;
	OscState oscState3;
	OscState oscState4;

	Osc<1>* osc1;
	Osc<2>* osc2;
	Osc<3>* osc3;
	Osc<4>* osc4;
	Env<1>* env1;
	Env<2>* env2;
	Env<3>* env3;
	Env<4>* env4;

	// Fixing the "plop" when all notes are buisy...
	bool newNotePending;
	char nextNote;
	char nextVelocity;
	unsigned int nextIndex;

};

#endif


