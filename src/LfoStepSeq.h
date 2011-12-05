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

#ifndef LFOSTEPSEQ_H_
#define LFOSTEPSEQ_H_

#include "Lfo.h"



class LfoStepSeq: public Lfo {
public:

	void init(int number, Matrix* matrix, SourceEnum source, DestinationEnum dest);

	void valueChanged(int encoder) {
		if (encoder < 2) {
			step = (stepParams->bpm << 16) / 15360;
			gate = stepParams->gate << 11;
		}
	}

	void nextValueInMatrix() {
		// We're called 1024 times per seconds
		// Each step must last (1024 / 4 * 60/bpm)
		// = 64*60/bmp
		// We must going forward bpm / 15360
		// fix point stepForward = (bpm << 16) / 15360;

		index += step;
		index &= 0xfffff;

		// Gated
		if ((index & 0xffff) >= gate) {
			matrix->setSource(source, 0);
		} else {
			matrix->setSource(source, stepParams->steps[index>>16]<<4);
		}
	}

	void noteOn() {
		index = 0;
	}

	void noteOff() {
	}


private:
	StepSequencerParams* stepParams;
    unsigned int index;
    unsigned int step;
    unsigned int gate;
};

#endif /* LFOENV_H_ */
