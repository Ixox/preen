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

int Voice::IM1, Voice::IM2, Voice::IM3;
int Voice::MIX1, Voice::MIX2, Voice::MIX3;

int vvcpt=0;

Voice::Voice(void)
{
}


Voice::~Voice(void)
{
}

void Voice::init(Matrix* matrix, Env<1>*env1, Env<2>*env2, Env<3>*env3, Env<4>*env4, Env<5>*env5, Env<6>*env6, Osc<1>*osc1, Osc<2>*osc2, Osc<3>*osc3, Osc<4>*osc4, Osc<5>*osc5, Osc<6>*osc6 ) {
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
}


void Voice::noteOnWithoutPop(short note, char velocity, unsigned int index) {
    this->nextNote = note;
    this->nextVelocity = velocity;
    // Update index : so that few change to be choosen again during the quick dying
    this->index = index;

    this->newNotePending = true;

    env1->noteOffQuick(&envState1);
    env2->noteOffQuick(&envState2);
    env3->noteOffQuick(&envState3);
    env4->noteOffQuick(&envState4);
    if (showUp[synthState.params.engine1.algo].osc>4) {
        env5->noteOffQuick(&envState5);
        env6->noteOffQuick(&envState6);
    }
}

void Voice::noteOn(short note, char velocity, unsigned int index) {
    osc1->newNote(oscState1, note);
    osc2->newNote(oscState2, note);
    osc3->newNote(oscState3, note);
    osc4->newNote(oscState4, note);
    env1->noteOn(envState1);
    env2->noteOn(envState2);
    env3->noteOn(envState3);
    env4->noteOn(envState4);

    if (showUp[synthState.params.engine1.algo].osc>4) {
        osc5->newNote(oscState5, note);
        osc6->newNote(oscState6, note);
        env5->noteOn(envState5);
        env6->noteOn(envState6);
    }

    this->released = false;
    this->playing = true;
    this->note = note;
    this->index = index;
    this->velocity = velocity;
}

void Voice::noteOff() {
    this->released = true;
    env1->noteOff(&envState1);
    env2->noteOff(&envState2);
    env3->noteOff(&envState3);
    env4->noteOff(&envState4);
    env5->noteOff(&envState5);
    env6->noteOff(&envState6);
}




