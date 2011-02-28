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

Voice::Voice(void)
{
}


Voice::~Voice(void)
{
}

void Voice::init(Matrix* matrix, Env<1>*env1, Env<2>*env2, Env<3>*env3, Env<4>*env4, Osc<1>*osc1, Osc<2>*osc2, Osc<3>*osc3, Osc<4>*osc4 ) {
	this->env1 = env1;
	this->env2 = env2;
	this->env3 = env3;
	this->env4 = env4;
	this->osc1 = osc1;
	this->osc2 = osc2;
	this->osc3 = osc3;
	this->osc4 = osc4;
	this->matrix = matrix;
	this->playing = false;
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

	this->released = false;
	this->playing = true;
	this->note = note;
	this->index = index;
	this->velocity = velocity;

}

void Voice::noteOff() {
	this->released = true;
	env1->noteOff(envState1);
	env2->noteOff(envState2);
	env3->noteOff(envState3);
	env4->noteOff(envState4);
}



