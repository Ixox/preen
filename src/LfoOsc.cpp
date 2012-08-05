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

#include "LfoOsc.h"



void LfoOsc::init(int number, Matrix *matrix, SourceEnum source, DestinationEnum dest) {
	Lfo::init(number, matrix, source, dest);
	this->type = LFO_SAW;
    this->ramp = 0;
    this->rampIndex = 0;
	LfoParams* lfoTmp = (LfoParams *)&this->synthState->params.lfo1;
	this->lfo = &lfoTmp[number];
    this->rampIndex =  0;
    this->ramp = lfo->keybRamp << 4;
    this->destination = dest;

	ticks = 1536;
	midiClock(0, true);
}


void LfoOsc::midiClock(int songPosition, boolean computeStep) {
	switch (lfo->freq) {
		case LFO_MIDICLOCK_MC_DIV_16:
			if ((songPosition & 0x1)==0) {
				if (computeStep) {
					stepPlusMatrix = 0x7fff / ticks;
					ticks = 0;
				}
				index = (songPosition & 0x3E) * 0x3fff ;
			}
			break;
		case LFO_MIDICLOCK_MC_DIV_8:
			if ((songPosition & 0x1)==0) {
				if (computeStep) {
					stepPlusMatrix = 0xffff / ticks;
					ticks = 0;
				}
				index = (songPosition & 0x1E) * 0x7fff ;
			}
			break;
		case LFO_MIDICLOCK_MC_DIV_4:
			if ((songPosition & 0x1)==0) {
				if (computeStep) {
					stepPlusMatrix = 0x1ffff / ticks;
					ticks = 0;
				}
				index = (songPosition & 0xE) * 0xffff ;
			}
			break;
		case LFO_MIDICLOCK_MC_DIV_2:
			if ((songPosition & 0x1)==0) {
				if (computeStep) {
					// 0xffff /4 in the interval
					stepPlusMatrix = 0x3ffff / ticks;
					ticks = 0;
				}
				// ((songPosition &0x6) >>1) * 0x1fff
				index = (songPosition & 0x6) * 0x1ffff ;
			}
			break;
		case LFO_MIDICLOCK_MC:
			// Midi Clock
			if ((songPosition & 0x1)==0) {
				if (computeStep) {
					// Half the steps in the interval
					stepPlusMatrix = 0x7ffff / ticks;
					ticks = 0;
				}
				// ((songPosition &0x2) >>1) * 0x7fff
				index = (songPosition & 0x2) * 0x3ffff;
			}
			break;
		case LFO_MIDICLOCK_MC_TIME_2:
			if ((songPosition & 0x1)==0) {
				if (computeStep) {
					stepPlusMatrix = 0xfffff / ticks;
					ticks = 0;
				}
				index = 0;
			}
			break;
		case LFO_MIDICLOCK_MC_TIME_3:
			if ((songPosition & 0x3)==0) {
				if (computeStep) {
					stepPlusMatrix = 0xfffff / ticks * 3;
					ticks = 0;
				}
				index = 0;
			}
			break;
		case LFO_MIDICLOCK_MC_TIME_4:
			if ((songPosition & 0x1)==0) {
				if (computeStep) {
					stepPlusMatrix = 0x1fffff / ticks;
					ticks = 0;
				}
				index = 0;
			}
			break;
		case LFO_MIDICLOCK_MC_TIME_8:
			if ((songPosition & 0x1)==0) {
				if (computeStep) {
					stepPlusMatrix = 0x3fffff / ticks;
					ticks = 0;
				}
				index = 0;
			}
			break;
	}
}


void LfoOsc::nextValueInMatrix() {
     int lfoValue = 0;

     ticks ++;
	// then new value
	//	index = (index +  ((lfo->freq << 16) / LFO_SAMPLE_RATE_x_8 ))  & 0xffff;
	//		int jmp = lfo->freq	<< 3 ; // << 16 >> 13
     if (lfo->freq <LFO_MIDICLOCK_MC_DIV_16) {
    	 stepPlusMatrix = (lfo->freq + (this->matrix->getDestination(destination) >> 7)) << 7;
     }

	switch (lfo->shape) {
	case LFO_SAW:
	{
		index = (index + stepPlusMatrix) & 0xfffff;
		if (index < 0x7ffff) {
		    lfoValue = (index>>11) - 128;
		} else {
		    lfoValue = 383 - (index>>11);
		}
		break;
	}
	case LFO_RAMP:
		index = (index + stepPlusMatrix) & 0xfffff;
		lfoValue = (index>>12)-128;
		break;
	case LFO_SIN:
		index = (index + stepPlusMatrix) & 0xfffff;
		// Index is on 20 bits and sinTable size on 11... so we shift 9
		lfoValue = sinTable[ index>> 9 ] >> 8;
		break;
	case LFO_SQUARE:
		index = (index + stepPlusMatrix) & 0xfffff;
		if ((index) < 0x7ffff) {
		    lfoValue = -128;
		} else {
            lfoValue = 127;
		}
		break;
	case LFO_RANDOM:
		index += stepPlusMatrix;
		if (index > 0xfffff) {
			 index &= 0xfffff;
			 currentRandomValue = (noise[0] >> 7) - 128;
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



