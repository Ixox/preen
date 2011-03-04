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

#ifndef ENCODERS_H_
#define ENCODERS_H_

#include "wirish.h"
#include "libmaple.h"
#include "Synth.h"
#include "RingBuffer.h"
#include "SynthStatus.h"

#define HC165_DATA   12
#define HC165_CLOCK  11
#define HC165_LOAD   10


#define NUMBER_OF_ENCODERS 4
#define NUMBER_OF_BUTTONS 6

#define BUTTON_SYNTH  0
#define BUTTON_OSC    1
#define BUTTON_ENV    2
#define BUTTON_MATRIX 3
#define BUTTON_LFO    4

#define BUTTON_MENU   5

#define BUTTON_SELECT 4
#define BUTTON_BACK   3
#define BUTTON_DUMP   0



enum MenuState {
	MENU_NONE = 0,
	MENU_LOAD,
	MENU_SAVE,
	MENU_LOAD_INTERNAL_BANK,
	MENU_LOAD_USER_BANK,
	MENU_SAVE_PRESET
};



struct EncoderStatus {
	char value;
	bool b1;
};

class Encoders {
public:
	Encoders();
	virtual ~Encoders();
	void checkStatus();
	void setSynth(Synth* synth) {
		this->synth = synth;
	}

	int getCurrentRow() {
		return currentRow;
	}

	bool isEditingEnvelope() {
		return currentRow>=5 && currentRow<=8;
	}

	bool isEditingMatrix() {
		return currentRow>=9 && currentRow<=14;
	}

	int valueHasChanged() {
		return changedValue;
	}

	bool rowChanged() {
		return newRow;
	}

	bool menuModeChanged() {
		return changedMenuMode;
	}

	void resetChanged() {
		changedValue = -1;
		newRow = false;
		changedMenuMode = false;
	}

	int getRowNumberRelative() {
		if (currentRow<5) {
			return currentRow;
		} else if (currentRow<9) {
			return currentRow - 4;
		} else if (currentRow<15) {
			return currentRow- 8;
		} else {
			return currentRow - 14;
		}
	}

	void dumpLine(int a, int b, int c, int d) {
		SerialUSB.print("{ ");
		SerialUSB.print(a);
		SerialUSB.print(", ");
		SerialUSB.print(b);
		SerialUSB.print(", ");
		SerialUSB.print(c);
		SerialUSB.print(", ");
		SerialUSB.print(d);
		SerialUSB.print("} ");
		SerialUSB.println(", ");
	}

	MenuState getMenuState() {
		return currentMenuState;
	}

	int getMenuSelect() {
		return menuSelect;
	}

	bool isMenuMode() {
		return menuMode;
	}

private:
	Synth* synth;

	bool encoderOldBit1[NUMBER_OF_ENCODERS];
	int encoderBit1[NUMBER_OF_ENCODERS];
	int encoderBit2[NUMBER_OF_ENCODERS];

	int buttonBit[NUMBER_OF_BUTTONS];
	bool buttonOldState[NUMBER_OF_BUTTONS];

	int currentRow;
	int changedValue;
	bool newRow;
	bool changedMenuMode;

	int oscRow, envRow, matrixRow, lfoRow;

	MenuState currentMenuState;
	bool menuMode;
	int menuSelect;

};

#endif /* ENCODERS_H_ */
