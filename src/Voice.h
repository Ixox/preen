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


    void nextSample() {
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
                int freq3 = osc3->getNextSample(&oscState3) * env3->getNextAmp(&envState3);
                freq3 >>= 19;
                freq3 *= oscState3.mainFrequency;
                freq3 >>= 15;

                oscState2.frequency =  freq3 * IM3 + oscState2.mainFrequency;

                int freq2 = osc2->getNextSample(&oscState2) * env2->getNextAmp(&envState2);
                freq2 >>= 19;
                freq2 *= oscState2.mainFrequency;
                freq2 >>= 15;

                oscState1.frequency =  IM1 * freq2 + IM2 * freq3 + oscState1.mainFrequency;
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
                int freq3 = osc3->getNextSample(&oscState3) * env3->getNextAmp(&envState3);
                freq3 >>= 19;
                freq3 *= oscState3.mainFrequency;
                freq3 >>= 15;

                oscState2.frequency =  freq3*IM2 + oscState2.mainFrequency;
                oscState1.frequency =  freq3*IM1 + oscState1.mainFrequency;

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


                int freq2 = osc2->getNextSample(&oscState2) * env2->getNextAmp(&envState2);
                freq2 >>= 19;
                freq2 *= oscState2.mainFrequency;
                freq2 >>= 15;
                freq2 *= IM1;

                int freq3 = osc3->getNextSample(&oscState3) * env3->getNextAmp(&envState3);
                freq3 >>= 19;
                freq3 *= oscState3.mainFrequency;
                freq3 >>= 15;
                oscState4.frequency =  freq3 * IM4 + oscState4.mainFrequency;
                freq3 *= IM2;

                int freq4 = osc4->getNextSample(&oscState4) * env4->getNextAmp(&envState4);
                freq4 >>= 19;
                freq4 *= oscState4.mainFrequency;
                freq4 >>= 15;
                freq4 *= IM3;

                oscState1.frequency =  freq2 + freq3 + freq4 + oscState1.mainFrequency;
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

                int freq4 = osc4->getNextSample(&oscState4) * env4->getNextAmp(&envState4);
                freq4 >>= 19;
                freq4 *= oscState4.mainFrequency;
                freq4 >>= 15;

                oscState3.frequency =  freq4 * IM4 + oscState3.mainFrequency;
                int freq3 = osc3->getNextSample(&oscState3) * env3->getNextAmp(&envState3);
                freq3 >>= 19;
                freq3 *= oscState3.mainFrequency;
                freq3 >>= 15;


                oscState1.frequency =  freq3*IM1 + oscState1.mainFrequency;
                oscState2.frequency =  freq4*IM2 +  freq3*IM3 + oscState2.mainFrequency;

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
                int freq4 = osc4->getNextSample(&oscState4) * env4->getNextAmp(&envState4);
                freq4 >>= 19;
                freq4 *= oscState4.mainFrequency;
                freq4 >>= 15;

                oscState3.frequency =  freq4 * IM3  + oscState3.mainFrequency;
                int freq3 = osc3->getNextSample(&oscState3) * env3->getNextAmp(&envState3);
                freq3 >>= 19;
                freq3 *= oscState3.mainFrequency;
                freq3 >>= 15;
                freq3 *= IM2;

                oscState2.frequency =  freq3 + freq4*IM4 + oscState2.mainFrequency;
                int freq2 = osc2->getNextSample(&oscState2) * env2->getNextAmp(&envState2);
                freq2 >>= 19;
                freq2 *= oscState2.mainFrequency; // Convertion in Hertz
                freq2 >>= 15;
                freq2 *= IM1;

                oscState1.frequency =  freq2 + oscState1.mainFrequency;
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

                int freq4 = osc4->getNextSample(&oscState4) * env4->getNextAmp(&envState4);
                freq4 >>= 19;
                freq4 *= oscState4.mainFrequency;
                freq4 >>= 15;

                oscState1.frequency =  freq4*IM1 + oscState1.mainFrequency;
                oscState2.frequency =  freq4*IM2 + oscState2.mainFrequency;
                oscState3.frequency =  freq4*IM3 + oscState3.mainFrequency;

                int currentSample3 = osc3->getNextSample(&oscState3)*env3->getNextAmp(&envState3);
                currentSample3  >>= 7; // 7 for mixOsc3
                currentSample3 *= MIX3;
                currentSample3  >>= 15;

                int currentSample2 = osc2->getNextSample(&oscState2)*env2->getNextAmp(&envState2);
                currentSample2  >>= 7; // 7 for mixOsc2
                currentSample2 *= MIX2;
                currentSample2  >>= 15;

                currentSample = osc1->getNextSample(&oscState1)*env1->getNextAmp(&envState1);
                currentSample  >>= 7; // 7 for mixOsc2
                currentSample *= MIX1;
                currentSample  >>= 15;
                currentSample += currentSample2 + currentSample3;

                currentSample *= velocity;
                currentSample >>=7; // >>7
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

                int freq2 = osc2->getNextSample(&oscState2) * env2->getNextAmp(&envState2);
                freq2 >>= 19;
                freq2 *= oscState2.mainFrequency;
                freq2 >>= 15;
                freq2 *= IM1;

                oscState1.frequency =  freq2 + oscState1.mainFrequency;

                int freq4 = osc4->getNextSample(&oscState4) * env4->getNextAmp(&envState4);
                freq4 >>= 19;
                freq4 *= oscState4.mainFrequency;
                freq4 >>= 15;

                // Use freq to modulate op6 by IM4.
                oscState6.frequency =  freq4 * IM4 + oscState6.mainFrequency;
                int freq6 = osc6->getNextSample(&oscState6) * env6->getNextAmp(&envState6);
                freq6 >>= 19;
                freq6 *= oscState6.mainFrequency;
                freq6 >>= 15;
                freq6 *= IM3;

                oscState5.frequency =  freq6 + oscState5.mainFrequency;
                oscState3.frequency =  freq4 * IM2 + oscState3.mainFrequency;


                int currentSample3 = osc5->getNextSample(&oscState5)*env5->getNextAmp(&envState5);
                currentSample3  >>= 7; // 7 for mixOsc3
                currentSample3 *= MIX3;
                currentSample3  >>= 15;

                int currentSample2 = osc3->getNextSample(&oscState3)*env3->getNextAmp(&envState3);
                currentSample2  >>= 7; // 7 for mixOsc2
                currentSample2 *= MIX2;
                currentSample2  >>= 15;

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

                int freq6 = osc6->getNextSample(&oscState6) * env6->getNextAmp(&envState6);
                freq6 >>= 19;
                freq6 *= oscState6.mainFrequency;
                freq6 >>= 15;
                freq6 *= IM4;

                oscState5.frequency =  freq6 + oscState5.mainFrequency;

                int currentSample2 = osc5->getNextSample(&oscState5)*env5->getNextAmp(&envState5);
                currentSample2  >>= 7; // 7 for mixOsc3
                currentSample2 *= MIX2;
                currentSample2  >>= 15;

                int freq4 = osc4->getNextSample(&oscState4) * env4->getNextAmp(&envState4);
                freq4 >>= 19;
                freq4 *= oscState4.mainFrequency;
                freq4 >>= 15;
                freq4 *= IM3;

                int freq3 = osc3->getNextSample(&oscState3) * env3->getNextAmp(&envState3);
                freq3 >>= 19;
                freq3 *= oscState3.mainFrequency;
                freq3 >>= 15;
                freq3 *= IM2;

                int freq2 = osc2->getNextSample(&oscState2) * env2->getNextAmp(&envState2);
                freq2 >>= 19;
                freq2 *= oscState2.mainFrequency;
                freq2 >>= 15;
                freq2 *= IM1;

                oscState1.frequency =  freq2 + freq3 + freq4 + oscState5.mainFrequency;

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
                int freq2 = osc2->getNextSample(&oscState2) * env2->getNextAmp(&envState2);
                freq2 >>= 19;
                freq2 *= oscState2.mainFrequency;
                freq2 >>= 15;
                freq2 *= IM1;

                int freq3 = osc3->getNextSample(&oscState3) * env3->getNextAmp(&envState3);
                freq3 >>= 19;
                freq3 *= oscState3.mainFrequency;
                freq3 >>= 15;
                freq3 *= IM2;

                oscState1.frequency =  freq2 + freq3 + oscState1.mainFrequency;
                int currentSample2 =  osc1->getNextSample(&oscState1) * env1->getNextAmp(&envState1);
                currentSample2  >>= 7; // 7 for MIX1
                currentSample2 *= MIX1;
                currentSample2  >>= 15;

                int freq6 = osc6->getNextSample(&oscState6) * env6->getNextAmp(&envState6);
                freq6 >>= 19;
                freq6 *= oscState6.mainFrequency;
                freq6 >>= 15;
                freq6 *= IM4;

                oscState5.frequency =  freq6 + oscState5.mainFrequency;
                int freq5 = osc5->getNextSample(&oscState5) * env5->getNextAmp(&envState5);
                freq5 >>= 19;
                freq5 *= oscState5.mainFrequency;
                freq5 >>= 15;
                freq5 *= IM3;

                oscState4.frequency =  freq5 +  oscState4.mainFrequency;

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


