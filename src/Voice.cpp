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

#include "Voice.h"

int Voice::IM1, Voice::IM2, Voice::IM3, Voice::IM4;
int Voice::MIX1, Voice::MIX2, Voice::MIX3, Voice::MIX4;


Voice::Voice(void)
{
}


Voice::~Voice(void)
{
}

void Voice::init(Matrix* matrix,  Lfo** lfo, Env<1>*env1, Env<2>*env2, Env<3>*env3, Env<4>*env4, Env<5>*env5, Env<6>*env6, Osc<1>*osc1, Osc<2>*osc2, Osc<3>*osc3, Osc<4>*osc4, Osc<5>*osc5, Osc<6>*osc6 ) {
    this->env1 = env1;
    this->env2 = env2;
    this->env3 = env3;
    this->env4 = env4;
    this->env5 = env5;
    this->env6 = env6;
    this->osc1 = osc1;
    this->osc2 = osc2;
    this->osc3 = osc3;
    this->osc4 = osc4;
    this->osc5 = osc5;
    this->osc6 = osc6;
    this->matrix = matrix;
    this->playing = false;
    this->newNotePending = false;
    this->lfo = lfo;
}


void Voice::glideToNote(short newNote) {
    // Must glide...
    this->gliding = true;
    this->glideStep = 0;
    this->nextNote = newNote;
    osc1->glideToNote(oscState1, newNote);
    osc2->glideToNote(oscState2, newNote);
    osc3->glideToNote(oscState3, newNote);
    osc4->glideToNote(oscState4, newNote);
    if (showUp[this->synthState->params.engine1.algo].osc>4) {
        osc5->glideToNote(oscState5, newNote);
        osc6->glideToNote(oscState6, newNote);
    }
}

void Voice::noteOnWithoutPop(short newNote, char velocity, unsigned int index) {

    // Update index : so that few chance to be choosen again during the quick dying
    this->index = index;
    if (!this->released && this->synthState->params.engine1.numberOfVoice == 1 && this->synthState->params.engine1.glide>0) {
        glideToNote(newNote);
    } else {
        // update note now so that the noteOff is triggered by the new note
        this->note = newNote;
        // Quick dead !
        this->newNotePending = true;
        this->nextVelocity = velocity;
        this->nextNote = newNote;
        // Not release anymore... not available for new notes...
        this->released = false;
        env1->noteOffQuick(&envState1);
        env2->noteOffQuick(&envState2);
        env3->noteOffQuick(&envState3);
        env4->noteOffQuick(&envState4);
        if (showUp[this->synthState->params.engine1.algo].osc>4) {
            env5->noteOffQuick(&envState5);
            env6->noteOffQuick(&envState6);
        }
    }
}

void Voice::glide() {
    if (!this->gliding) {
        return;
    }
    this->glideStep++;
    if (glideStep<=(1<<this->synthState->params.engine1.glide)) {
        osc1->glideStep(oscState1, this->glideStep);
        osc2->glideStep(oscState2, this->glideStep);
        osc3->glideStep(oscState3, this->glideStep);
        osc4->glideStep(oscState4, this->glideStep);
        if (showUp[this->synthState->params.engine1.algo].osc>4) {
            osc5->glideStep(oscState5, this->glideStep);
            osc6->glideStep(oscState6, this->glideStep);
        }
    } else {
        this->gliding = false;
    }
}

void Voice::noteOn(short newNote, char velocity, unsigned int index) {
    osc1->newNote(oscState1, newNote);
    osc2->newNote(oscState2, newNote);
    osc3->newNote(oscState3, newNote);
    osc4->newNote(oscState4, newNote);

    env1->noteOn(envState1);
    env2->noteOn(envState2);
    env3->noteOn(envState3);
    env4->noteOn(envState4);

    if (showUp[this->synthState->params.engine1.algo].osc>4) {
        osc5->newNote(oscState5, newNote);
        osc6->newNote(oscState6, newNote);
        env5->noteOn(envState5);
        env6->noteOn(envState6);
    }

    this->released = false;
    this->playing = true;
    this->note = newNote;
    this->nextNote = 0;
    this->index = index;
    this->velocity = velocity;

    for (int k=0; k<NUMBER_OF_LFO; k++) {
        lfo[k]->noteOn();
    }
}

void Voice::glideNoteOff() {
    // while gliding the first note was released
    this->note = this->nextNote;
    this->nextNote = 0;
}

