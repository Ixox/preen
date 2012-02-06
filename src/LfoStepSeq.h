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

enum {
	LFO_SEQ_MIDICLOCK_DIV_4 = 241,
	LFO_SEQ_MIDICLOCK_DIV_2,
	LFO_SEQ_MIDICLOCK,
	LFO_SEQ_MIDICLOCK_TIME_2,
	LFO_SEQ_MIDICLOCK_TIME_4,
};


class LfoStepSeq: public Lfo {
public:
	void init(int number, Matrix* matrix, SourceEnum source, DestinationEnum dest);
	void valueChanged(int encoder);
	void nextValueInMatrix();
	void noteOn();
	void noteOff();
	void midiClock(int songPosition, boolean computeStep);


private:
	StepSequencerParams* seqParams;
	StepSequencerSteps* seqSteps;

    int index;
    int step;
    int gateValue;
    int target;
    int currentValue;
    boolean gated;
    DestinationEnum matrixGateDestination;
};

#endif /* LFOENV_H_ */
