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



void LfoStepSeq::init(int number, Matrix *matrix, SourceEnum source, DestinationEnum dest) {
	Lfo::init(number, matrix, source, dest);
	if (source == MATRIX_SOURCE_LFO5) {
		this->stepParams = (StepSequencerParams *)&this->synthState->params.lfo5;
	} else {
		this->stepParams = (StepSequencerParams *)&this->synthState->params.lfo6;
	}
	gated = false;
}


void LfoStepSeq::valueChanged(int encoder) {
	if (encoder < 2) {
		step = (stepParams->bpm << 16) / 15360;
		gateValue = stepParams->gate<< 11;
	}
}

void LfoStepSeq::nextValueInMatrix() {
  	 // We're called 1024 times per seconds
  	 // Each step must last (1024 / 4 * 60/bpm)
  	 // = 64*60/bmp = 15360 / bmp
  	 // We must going forward bpm / 15360
  	 // fix point stepForward = (bpm << 16) / 15360;

  	 index += step;
  	 index &= 0xfffff;

   	// We'll reach the new value step by step to reduice audio click !

  	 if (stepParams->gate < 32) {
		 // Gated ?
		 if (!gated && ((index & 0xffff) >= gateValue)) {
			target = 0;
			gated = true;
		 }
		// End of gate ?
		 if (gated && ((index & 0xffff) < gateValue)) {
			target = stepParams->steps[index>>16];
			gated = false;
		 }
  	 } else {
  		 target = stepParams->steps[index>>16];
  	 }

   	if (currentValue < target) {
       	currentValue ++;
   	}
   	if (currentValue > target) {
       	currentValue --;
   	}

  	 matrix->setSource(source, currentValue << 4);}

void LfoStepSeq::noteOn() {
	index = 0;
}

void LfoStepSeq::noteOff() {
}

