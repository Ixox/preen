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

#ifndef LFO_H_
#define LFO_H_

#include "SynthState.h"
#include "Matrix.h"

// LFo sample rate is 32 times slower than the rest
// SAMPLE_RATE_x_8 / 32 =
#define LFO_SAMPLE_RATE_x_8 8192

class Lfo {
public:
	Lfo();
	~Lfo();
	void init(int number, Matrix* matrix, SourceEnum source);


	void nextValue() {

		// then new value
		//	index = (index +  ((lfo->freq << 16) / LFO_SAMPLE_RATE_x_8 ))  & 0xffff;
		//		int jmp = lfo->freq << 3 ; // << 16 >> 13
		index = (index + (lfo->freq << 3)) & 0xffff;

		switch (lfo->shape) {
		case LFO_RAMP:
			matrix->setSource(source, (index>>8)-128);
			break;
		case LFO_SAW:
		{
			if (index < 32768) {
				matrix->setSource(source, (index>>7) - 128);
			} else {
				matrix->setSource(source, 383 - (index>>7));
			}
			break;
		}
		case LFO_SQUARE:
			if ((index) < 32768) {
				matrix->setSource(source, -128);
			} else {
				matrix->setSource(source, 127);
			}
			break;
		case LFO_TYPE_MAX:
			break;
		}
	}
private:
	int number;
	Matrix *matrix;
	SourceEnum source;
	unsigned int index;
	LfoType type;
	LfoParams* lfo ;
	int frequency;
	int jump;
};

#endif /* LFO_H_ */
