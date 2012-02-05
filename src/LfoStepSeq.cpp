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

#include "LfoStepSeq.h"


int expValues[] = { 0, 15, 31, 48, 67, 86, 106, 128, 150, 175, 200, 227, 256,
		286, 319, 353 };

void LfoStepSeq::init(int number, Matrix *matrix, SourceEnum source,
		DestinationEnum dest) {
	Lfo::init(number, matrix, source, dest);
	switch (source) {
	case MATRIX_SOURCE_LFO5:
		this->seqParams =
				(StepSequencerParams *) &this->synthState->params.lfo5;
		this->seqSteps = (StepSequencerSteps*) &this->synthState->params.steps5;
		this->matrixGateDestination = LFO5_GATE;
		break;
	case MATRIX_SOURCE_LFO6:
		this->seqParams =
				(StepSequencerParams *) &this->synthState->params.lfo6;
		this->seqSteps = (StepSequencerSteps*) &this->synthState->params.steps6;
		this->matrixGateDestination = LFO6_GATE;
		break;
	}
	gated = false;
	ticks = 0;
}


void LfoStepSeq::midiClock(int songPosition) {
	switch (seqParams->bpm)  {
	case LFO_SEQ_MIDICLOCK_DIV_4:
		// Midi Clock  / 4
		if ((songPosition & 0x3)==0) {
			step = 0xffff / ticks;
			ticks = 0;
			index = ((songPosition >> 2)& 0xf) << 16;
		}
		break;
	case LFO_SEQ_MIDICLOCK_DIV_2:
		if ((songPosition & 0x3)==0) {
			step = 0x1ffff / ticks;
			ticks = 0;
			index = ((songPosition >> 1) & 0xf) << 16;
		}
		break;
	case LFO_SEQ_MIDICLOCK:
		if ((songPosition & 0x3)==0) {
			step = 0x3ffff / ticks;
			ticks = 0;
			index = (songPosition& 0xf) << 16;
		}
		break;
	case LFO_SEQ_MIDICLOCK_TIME_2:
		if ((songPosition & 0x3)==0) {
			step = 0x7ffff / ticks;
			ticks = 0;
			index = ((songPosition * 2) & 0xf) << 16;
		}
		break;
	case LFO_SEQ_MIDICLOCK_TIME_4:
		if ((songPosition & 0x3)==0) {
			step = 0xfffff / ticks;
			ticks = 0;
			index = ((songPosition * 4) & 0xf) << 16;
		}
		break;
	}
}


void LfoStepSeq::valueChanged(int encoder) {
	if (encoder < 2) {
		if (seqParams->bpm <= 240 ) {
			// We're called 1024 times per seconds
			// Each step must last (1024 / 4 * 60/bpm)
			// = 64*60/bmp = 15360 / bmp
			// We must going forward bpm / 15360
			// fix point stepForward = (bpm << 16) / 15360;

			step = (seqParams->bpm << 16) / 15360;
		}
	}
}

void LfoStepSeq::nextValueInMatrix() {

	ticks++;

	index += step;
	index &= 0xfffff;

	// Add gate and matrix value
	int gatePlusMatrix = seqParams->gate + (this->matrix->getDestination(matrixGateDestination) >> 7);

	// We'll reach the new value step by step to reduce audio click !
	if (gatePlusMatrix <= 0) {
		target = 0;
	} else if (gatePlusMatrix < 32) {
		// Gated ?
		if (!gated && ((index & 0xffff) >= (gatePlusMatrix << 11))) {
			target = 0;
			gated = true;
		}
		// End of gate ?
		if (gated && ((index & 0xffff) < (gatePlusMatrix << 11))) {
			target = seqSteps->steps[index >> 16];
			gated = false;
		}
	} else {
		target = seqSteps->steps[index >> 16];
	}

	if (currentValue < target) {
		currentValue++;
	}
	if (currentValue > target) {
		currentValue--;
	}

	matrix->setSource(source, expValues[currentValue]);
}

void LfoStepSeq::noteOn() {
	index = 0;
	target = seqSteps->steps[0];
}

void LfoStepSeq::noteOff() {
}

