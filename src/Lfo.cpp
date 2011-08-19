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

#include "Lfo.h"

Lfo::Lfo() {
	type = LFO_RAMP;
}

Lfo::~Lfo() {
}

void Lfo::init(int number, Matrix *matrix, SourceEnum source) {
	switch (number) {
	case 0:
		this->destination = LFO1_FREQ ;
		break;
	case 1:
		this->destination = LFO2_FREQ ;
		break;
	case 2:
		this->destination = LFO3_FREQ ;
		break;
	case 3:
		this->destination = LFO4_FREQ ;
		break;
	}
	this->matrix = matrix;
	this->source = source;
    this->ramp = 0;
    this->rampIndex = 0;
	LfoParams* lfoTmp = (LfoParams *)&this->synthState->params.lfo1;
	this->lfo = &lfoTmp[number];
	this->index = 0;
    this->rampIndex =  0;
    this->ramp = lfo->keybRamp << 4;

}
