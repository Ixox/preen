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

#include "Env.h"

void Env::setSynthState(SynthState* sState) {
	SynthStateAware::setSynthState(sState);
}

void Env::init(int number, Matrix* matrix) {
    this->destAttack = (DestinationEnum)(ENV1_ATTACK + number - 1);
    this->matrix = matrix;

    EnvelopeParams * e = (EnvelopeParams *)&(this->synthState->params.env1);
	envParams = &e[number-1];
	reloadADSR();
}


