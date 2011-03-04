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

	changedMenuMode = true;
	currentRow = 0;
	newRow= false;

	oscRow = 1;
	envRow = 5;
	matrixRow = 9;
	lfoRow = 15;

	// Start in edit mode
	menuMode = false;
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

	if (!menuMode) {
		// Edit MODE
		bool b1, b2;
		int index = currentRow* NUMBER_OF_ENCODERS ;
		for (int k=0; k<NUMBER_OF_ENCODERS; k++) {
			b1 = ((registerBits & encoderBit1[k]) == 0);
			int newValue;
			int oldValue;
			if (!encoderOldBit1[k] && b1) {
				b2 = ((registerBits & encoderBit2[k]) == 0);
				struct Parameter param = allParameterRows.row[currentRow]->params[k];
				// unsigned or signed

				// Change preset shorcut
				if (k==0 && buttonOldState[BUTTON_LFO] == true) {
					if (b2) {
						currentSynthState++;
						if (currentSynthState->presetName[0]=='*') {
							currentSynthState = &presets[0];
						}
						changedMenuMode = true;
					}
					continue;
				}

				if (param.minValue<0) {
					char &value = ((char*)currentSynthState)[index+k];
					oldValue = value;
					if (b2) {
						if (value>param.minValue) {
							value--;
						}
					} else {
						if (value<param.maxValue) {
							value++;
						}
					}
					newValue = value;
				} else {
					unsigned char &value = ((unsigned char*)currentSynthState)[index+k];
					oldValue = value;
					if (b2) {
						if (value>param.minValue) {
							value--;
						}
					} else {
						if (value<param.maxValue) {
							value++;
						}
					}
					newValue = value;
				}
				if (newValue != oldValue) {
					if (isEditingMatrix()) {
						synth->getMatrix()->reinitUsage(k, oldValue, newValue);
					}
					if (isEditingEnvelope()) {
						synth->reloadADSR();
					}
				}
				changedValue = index+k;
			}
			encoderOldBit1[k] = b1;
		}


		for (int k=0; k<NUMBER_OF_BUTTONS; k++) {
			b1 = ((registerBits & buttonBit[k]) == 0);

			if (!buttonOldState[k] && b1) {
				switch (k) {
				case BUTTON_SYNTH:
					currentRow = 0;
					break;
				case BUTTON_OSC:
					if (currentRow<1 || currentRow>4) {
						currentRow = oscRow;
					} else {
						currentRow ++;
						if (currentRow<1 || currentRow>4) {
							currentRow = 1;
						}
					}
					oscRow = currentRow;
					break;
				case BUTTON_ENV:
					if (currentRow<5 || currentRow>8) {
						currentRow = envRow;
					} else {
						currentRow ++;
						if (currentRow<5 || currentRow>8) {
							currentRow = 5;
						}
					}
					envRow = currentRow;
					break;
				case BUTTON_MATRIX:
					if (currentRow<9 || currentRow>14) {
						currentRow = matrixRow;
					} else {
						currentRow ++;
						if (currentRow<9 || currentRow>14) {
							currentRow = 9;
						}
					}
					matrixRow = currentRow;
					break;
				case BUTTON_LFO:
					if (currentRow<15 || currentRow>18) {
						currentRow = lfoRow;
					} else {
						currentRow ++;
						if (currentRow<15 || currentRow>18) {
							currentRow = 15;
						}
					}
					break;
				case BUTTON_MENU:
					menuMode = !menuMode;
					currentMenuState = MENU_NONE;
					menuSelect = 0;
					changedMenuMode = true;
					break;
				}
				newRow = true;
			}
			buttonOldState[k] = b1;
		}
	} else {
		// Only one encoder in menu mode
		bool b1, b2;
		b1 = ((registerBits & encoderBit1[0]) == 0);
		if (!encoderOldBit1[0] && b1) {
			b2 = ((registerBits & encoderBit2[0]) == 0);

			if (currentMenuState == MENU_NONE || currentMenuState == MENU_LOAD) {
				if (!b2) {
					if (menuSelect<1) {
						menuSelect = menuSelect + 1;
					}
				} else {
					if (menuSelect>0) {
						menuSelect = menuSelect - 1;
					}
				}
			} else {
				if (!b2) {
					if (menuSelect<255) {
						menuSelect = menuSelect + 1;
					}
				} else {
					if (menuSelect>0) {
						menuSelect = menuSelect - 1;
					}
				}
			}
			newRow = true;
		}
		encoderOldBit1[0] = b1;

		for (int k=0; k<NUMBER_OF_BUTTONS; k++) {
			b1 = ((registerBits & buttonBit[k]) == 0);

			if (!buttonOldState[k] && b1) {
				switch (k) {
				case BUTTON_SELECT:
					switch (currentMenuState) {
						case MENU_NONE:
							if (menuSelect == 0) {
								currentMenuState = MENU_LOAD;
							} else {
								currentMenuState = MENU_SAVE;
							}
							break;
						case MENU_LOAD:
							if (menuSelect == 0) {
								currentMenuState = MENU_LOAD_INTERNAL_BANK;
							} else {
								currentMenuState = MENU_LOAD_USER_BANK;
							}
							break;
						case MENU_SAVE:
							// save menuSelect bank
							break;
						case MENU_LOAD_INTERNAL_BANK:
							// load internal bank
							break;
						case MENU_LOAD_USER_BANK:
							// load internal bank
							break;
						default:
							break;
					}
					menuSelect = 0;
					break;
				case BUTTON_BACK:
					switch (currentMenuState) {
						case MENU_SAVE:
							menuSelect = 1;
							currentMenuState = MENU_NONE;
							break;
						case MENU_LOAD:
							menuSelect = 0;
							currentMenuState = MENU_NONE;
							break;
						case MENU_LOAD_INTERNAL_BANK:
						case MENU_LOAD_USER_BANK:
							menuSelect = 0;
							currentMenuState = MENU_LOAD;
							break;
						default:
							break;
					}
					break;
				case BUTTON_DUMP:
				{
					SerialUSB.println("New Sound....");
					dumpLine(currentSynthState->engine.algo, currentSynthState->engine.modulationIndex1, currentSynthState->engine.modulationIndex2, currentSynthState->engine.modulationIndex3 );
					Oscillator * o = (Oscillator *)(&(currentSynthState->osc1));
					for (int k=0; k<4; k++) {
						dumpLine(o[k].shape, o[k].frequencyType, o[k].frequencyMul, o[k].detune);
					}
					Envelope * e = (Envelope*)(&(currentSynthState->env1));
					for (int k=0; k<4; k++) {
						dumpLine(e[k].attack, e[k].decay, e[k].sustain, e[k].release);
					}
					MatrixRowState* m = (MatrixRowState*)(&(currentSynthState->matrixRowState1));
					for (int k=0; k<6; k++) {
						dumpLine(m[k].source, m[k].mul, m[k].destination, 0);
					}
					LfoState* l = (LfoState*)(&(currentSynthState->lfo1));
					for (int k=0; k<4; k++) {
						dumpLine(l[k].shape, l[k].freq, 0, 0);
					}
					SerialUSB.println("\"SoundName\"");
					break;
				}
				case BUTTON_MENU:
					menuMode = !menuMode;
					changedMenuMode = true;
					break;
				}
				newRow = true;
			}
			buttonOldState[k] = b1;
		}

	}

}
