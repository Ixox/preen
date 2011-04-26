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

    void init(Matrix* matrix, Env<1>*env1, Env<2>*env2, Env<3>*env3, Env<4>*env4, Env<5>*env5, Env<6>*env6, Osc<1>*osc1, Osc<2>*osc2, Osc<3>*osc3, Osc<4>*osc4, Osc<5>*osc5, Osc<6>*osc6 );


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
				 	 '---'  '---'  |
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
                int freq = osc2->getNextSample(&oscState2);
                freq *= env2->getNextAmp(&envState2);
                freq >>= 19;
                freq *= oscState2.mainFrequency;
                freq >>= 15;
                freq *= IM1;

                //oscState3.frequency =  ((synthState.params.engine2.modulationFeedback * this->feedback)>>4) + oscState3.mainFrequency;
                int freq2 = osc3->getNextSample(&oscState3) ;
                freq2 *= env3->getNextAmp(&envState3);
                freq2 >>= 19;
                freq2 *= oscState3.mainFrequency;
                freq2 >>= 15;
                //this->feedback = freq2;
                freq2 *= IM2;

                oscState1.frequency =  freq + freq2 + oscState1.mainFrequency;
                currentSample =  osc1->getNextSample(&oscState1);
                currentSample *= env1->getNextAmp(&envState1);
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

                         .---.  ^
                         | 3 |  | Feedback
                         '---'  |
				           |
				       .------.
				       |IM1   |IM2
				 	 .---.  .---.
				 	 | 1 |  | 2 |
				 	 '---'  '---'
                       |Mix1  |Mix2
                 */
            {
                // FEEDBACK on osciallator 3
                // >> 3 for the modulationFeedBack and >> 4 to cut -32000/32000 to -2000 / 2000 (may have to be adjusted)
                //oscState3.frequency =  (synthState.params.engine2.modulationFeedback * this->feedback) + oscState3.mainFrequency;
                int freq = osc3->getNextSample(&oscState3) * env3->getNextAmp(&envState3);
                freq >>= 19;
                freq *= oscState3.mainFrequency;
                freq >>= 15;
                //this->feedback = freq;


                oscState2.frequency =  freq*IM2 + oscState2.mainFrequency;
                oscState1.frequency =  freq*IM1 + oscState1.mainFrequency;

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
                         .---.  ^
                         | 3 |  | Feedback
                         '---'  |
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
                // FEEDBACK on osciallator 3
                // >> 3 for the modulationFeedBack and >> 4 to cut -32000/32000 to -2000 / 2000 (may have to be adjusted)
                oscState3.frequency =  (synthState.params.engine2.modulationFeedback * this->feedback)+ oscState3.mainFrequency;
                int freq = osc3->getNextSample(&oscState3) * env3->getNextAmp(&envState3);
                freq >>= 19;
                freq *= oscState3.mainFrequency;
                freq >>= 15;
                this->feedback = freq;
                freq *= IM2;
                oscState2.frequency =  freq + oscState2.mainFrequency;
                freq = osc2->getNextSample(&oscState2) * env2->getNextAmp(&envState2);
                freq >>= 19;
                freq *= oscState2.mainFrequency; // Convertion in Hertz
                freq >>= 15;
                freq *= IM1;
                oscState1.frequency =  freq + oscState1.mainFrequency;
                currentSample = osc1->getNextSample(&oscState1)*env1->getNextAmp(&envState1);
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
				 	 .---.  .---. ^
				 	 | 3 |  | 4 | | Feedback
				 	 '---'  '---' |
                       |IM1   |IM2
				 	 .---.  .---.
				 	 | 1 |  | 2 |
				 	 '---'  '---'
                       |Mix1  |Mix2

                 */
            {
                int freq = osc3->getNextSample(&oscState3) * env3->getNextAmp(&envState3);
                freq >>= 19;
                freq *= oscState3.mainFrequency;
                freq >>= 15;
                freq *= IM1;

                oscState1.frequency =  freq + oscState1.mainFrequency;

                // FEEDBACK on oscillator 4
                // >> 3 for the modulationFeedBack and >> 4 to cut -32000/32000 to -2000 / 2000 (may have to be adjusted)
                oscState4.frequency =  (synthState.params.engine2.modulationFeedback * this->feedback) + oscState4.mainFrequency;
                freq = osc4->getNextSample(&oscState4) * env4->getNextAmp(&envState4);
                freq >>= 19;
                freq *= oscState4.mainFrequency;
                freq >>= 15;
                this->feedback = freq;
                freq *= IM2;

                oscState2.frequency =  freq + oscState2.mainFrequency;

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
				         .---. ^
				         | 4 | | Feedback
				         '---' |
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
                // FEEDBACK on oscillator 4
                // >> 3 for the modulationFeedBack and >> 4 to cut -32000/32000 to -2000 / 2000 (may have to be adjusted)
                oscState4.frequency =  (synthState.params.engine2.modulationFeedback * this->feedback) + oscState4.mainFrequency;

                int freq = osc4->getNextSample(&oscState4) * env4->getNextAmp(&envState4);
                freq >>= 19;
                freq *= oscState4.mainFrequency;
                freq >>= 15;
                this->feedback = freq;
                freq *= IM3;

                oscState3.frequency =  freq + oscState3.mainFrequency;
                freq = osc3->getNextSample(&oscState3) * env3->getNextAmp(&envState3);
                freq >>= 19;
                freq *= oscState3.mainFrequency;
                freq >>= 15;
                freq *= IM2;

                oscState2.frequency =  freq + oscState2.mainFrequency;
                freq = osc2->getNextSample(&oscState2) * env2->getNextAmp(&envState2);
                freq >>= 19;
                freq *= oscState2.mainFrequency; // Convertion in Hertz
                freq >>= 15;
                freq *= IM1;

                oscState1.frequency =  freq + oscState1.mainFrequency;
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
                                           |IM1
                 .---. ^          .---.  .---.
                 | 1 | | Feedback | 2 |  | 3 |
                 '---'            '---'  '---'
                   |Mix1            |Mix2  | Mix3

                 */
            {

                // FEEDBACK on oscillator 1
                // >> 3 for the modulationFeedBack and >> 4 to cut -32000/32000 to -2000 / 2000 (may have to be adjusted)
                oscState1.frequency =  (synthState.params.engine2.modulationFeedback * this->feedback) + oscState1.mainFrequency;
                int currentSample1 = osc1->getNextSample(&oscState1)*env1->getNextAmp(&envState1);

                this->feedback = currentSample1  >> 19;
                this->feedback *= oscState1.mainFrequency;
                this->feedback >>= 15;

                currentSample1  >>= 7; // 7 for mixOsc2
                currentSample1 *= MIX1;
                currentSample1  >>= 15;

                int currentSample2 = osc2->getNextSample(&oscState2)*env2->getNextAmp(&envState2);
                currentSample2  >>= 7; // 7 for mixOsc2
                currentSample2 *= MIX2;
                currentSample2  >>= 15;

                int freq = osc4->getNextSample(&oscState4) * env4->getNextAmp(&envState4);
                freq >>= 19;
                freq *= oscState4.mainFrequency;
                freq >>= 15;
                freq *= IM1;

                oscState3.frequency =  freq + oscState3.mainFrequency;
                currentSample = osc3->getNextSample(&oscState3)*env3->getNextAmp(&envState3);
                currentSample  >>= 7; // 7 for mixOsc2
                currentSample *= MIX3;
                currentSample  >>= 15;
                currentSample += currentSample2 + currentSample1;

                currentSample *= velocity;
                currentSample >>=7;
                currentSample /= 3;  // we added 3 samples
                if (env1->isDead(envState1) && env2->isDead(envState2) && env3->isDead(envState3)) {
                    endNoteOfBeginNextOne();
                }

                break;
            }
            case ALGO7:
                /*
                     .---.  .---.  .---. ^
                     | 2 |  | 4 |  | 6 | | Feedback
                     '---'  '---'  '---'
                       |IM1   |IM2   |IM3
                     .---.  .---.  .---.
                     | 1 |  | 3 |  | 5 |
                     '---'  '---'  '---'
                       |Mix1  |Mix2  |Mix3

                 */
            {

                int freq = osc2->getNextSample(&oscState2) * env2->getNextAmp(&envState2);
                freq >>= 19;
                freq *= oscState2.mainFrequency;
                freq >>= 15;
                freq *= IM1;

                oscState1.frequency =  freq + oscState1.mainFrequency;
                freq = osc4->getNextSample(&oscState4) * env4->getNextAmp(&envState4);
                freq >>= 19;
                freq *= oscState4.mainFrequency;
                freq >>= 15;
                freq *= IM2;

                oscState3.frequency =  freq + oscState3.mainFrequency;
                freq = osc6->getNextSample(&oscState6) * env6->getNextAmp(&envState6);
                freq >>= 19;
                freq *= oscState6.mainFrequency;
                freq >>= 15;
                freq *= IM3;

                oscState5.frequency =  freq + oscState5.mainFrequency;

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
            } // End switch
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
    void updateMixOsc1() {
        MIX1 = synthState.params.engine3.mixOsc1 + (matrix->getDestination(MIX_OSC1)>>4);
    }
    void updateMixOsc2() {
        MIX2 = synthState.params.engine3.mixOsc2 + (matrix->getDestination(MIX_OSC2)>>4);
    }
    void updateMixOsc3() {
        MIX3 = synthState.params.engine3.mixOsc3 + (matrix->getDestination(MIX_OSC3)>>4);
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
    static int MIX1, MIX2, MIX3;

    // feedback is per voice
    int feedback;

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

};

#endif


