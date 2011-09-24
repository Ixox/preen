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

#include "Encoders.h"


Encoders::Encoders() {
	//char encoderPins[] = { 2,3,0,1,10,11, 8,9 };
	// PCB....
    char encoderPins[] = { 9, 8, 11, 10, 2,3, 0,1};
    //char buttonPins[] = { 7,6,5,4, 12,14,13 };
    char buttonPins[] = { 12, 13, 14, 15, 7, 5, 6};

	/*
			0: 0000 = 00 ; No change
			1: 0001 = 00 ; A 0>1, count up
			2: 0010 = 00; B 0>1, count down
			3: 0011 = 00 ; Both changed, invalid
			4: 0100 = 02 ; A 1>0, Down
			5: 0101 = 00 ; no change
			6: 0110 = 01 ; Invalid
			7: 0111 = 00 ;
			8: 1000 = 00
			9: 1001 = 00
			A: 1010 = 00
			B: 1011 = 00
			C: 1100 = 02
			D: 1101 = 00
			E: 1110 = 01
			F: 1111 = 00
	*/

    int actionToCopy[] = { 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
    for (int i=0; i<16; i++) {
    	action[i] = actionToCopy[i];
    }


    firstListener= 0;

	pinMode(HC165_DATA, INPUT);
	pinMode(HC165_CLOCK, OUTPUT);
	pinMode(HC165_LOAD, OUTPUT);

	digitalWrite(HC165_CLOCK, 0);
	digitalWrite(HC165_LOAD, 0);

	for (int k=0; k<NUMBER_OF_ENCODERS; k++) {
		encoderBit1[k] = 1 << encoderPins[k*2];
		encoderBit2[k] = 1 << encoderPins[k*2 + 1];
		lastMove[k] = LAST_MOVE_NONE;
		tickSpeed[k] = 1;
	}

	for (int k=0; k<NUMBER_OF_BUTTONS; k++) {
		buttonBit[k] = 1 << buttonPins[k];
		buttonOldState[k] = true;
	}

	encoderTimer = 0;
}

Encoders::~Encoders() {
}


void Encoders::checkStatus() {



    // Copy the values in the HC165 registers
	digitalWrite(HC165_LOAD, 0);
	digitalWrite(HC165_LOAD, 1);

	// Analyse the new value
	int registerBits = 0;
	for(int i=0; i<16; i++) {
		digitalWrite(HC165_CLOCK, 0);
		registerBits |= (digitalRead(HC165_DATA) << i) ;
		digitalWrite(HC165_CLOCK, 1);
	}

	for (int k=0; k<NUMBER_OF_ENCODERS; k++) {
		bool b1 = ((registerBits & encoderBit1[k]) == 0);
		bool b2 = ((registerBits & encoderBit2[k]) == 0);

		encoderState[k] <<= 2;
		encoderState[k] &= 0xf;
		if (b1) {
			encoderState[k] |= 1;
		}
		if (b2) {
			encoderState[k] |= 2;
		}

		if (action[encoderState[k]] == 1 && lastMove[k]!=LAST_MOVE_DEC) {
			encoderTurned(k, tickSpeed[k]);
			tickSpeed[k] +=3;
			lastMove[k] = LAST_MOVE_INC;
			timerAction[k] = 60;
		} else if (action[encoderState[k]] == 2 && lastMove[k]!=LAST_MOVE_INC) {
			encoderTurned(k, -tickSpeed[k]);
			tickSpeed[k] +=3;
			lastMove[k] = LAST_MOVE_DEC;
			timerAction[k] = 60;
		} else {
			if (timerAction[k] > 1) {
				timerAction[k] --;
			} else if (timerAction[k] == 1) {
				timerAction[k] --;
				lastMove[k] = LAST_MOVE_NONE;
			}
			if (tickSpeed[k] > 1 && ((encoderTimer & 0x3) == 0)) {
				tickSpeed[k] = tickSpeed[k] - 1;
			}
		}
		if (tickSpeed[k]>10) {
			tickSpeed[k] = 10;
		}
	}

	for (int k=0; k<NUMBER_OF_BUTTONS; k++) {
		bool b1 = ((registerBits & buttonBit[k]) == 0);

		if (!buttonOldState[k] && b1) {
			buttonPressed(k);
		}
		buttonOldState[k] = b1;
	}
	encoderTimer++;
}
