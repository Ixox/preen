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

#ifndef LFOOSC_H_
#define LFOOSC_H_

#include "Lfo.h"

class LfoOsc: public Lfo {
public:

	void init(int number, Matrix* matrix, SourceEnum source, DestinationEnum dest);

	void valueChanged(int encoder) {
	    ramp = lfo->keybRamp << 4; // * 16
	}

	void nextValueInMatrix() {
	     int lfoValue = 0;

		// then new value
		//	index = (index +  ((lfo->freq << 16) / LFO_SAMPLE_RATE_x_8 ))  & 0xffff;
		//		int jmp = lfo->freq	<< 3 ; // << 16 >> 13
	    int realfreq = lfo->freq + (this->matrix->getDestination(destination) >> 7);

		switch (lfo->shape) {
		case LFO_RAMP:
			index = (index + (realfreq << 3)) & 0xffff;
			lfoValue = (index>>8)-128;
			break;
		case LFO_SAW:
		{
			index = (index + (realfreq << 3)) & 0xffff;
			if (index < 32768) {
			    lfoValue = (index>>7) - 128;
			} else {
			    lfoValue = 383 - (index>>7);
			}
			break;
		}
		case LFO_SQUARE:
			index = (index + (realfreq << 3)) & 0xffff;
			if ((index) < 32768) {
			    lfoValue = -128;
			} else {
                lfoValue = 127;
			}
			break;

		case LFO_RANDOM:
			index = (index + (realfreq << 3));
			if (index > 0xffff) {
				 index &= 0xffff;
				 currentRandomValue = (randomOsc >> 8);
			}
			lfoValue = currentRandomValue;
			break;
		}

		lfoValue += lfo->bias;

		if (rampIndex < ramp) {
		    lfoValue = lfoValue * rampIndex  / ramp ;
            rampIndex ++;
		}

		matrix->setSource(source, lfoValue);
	}

	void noteOn() {
        if (ramp > 0) {
            index = 0;
            rampIndex = 0;
        } else {
            rampIndex = 1; // greater than 0
        }
	}

	void noteOff() {
		// Nothing to do
	}


private:
	LfoType type;
	LfoParams* lfo ;
    int index, rampIndex, ramp;
    DestinationEnum destination;
    int currentRandomValue;

};

#endif /* LFOOSC_H_ */