void Voice::noteOff() {
    this->released = true;
    this->nextNote = 0;
    this->gliding = false;
    env1->noteOff(&envState1);
    env2->noteOff(&envState2);
    env3->noteOff(&envState3);
    env4->noteOff(&envState4);
    env5->noteOff(&envState5);
    env6->noteOff(&envState6);
    for (int k=0; k<NUMBER_OF_LFO; k++) {
        lfo[k]->noteOff();
    }
}

void Voice::killNow() {
    this->playing = false;
    this->currentSample = 0;
}


void Voice::nextSample() {
    if (playing) {
        switch (this->synthState->params.engine1.algo) {
        case ALGO1:
            /*
                      IM3
                     <----
			 	 .---.  .---.
			 	 | 2 |  | 3 |
			 	 '---'  '---'
			       \IM1   /IM2
			         .---.
			         | 1 |
			         '---'

             */
        {
        	// Bitshifting to do :
        	// 31 (32 - 1)  + 3 (IM)= 35....
        	oscState3.frequency =  oscState3.mainFrequencyPlusMatrix;
        	int freq3 = osc3->getNextSample(&oscState3) * env3->getNextAmp(&envState3);
            freq3 >>= 18;
            freq3 *=  oscState3.frequency;
            freq3 >>= 13;

            oscState2.frequency =  ((freq3 * IM3) >> 3) + oscState2.mainFrequencyPlusMatrix;
            int freq2 = osc2->getNextSample(&oscState2) * env2->getNextAmp(&envState2);
            freq2 >>= 18;
            freq2 *= oscState2.frequency;
            freq2 >>= 13;

            oscState1.frequency =  ((IM1 * freq2) >> 3) + ((IM2 * freq3) >> 3) + oscState1.mainFrequencyPlusMatrix;
            currentSample =  osc1->getNextSample(&oscState1) * env1->getNextAmp(&envState1);
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
        	oscState3.frequency = oscState3.mainFrequencyPlusMatrix;
            int freq3 = osc3->getNextSample(&oscState3) * env3->getNextAmp(&envState3);
            freq3 >>= 18;
            freq3 *=  oscState3.frequency;
            freq3 >>= 13;

            oscState2.frequency =  ((freq3*IM2)>>3) + oscState2.mainFrequencyPlusMatrix;
            oscState1.frequency =  ((freq3*IM1)>>3) + oscState1.mainFrequencyPlusMatrix;

            int currentSample2 = osc2->getNextSample(&oscState2)*env2->getNextAmp(&envState2);
            currentSample2  >>= 7; //  7 for mixOsc2
            currentSample2 *= MIX2;
            currentSample2  >>= 15;

            currentSample = osc1->getNextSample(&oscState1)*env1->getNextAmp(&envState1);
            currentSample  >>= 7; // 7 for mixOsc1
            currentSample *= MIX1;
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

                           IM4
                          ---->
              .---.  .---.     .---.
              | 2 |  | 3 |     | 4 |
              '---'  '---'     '---'
                 \IM1  |IM2    /IM3
                     .---.
                     | 1 |
                     '---'
             */
        {


        	oscState2.frequency = oscState2.mainFrequencyPlusMatrix;
            int freq2 = osc2->getNextSample(&oscState2) * env2->getNextAmp(&envState2);
            freq2 >>= 18;
            freq2 *=  oscState2.frequency;
            freq2 >>= 13;

        	oscState3.frequency = oscState3.mainFrequencyPlusMatrix;
            int freq3 = osc3->getNextSample(&oscState3) * env3->getNextAmp(&envState3);
            freq3 >>= 18;
            freq3 *=  oscState3.frequency;
            freq3 >>= 13;

            oscState4.frequency =  ((freq3 * IM4) >> 3) + oscState4.mainFrequencyPlusMatrix;
            int freq4 = osc4->getNextSample(&oscState4) * env4->getNextAmp(&envState4);
            freq4 >>= 18;
            freq4 *=  oscState4.frequency;
            freq4 >>= 13;

            oscState1.frequency =  ((freq2 * IM1) >> 3) + ((freq3 * IM2)>> 3) + ((freq4 * IM3) >> 3) + oscState1.mainFrequencyPlusMatrix;
            currentSample = osc1->getNextSample(&oscState1)*env1->getNextAmp(&envState1);
            currentSample  >>= 15;
            currentSample *= velocity;
            currentSample >>=7;

            if (env1->isDead(envState1)) {
                endNoteOfBeginNextOne();
            }


            break;
        }
        case ALGO4:
            /*           IM4
			 	  .---. <----   .---.
			 	  | 3 |         | 4 |
			 	  '---'         '---'
                   |IM1 \IM3     |IM2
		 	 	 .---.          .---.
			 	 | 1 |          | 2 |
			 	 '---'          '---'
                   |Mix1          |Mix2

             */
        {

            oscState4.frequency =  oscState4.mainFrequencyPlusMatrix;
            int freq4 = osc4->getNextSample(&oscState4) * env4->getNextAmp(&envState4);
            freq4 >>= 18;
            freq4 *=  oscState4.frequency;
            freq4 >>= 13;

            oscState3.frequency =  ((freq4 * IM4) >> 3) + oscState3.mainFrequencyPlusMatrix;
            int freq3 = osc3->getNextSample(&oscState3) * env3->getNextAmp(&envState3);
            freq3 >>= 18;
            freq3 *=  oscState3.frequency;
            freq3 >>= 13;

            oscState1.frequency =  ((freq3*IM1)>>3) + oscState1.mainFrequencyPlusMatrix;
            oscState2.frequency =  ((freq4*IM2)>>3) +  ((freq3*IM3)>>3) + oscState2.mainFrequencyPlusMatrix;

            int currentSample2 = osc2->getNextSample(&oscState2)*env2->getNextAmp(&envState2);
            currentSample2  >>= 7; // 7 for mixOsc2
            currentSample2 *= MIX2;
            currentSample2  >>= 15;

            currentSample = osc1->getNextSample(&oscState1)*env1->getNextAmp(&envState1);
            currentSample  >>= 7; // 7 for mixOsc2
            currentSample *= MIX1;
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
			         '---'  \
			           |IM3  |
			         .---.   |
			         | 3 |   | IM4
			         '---'   |
			           |IM2  |
			         .---.  /
			         | 2 |
			         '---'
			           |IM1
			         .---.
			         | 1 |
			         '---'

             */
        {
            oscState4.frequency =  oscState4.mainFrequencyPlusMatrix;
            int freq4 = osc4->getNextSample(&oscState4) * env4->getNextAmp(&envState4);
            freq4 >>= 18;
            freq4 *=  oscState4.frequency;
            freq4 >>= 13;

            oscState3.frequency =  ((freq4 * IM3)>>3)  + oscState3.mainFrequencyPlusMatrix;
            int freq3 = osc3->getNextSample(&oscState3) * env3->getNextAmp(&envState3);
            freq3 >>= 18;
            freq3 *=  oscState3.frequency;
            freq3 >>= 13;

            oscState2.frequency =  ((freq3 * IM2)>>3) + ((freq4*IM4)>>3) + oscState2.mainFrequencyPlusMatrix;
            int freq2 = osc2->getNextSample(&oscState2) * env2->getNextAmp(&envState2);
            freq2 >>= 18;
            freq2 *=  oscState2.frequency;
            freq2 >>= 13;

            oscState1.frequency =  ((freq2 * IM1)>>3) + oscState1.mainFrequencyPlusMatrix;
            currentSample = osc1->getNextSample(&oscState1)*env1->getNextAmp(&envState1);
            currentSample  >>= 15;
            currentSample *= velocity;
            currentSample >>=7;
            if (env1->isDead(envState1)) {
                endNoteOfBeginNextOne();
            }
        }
        break;
        case ALGO6:
            /*
                    .---.
                    | 4 |
                    '---'
                 /IM1 |IM2 \IM3
             .---.  .---.  .---.
             | 1 |  | 2 |  | 3 |
             '---'  '---'  '---'
               |Mix1  |Mix2  | Mix3

             */
        {
            oscState4.frequency =  oscState4.mainFrequencyPlusMatrix;
            int freq4 = osc4->getNextSample(&oscState4) * env4->getNextAmp(&envState4);
            freq4 >>= 18;
            freq4 *=  oscState4.frequency;
            freq4 >>= 13;

            oscState3.frequency =  ((freq4*IM3)>>3) + oscState3.mainFrequencyPlusMatrix;

            int currentSample3 = osc3->getNextSample(&oscState3)*env3->getNextAmp(&envState3);
            currentSample3  >>= 15;
            currentSample3 *= MIX3;

            oscState2.frequency =  ((freq4*IM2)>>3) + oscState2.mainFrequencyPlusMatrix;

            int currentSample2 = osc2->getNextSample(&oscState2)*env2->getNextAmp(&envState2);
            currentSample2  >>= 15;
            currentSample2 *= MIX2;

            oscState1.frequency =  ((freq4*IM1)>>3) + oscState1.mainFrequencyPlusMatrix;

            currentSample = osc1->getNextSample(&oscState1)*env1->getNextAmp(&envState1);
            currentSample  >>= 15;
            currentSample *= MIX1;

            currentSample += currentSample2 + currentSample3;

            currentSample *= velocity;
            currentSample >>=14; // >>7 + 7 (MIX)
            currentSample /=3; // 3 samples !!

            if (env1->isDead(envState1) && env2->isDead(envState2)&& env3->isDead(envState3)) {
                endNoteOfBeginNextOne();
            }


            break;
        }
        case ALGO7:
            /*
                            IM4
                           ---->
                 .---.  .---.  .---.
                 | 2 |  | 4 |  | 6 |
                 '---'  '---'  '---'
                   |IM1   |IM2   |IM3
                 .---.  .---.  .---.
                 | 1 |  | 3 |  | 5 |
                 '---'  '---'  '---'
                   |Mix1  |Mix2  |Mix3

             */
        {
        	oscState2.frequency =  oscState2.mainFrequencyPlusMatrix;
            int freq2 = osc2->getNextSample(&oscState2) * env2->getNextAmp(&envState2);
            freq2 >>= 18;
            freq2 *=  oscState2.frequency;
            freq2 >>= 13;

            oscState4.frequency =  oscState4.mainFrequencyPlusMatrix;

            int freq4 = osc4->getNextSample(&oscState4) * env4->getNextAmp(&envState4);
            freq4 >>= 18;
            freq4 *=  oscState4.frequency;
            freq4 >>= 13;

            // Use freq4 to modulate op6 by IM4.
            oscState6.frequency =  ((freq4 * IM4)>>3) + oscState6.mainFrequencyPlusMatrix;

            int freq6 = osc6->getNextSample(&oscState6) * env6->getNextAmp(&envState6);
            freq6 >>= 18;
            freq6 *=  oscState6.frequency;
            freq6 >>= 13;

            oscState5.frequency =  ((freq6 * IM3)>>3)+ oscState5.mainFrequencyPlusMatrix;
            int currentSample3 = osc5->getNextSample(&oscState5)*env5->getNextAmp(&envState5);
            currentSample3  >>= 7; // 7 for mixOsc3
            currentSample3 *= MIX3;
            currentSample3  >>= 15;

            oscState3.frequency =  ((freq4 * IM2)>>3) + oscState3.mainFrequencyPlusMatrix;
            int currentSample2 = osc3->getNextSample(&oscState3)*env3->getNextAmp(&envState3);
            currentSample2  >>= 7; // 7 for mixOsc2
            currentSample2 *= MIX2;
            currentSample2  >>= 15;

            oscState1.frequency =  ((freq2 *IM1)>>3) + oscState1.mainFrequencyPlusMatrix;

            currentSample = osc1->getNextSample(&oscState1)*env1->getNextAmp(&envState1);
            currentSample  >>= 7; // 7 for mixOsc2
            currentSample *= MIX1;
            currentSample  >>= 15;
            currentSample += currentSample2 + currentSample3;

            currentSample *= velocity;
            currentSample >>=7; // >>7
            currentSample /=3; // 3 samples !!
            if (env1->isDead(envState1) && env3->isDead(envState3)&& env5->isDead(envState5)) {
                endNoteOfBeginNextOne();
            }
        }
        break;
        case ALGO8:
            /*
              .---.  .---.  .---.       .---.
              | 2 |  | 3 |  | 4 |       | 6 |
              '---'  '---'  '---'       '---'
                 \IM1  |IM2  /IM3         | IM4
                     .---.              .---.
                     | 1 |              | 5 |
                     '---'              '---'
                       |Mix1              | Mix2

             */
        {
        	oscState6.frequency =  oscState6.mainFrequencyPlusMatrix;
        	int freq6 = osc6->getNextSample(&oscState6) * env6->getNextAmp(&envState6);
            freq6 >>= 18;
            freq6 *=  oscState6.frequency;
            freq6 >>= 13;

            oscState4.frequency =  oscState4.mainFrequencyPlusMatrix;
            int freq4 = osc4->getNextSample(&oscState4) * env4->getNextAmp(&envState4);
            freq4 >>= 18;
            freq4 *=  oscState4.frequency;
            freq4 >>= 13;

            oscState3.frequency =  oscState3.mainFrequencyPlusMatrix;
            int freq3 = osc3->getNextSample(&oscState3) * env3->getNextAmp(&envState3);
            freq3 >>= 18;
            freq3 *=  oscState3.frequency;
            freq3 >>= 13;

            oscState2.frequency =  oscState2.mainFrequencyPlusMatrix;
            int freq2 = osc2->getNextSample(&oscState2) * env2->getNextAmp(&envState2);
            freq2 >>= 18;
            freq2 *=  oscState2.frequency;
            freq2 >>= 13;

            oscState5.frequency =  ((freq6 * IM4)>>3) + oscState5.mainFrequencyPlusMatrix;
            int currentSample2 = osc5->getNextSample(&oscState5)*env5->getNextAmp(&envState5);
            currentSample2  >>= 7; // 7 for mixOsc3
            currentSample2 *= MIX2;
            currentSample2  >>= 15;

            oscState1.frequency =  ((freq2 *IM1)>>3) + ((freq3 *IM2)>>3) + ((freq4 *IM3)>>3) + oscState1.mainFrequencyPlusMatrix;
            currentSample = osc1->getNextSample(&oscState1)*env1->getNextAmp(&envState1);
            currentSample  >>= 7; // 7 for mixOsc2
            currentSample *= MIX1;
            currentSample  >>= 15;
            currentSample += currentSample2;

            currentSample *= velocity;
            currentSample >>=8; // >>7 + >> 1 (2 samples)

            if (env1->isDead(envState1) && env5->isDead(envState5)) {
                endNoteOfBeginNextOne();
            }


            break;
        }
        case ALGO9:
            /*
                                    .---.
                                    | 6 |
                                    '---'
                                      |IM4
             .---.      .---.       .---.
             | 2 |      | 3 |       | 5 |
             '---'      '---'       '---'
                 \IM1    /IM2         | IM3
                   .---.            .---.
                   | 1 |            | 4 |
                   '---'            '---'
                     |Mix1            | Mix2

             */
        {
        	oscState2.frequency =  oscState2.mainFrequencyPlusMatrix;
            int freq2 = osc2->getNextSample(&oscState2) * env2->getNextAmp(&envState2);
            freq2 >>= 18;
            freq2 *=  oscState2.frequency;
            freq2 >>= 13;

            oscState3.frequency =  oscState3.mainFrequencyPlusMatrix;
            int freq3 = osc3->getNextSample(&oscState3) * env3->getNextAmp(&envState3);
            freq3 >>= 18;
            freq3 *=  oscState3.frequency;
            freq3 >>= 13;

            oscState1.frequency =  ((freq2*IM1)>>3) + ((freq3*IM2)>>3) + oscState1.mainFrequencyPlusMatrix;
            int currentSample2 =  osc1->getNextSample(&oscState1) * env1->getNextAmp(&envState1);
            currentSample2  >>= 7; // 7 for MIX1
            currentSample2 *= MIX1;
            currentSample2  >>= 15;

            oscState6.frequency =  oscState6.mainFrequencyPlusMatrix;
            int freq6 = osc6->getNextSample(&oscState6) * env6->getNextAmp(&envState6);
            freq6 >>= 18;
            freq6 *=  oscState6.frequency;
            freq6 >>= 13;

            oscState5.frequency =  ((freq6 * IM4)>>3) + oscState5.mainFrequencyPlusMatrix;
            int freq5 = osc5->getNextSample(&oscState5) * env5->getNextAmp(&envState5);
            freq5 >>= 18;
            freq5 *=  oscState5.frequency;
            freq5 >>= 13;

            oscState4.frequency =  ((freq5 * IM3)>>3) +  oscState4.mainFrequencyPlusMatrix;

            currentSample = osc4->getNextSample(&oscState4)*env4->getNextAmp(&envState4);
            currentSample  >>= 7; // 7 for mixOsc2
            currentSample *= MIX2;
            currentSample  >>= 15;
            currentSample += currentSample2;

            currentSample *= velocity;
            currentSample >>=8; // >>7 + >> 1 (2 samples)

            if (env1->isDead(envState1) && env4->isDead(envState4)) {
                endNoteOfBeginNextOne();
            }


            break;
        }
        } // End switch
    }
}



