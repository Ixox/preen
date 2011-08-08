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

void Voice::init(Matrix* matrix,  Lfo* lfo, Env<1>*env1, Env<2>*env2, Env<3>*env3, Env<4>*env4, Env<5>*env5, Env<6>*env6, Osc<1>*osc1, Osc<2>*osc2, Osc<3>*osc3, Osc<4>*osc4, Osc<5>*osc5, Osc<6>*osc6 ) {
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

    for (int k=0; k<NUMBER_OF_LFOS; k++) {
        lfo[k].resetRamp();
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
}

void Voice::killNow() {
    this->playing = false;
}



