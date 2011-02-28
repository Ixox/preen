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
	this->matrix = matrix;
	this->source = source;
	this->number = number;
	LfoState* lfoTmp = (LfoState *)&currentSynthState.lfo1;
	lfo = &lfoTmp[number];
	index = 0;
}
