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
	char encoderPins[] = {12, 13, 14, 15, 4, 5, 6, 7};
	char buttonPins[] = { 8, 9,3, 10, 11, 2};
	firstListener= 0;

	pinMode(HC165_DATA, INPUT);
	pinMode(HC165_CLOCK, OUTPUT);
	pinMode(HC165_LOAD, OUTPUT);
	digitalWrite(HC165_CLOCK, 0);
	digitalWrite(HC165_LOAD, 0);

	for (int k=0; k<NUMBER_OF_ENCODERS; k++) {
		encoderBit1[k] = 1 << encoderPins[k*2];
		encoderBit2[k] = 1 << encoderPins[k*2 + 1];
		encoderOldBit1[k] = true;
	}

	for (int k=0; k<NUMBER_OF_BUTTONS; k++) {
		buttonBit[k] = 1 << buttonPins[k];
		buttonOldState[k] = true;
	}
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
		if (!encoderOldBit1[k] && b1) {
			bool b2 = ((registerBits & encoderBit2[k]) == 0);
			if (b2) {
				decEncoder(k);
			} else {
				incEncoder(k);
			}
		}
		encoderOldBit1[k] = b1;
	}


	for (int k=0; k<NUMBER_OF_BUTTONS; k++) {
		bool b1 = ((registerBits & buttonBit[k]) == 0);

		if (!buttonOldState[k] && b1) {
			buttonPressed(k);
		}
		buttonOldState[k] = b1;
	}

}
