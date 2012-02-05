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

}


void LfoOsc::midiClock(int songPosition) {
	switch (lfo->freq) {
		case LFO_MIDICLOCK_MC_DIV_16:
			if ((songPosition & 0x3)==0) {
				stepPlusMatrix = 0xfff / ticks;
				ticks = 0;
				index = (songPosition & 0x3C) * 0x3ff ;
			}
			break;
		case LFO_MIDICLOCK_MC_DIV_8:
			if ((songPosition & 0x3)==0) {
				stepPlusMatrix = 0x1fff / ticks;
				ticks = 0;
				index = (songPosition & 0x1C) * 0x7ff ;
			}
			break;
		case LFO_MIDICLOCK_MC_DIV_4:
			if ((songPosition & 0x3)==0) {
				stepPlusMatrix = 0x3fff / ticks;
				ticks = 0;
				index = (songPosition & 0xC) * 0xfff ;
			}
			break;
		case LFO_MIDICLOCK_MC_DIV_2:
			if ((songPosition & 0x3)==0) {
				stepPlusMatrix = 0x7fff / ticks;
				ticks = 0;
				index = (songPosition & 0x4) * 0x1fff ;
			}
			break;
		case LFO_MIDICLOCK_MC:
			// Midi Clock
			if ((songPosition & 0x3)==0) {
				stepPlusMatrix = 0xffff / ticks;
				ticks = 0;
				index = 0;
			}
			break;
		case LFO_MIDICLOCK_MC_TIME_2:
			if ((songPosition & 0x3)==0) {
				stepPlusMatrix = 0x1ffff / ticks;
				ticks = 0;
				index = 0;
			}
			break;
		case LFO_MIDICLOCK_MC_TIME_3:
			if ((songPosition & 0x3)==0) {
				stepPlusMatrix = 0xffff / ticks * 3;
				ticks = 0;
				index = 0;
			}
			break;
		case LFO_MIDICLOCK_MC_TIME_4:
			if ((songPosition & 0x3)==0) {
				stepPlusMatrix = 0x3ffff / ticks;
				ticks = 0;
				index = 0;
			}
			break;
		case LFO_MIDICLOCK_MC_TIME_8:
			if ((songPosition & 0x3)==0) {
				stepPlusMatrix = 0x7ffff / ticks;
				ticks = 0;
				index = 0;
			}
			break;
	}
}


