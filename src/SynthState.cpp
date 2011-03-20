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

#include "SynthState.h"



// FLASH :  __attribute__ ((section (".USER_FLASH")))
// Ex : const char* nullNames [] __attribute__ ((section (".USER_FLASH")))= {};
// DISPLAY structures
const char* allChars = "_ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz 0123456789";

const char* nullNames []= {};
const char* algoNames []= { "alg1", "alg2", "alg3", "alg4", "alg5" };
struct ParameterRowDisplay engineParameterRow= {
		"Engine" ,
		{ "Algo", "IM1 ", "IM2 ", "IM3 " },
		{
				{ALGO1, ALGO_END-1, DISPLAY_TYPE_STRINGS, algoNames},
				{0, 255, DISPLAY_TYPE_FLOAT_5_3, nullNames },
				{0, 255, DISPLAY_TYPE_FLOAT_5_3, nullNames },
				{0, 255, DISPLAY_TYPE_FLOAT_5_3, nullNames }
		}
};

const char* oscShapeNames []=  {"sin ", "off " } ;
const char* oscTypeNames [] = { "keyb", "fixe"};
struct ParameterRowDisplay oscParameterRow = {
		"Oscillator",
		{ "Shap", "FTyp", "Freq", "FTun" },
		{
				{ OSC_SHAPE_SIN, OSC_SHAPE_OFF, DISPLAY_TYPE_STRINGS, oscShapeNames },
				{ OSC_FT_KEYBOARD, OSC_FT_FIXE, DISPLAY_TYPE_STRINGS, oscTypeNames },
				{ 0, 128, DISPLAY_TYPE_OSC_FREQUENCY , nullNames },
				{ (char)-127, 127, DISPLAY_TYPE_OSC_FREQUENCY, nullNames }
		}
};

struct ParameterRowDisplay envParameterRow = {
		"Enveloppe",
		{ "Attk", "Rele", "Sust", "Deca" },
		{
				{ 0, 255, DISPLAY_TYPE_UNSIGNED_CHAR, nullNames },
				{ 0, 255, DISPLAY_TYPE_UNSIGNED_CHAR, nullNames },
				{ 0, 255, DISPLAY_TYPE_UNSIGNED_CHAR, nullNames },
				{ 0, 255, DISPLAY_TYPE_UNSIGNED_CHAR, nullNames }
		}
};


const char* matrixSourceNames [] = { "None", "lfo1", "lfo2", "lfo3", "lfo4", "PitB", "AftT", "ModW"} ;
const char* matrixDestNames [] = { "None", "o1Fr", "o2Fr", "o3Fr", "o4Fr", "IM1 ", "IM2 ", "IM3 "} ;
struct ParameterRowDisplay matrixParameterRow = {
		"Matrix",
		{ "Srce", "Mult", "Dest", "    " },
		{
				{ SOURCE_NONE, SOURCE_MAX-1, DISPLAY_TYPE_STRINGS, matrixSourceNames},
				{ (char)-127, 127, DISPLAY_TYPE_SIGNED_CHAR, nullNames },
				{ DESTINATION_NONE, DESTINATION_MAX-1, DISPLAY_TYPE_STRINGS, matrixDestNames},
				{ 0, 0, DISPLAY_TYPE_NONE, nullNames }
		}
};


const char* lfoShapeNames [] =  { "Saw ", "Ramp", "Squa"} ;
struct ParameterRowDisplay lfoParameterRow = {
		"LFO",
		{ "Shap", "Freq", "    ", "    " },
		{
				{ LFO_SAW, LFO_TYPE_MAX-1, DISPLAY_TYPE_STRINGS,  lfoShapeNames},
				{ 0, 255, DISPLAY_TYPE_FLOAT_4_4, nullNames },
				{ 0, 0, DISPLAY_TYPE_NONE, nullNames },
				{ 0, 0, DISPLAY_TYPE_NONE, nullNames }
		}
};


struct AllParameterRowsDisplay allParameterRows = {
		{&engineParameterRow,
				&oscParameterRow,
				&oscParameterRow,
				&oscParameterRow,
				&oscParameterRow,
				&envParameterRow,
				&envParameterRow,
				&envParameterRow,
				&envParameterRow,
				&matrixParameterRow,
				&matrixParameterRow,
				&matrixParameterRow,
				&matrixParameterRow,
				&matrixParameterRow,
				&matrixParameterRow,
				&lfoParameterRow,
				&lfoParameterRow,
				&lfoParameterRow,
				&lfoParameterRow}
};




/******************** PRESET **********************/

//struct AllSynthParams presets[]  =  {{

const struct AllSynthParams presets[] __attribute__ ((section (".USER_FLASH"))) = {
		{
				// Engine
				{ ALGO1, 16, 21, 6},

				// OSC1
				{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD, 16,  0 },
				{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD, 8,  0 },
				{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD, 32, 0 },
				{ OSC_SHAPE_OFF, OSC_FT_KEYBOARD, 3,  0 },

				// ENV 1
				{ 0,   150, 160,  60 },
				{ 100, 65,  150, 100 },
				{ 50,  100, 255, 100 },
				{ 3,   100, 100, 100 },

				// Matrix row

				{ MODWHEEL ,   16,   INDEX_MODULATION1, 0 },
				{ PITCHBEND,   64,  OSC1_FREQ,   0 },
				{ LFO1,        0,   OSC1_FREQ, 0 },
				{ SOURCE_NONE, 0,  DESTINATION_NONE, 0 },
				{ SOURCE_NONE, 0,  DESTINATION_NONE, 0 },
				{ SOURCE_NONE, 0,  DESTINATION_NONE, 0 },

				// LFOS
				{ LFO_SAW, 36, 0, 0}, {LFO_SAW, 20, 0, 0 },
				{ LFO_SAW, 3, 0, 0}, { LFO_SAW, 4, 0, 0 },
				"Preen"
		}
		,
		{
				// Engine
				{ 1, 3, 28, 7} ,
				// OSC1
				{ 0, 0, 16, 0} ,
				{ 0, 0, 8, 0} ,
				{ 0, 0, 32, 0} ,
				{ 1, 0, 3, 0} ,
				// ENV 1
				{ 72, 150, 160, 60} ,
				{ 81, 66, 196, 77} ,
				{ 50, 100, 255, 100} ,
				{ 3, 100, 100, 100} ,
				// Matrix row
				{ 7, 16, 5, 0} ,
				{ 5, 64, 1, 0} ,
				{ 1, 0, 1, 0} ,
				{ 0, 0, 0, 0} ,
				{ 0, 0, 0, 0} ,
				{ 0, 0, 0, 0} ,
				// LFOS
				{ 0, 36, 0, 0} ,
				{ 0, 20, 0, 0} ,
				{ 0, 3, 0, 0} ,
				{ 0, 4, 0, 0} ,
				"Organ"
		},
		{
				// Engine
				{ 0, 20, 28, 7} ,
				// OSC1
				{ 0, 0, 16, 0} ,
				{ 0, 0, 8, 0} ,
				{ 0, 0, 32, 0} ,
				{ 1, 0, 3, 0} ,
				// ENV 1
				{ 0, 150, 160, 60} ,
				{ 100, 65, 150, 100} ,
				{ 50, 100, 255, 100} ,
				{ 3, 100, 100, 100} ,
				// Matrix row
				{ 7, 2, 6, 0} ,
				{ 5, 64, 1, 0} ,
				{ 1, 0, 1, 0} ,
				{ 7, 6, 6, 0} ,
				{ 2, 11, 6, 0} ,
				{ 0, 0, 0, 0} ,
				// LFOS
				{ 2, 57, 0, 0} ,
				{ 2, 97, 0, 0} ,
				{ 2, 96, 0, 0} ,
				{ 0, 12, 0, 0} ,
				"Weird Bubble"
		},
		{
				// Engine
				{ 0, 15, 3, 0} ,
				// OSC1
				{ 0, 0, 16, 0} ,
				{ 0, 0, 32, 0} ,
				{ 0, 0, 1, 0} ,
				{ 0, 0, 3, 0} ,
				// ENV 1
				{ 100, 150, 63, 11} ,
				{ 0, 122, 125, 253} ,
				{ 50, 100, 255, 100} ,
				{ 3, 100, 100, 100} ,
				// Matrix row
				{ 7, 2, 5, 0} ,
				{ 5, 64, 1, 0} ,
				{ 1, 15, 2, 0} ,
				{ 0, 0, 0, 0} ,
				{ 0, 0, 0, 0} ,
				{ 0, 0, 0, 0} ,
				// LFOS
				{ 0, 23, 0, 0} ,
				{ 0, 20, 0, 0} ,
				{ 0, 3, 0, 0} ,
				{ 0, 4, 0, 0} ,
				"Old and Sad"
		},
		{
				{ 1, 3, 28, 7} ,
				{ 0, 0, 16, 0} ,
				{ 0, 0, 8, 0} ,
				{ 0, 0, 32, 0} ,
				{ 1, 0, 3, 0} ,
				{ 72, 150, 160, 60} ,
				{ 81, 66, 196, 77} ,
				{ 50, 100, 255, 100} ,
				{ 3, 100, 100, 100} ,
				{ 7, 16, 9, 0} ,
				{ 5, 64, 1, 0} ,
				{ 1, 0, 1, 0} ,
				{ 0, 0, 0, 0} ,
				{ 0, 0, 0, 0} ,
				{ 0, 0, 0, 0} ,
				{ 0, 36, 0, 0} ,
				{ 0, 20, 0, 0} ,
				{ 0, 3, 0, 0} ,
				{ 0, 4, 0, 0} ,
				"*"
		}
};

#define INTERNAL_LAST_BANK 3

SynthState::SynthState() {
	preset = 0;
	bank = BANK_INTERNAL;
	oscRow = 1;
	envRow = 5;
	matrixRow = 9;
	lfoRow = 15;
	// First default preset
	fullState.synthMode = SYNTH_MODE_EDIT;
	for (int k=0; k<12; k++) {
		fullState.name[k] = 0;
	}

	currentRow = 0;
	for (unsigned int k=0; k<sizeof(struct AllSynthParams); k++) {
		((char*)&params)[k] = ((char*)presets)[k];
	}
	// enable the i2c bus
	i2c_master_enable(I2C1, 0);

}

void SynthState::encoderTurned(int encoder, int ticks) {
	if (fullState.synthMode == SYNTH_MODE_EDIT) {
		int num = currentRow * NUMBER_OF_ENCODERS + encoder;
		struct ParameterDisplay* param = &(allParameterRows.row[currentRow]->params[encoder]);
		int newValue;
		int oldValue;

		if ((param->displayType == DISPLAY_TYPE_SIGNED_CHAR) || (param->displayType == DISPLAY_TYPE_OSC_FREQUENCY && param->minValue<0)) {
			char &value = ((char*)&params)[num];
			oldValue = value;
			value+=ticks;
			if (ticks>0 && value>param->maxValue) {
				value = param->maxValue;
			}
			if (ticks<0 && value<param->minValue) {
				value = param->minValue;
			}
			newValue = value;
		} else {
			unsigned char &value = ((unsigned char*)&params)[num];
			oldValue = value;
			// Must use oldValue (int) so that the minValue comparaison works
			newValue = value + ticks;
			if (ticks>0 && newValue>param->maxValue) {
				newValue = param->maxValue;
			}
			if (ticks<0 && newValue<param->minValue) {
				newValue = param->minValue;
			}
			value = (char)newValue;
		}

		if (newValue != oldValue) {
			propagateNewParamValue(currentRow, encoder, param, oldValue, newValue);
		}
	} else {
		if (encoder==0) {
			int oldMenuSelect = fullState.menuSelect;
			if (ticks>0) {
				if (fullState.currentMenuState == MENU_LOAD_INTERNAL_BANK) {
					if (fullState.menuSelect< INTERNAL_LAST_BANK) {
						fullState.menuSelect = fullState.menuSelect + 1;
						char* preset = (char*)&(presets[fullState.menuSelect].engine);
						copyPatch(preset, (char*)&params, true);
					}
				} else if (fullState.currentMenuState == MENU_NONE || fullState.currentMenuState == MENU_LOAD) {
					if (fullState.menuSelect<1) {
						fullState.menuSelect = fullState.menuSelect + 1;
					}
				} else if (fullState.currentMenuState == MENU_ENTER_NAME) {
					if (fullState.menuSelect<11) {
						fullState.menuSelect = fullState.menuSelect + 1;
					}
				} else {
					if (fullState.menuSelect<255) {
						fullState.menuSelect = fullState.menuSelect + 1;
					}
				}
			} else if (ticks<0) {
				if (fullState.menuSelect>0) {
					fullState.menuSelect = fullState.menuSelect - 1;
				}

				if (fullState.currentMenuState == MENU_LOAD_INTERNAL_BANK) {
					char* preset = (char*)&(presets[fullState.menuSelect].engine);
					copyPatch(preset, (char*)&params, true);
				}
			}
			if (fullState.menuSelect != oldMenuSelect) {
				propagateNewMenuSelect();
			}
		} else if (encoder==3) {
			if (fullState.currentMenuState == MENU_ENTER_NAME) {
				fullState.name[fullState.menuSelect] = (fullState.name[fullState.menuSelect] + ticks);
				if (fullState.name[fullState.menuSelect]<0) {
					fullState.name[fullState.menuSelect]=0;
				}
				if (fullState.name[fullState.menuSelect]>= getLength(allChars)) {
					fullState.name[fullState.menuSelect]= getLength(allChars)-1;
				}
				propagateNewMenuSelect();
			}
		}
	}

}


void SynthState::copyPatch(char* source, char* dest, bool propagate) {
	for (unsigned int k=0; k<sizeof(struct AllSynthParams); k++) {
		int currentRow = k / NUMBER_OF_ENCODERS;
		if (currentRow < NUMBER_OF_ROWS && propagate) {
			int encoder = k % NUMBER_OF_ENCODERS;
			struct ParameterDisplay* param = &(allParameterRows.row[currentRow]->params[encoder]);
			propagateNewParamValue(currentRow, encoder, param, dest[k], source[k]);
		}
		dest[k] = source[k];
	}
}


void SynthState::buttonPressed(int button) {
	SynthMode oldSynthMode = fullState.synthMode;
	int oldCurrentRow = currentRow;
	int oldMenuSelect = fullState.menuSelect;
	MenuState oldMenuState = fullState.currentMenuState;

	if (fullState.synthMode == SYNTH_MODE_EDIT)  {
		switch (button) {
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
		case BUTTON_MENUSELECT:
			fullState.synthMode = SYNTH_MODE_MENU;
			fullState.currentMenuState = MENU_NONE;
			fullState.menuSelect = 0;
			// allow undo event after trying some patches
			copyPatch((char*)&params, (char*)&backupParams, true);
			break;
		}
	} else {

		switch (button) {
		case BUTTON_MENUSELECT:
			switch (fullState.currentMenuState) {
			case MENU_NONE:
				if (fullState.menuSelect == 0) {
					fullState.currentMenuState = MENU_LOAD;
				} else {
					fullState.currentMenuState = MENU_SAVE;
				}
				fullState.menuSelect = 0;
				break;
			case MENU_LOAD:
				if (fullState.menuSelect == 0) {
					// Load first patch
					copyPatch((char*)&presets[0], (char*)&params, true);
					fullState.currentMenuState = MENU_LOAD_INTERNAL_BANK;
				} else {
					fullState.currentMenuState = MENU_LOAD_USER_BANK;
				}
				fullState.menuSelect = 0;
				break;
			case MENU_SAVE:
				fullState.currentMenuState = MENU_ENTER_NAME;
				fullState.presetNumber = fullState.menuSelect;
				fullState.menuSelect = 0;
				break;
			case MENU_ENTER_NAME:
			{
				int length;
				for (length=12; fullState.name[length-1] == 0; length--);
				for (int k=0; k<length; k++) {
					params.presetName[k] = allChars[(int)fullState.name[k]];
				}
				params.presetName[length] = '\0';
				pruneToEEPROM(fullState.presetNumber);
				fullState.currentMenuState = MENU_DONE;
				break;
			}
			case MENU_LOAD_INTERNAL_BANK:
				copyPatch((char*)&params, (char*)&backupParams, true);
				fullState.currentMenuState = MENU_DONE;
				break;
			case MENU_LOAD_USER_BANK:
				readFromEEPROM(fullState.menuSelect);
				copyPatch((char*)&params, (char*)&backupParams, true);
				fullState.currentMenuState = MENU_DONE;
				break;
			case MENU_DONE:
				fullState.synthMode = SYNTH_MODE_EDIT;
				break;
			default:
				break;
			}
			break;
		case BUTTON_BACK:
			switch (fullState.currentMenuState) {
			case MENU_SAVE:
				fullState.menuSelect = 1;
				fullState.currentMenuState = MENU_NONE;
				break;
			case MENU_LOAD:
				fullState.menuSelect = 0;
				fullState.currentMenuState = MENU_NONE;
				break;
			case MENU_LOAD_INTERNAL_BANK:
			case MENU_LOAD_USER_BANK:
				fullState.menuSelect = 0;
				fullState.currentMenuState = MENU_LOAD;
				// put back old patch (has been overwritten if a new patch has been loaded)
				copyPatch((char*)&backupParams, (char*)&params, true);
				break;
			case MENU_NONE:
				fullState.synthMode = SYNTH_MODE_EDIT;
				// put back old patch (has been overwritten if a new patch has been loaded)
				copyPatch((char*)&backupParams, (char*)&params, true);
				break;
			case MENU_ENTER_NAME:
				fullState.menuSelect = fullState.presetNumber;
				fullState.currentMenuState = MENU_SAVE;
				break;
			}
			break;
		case BUTTON_DUMP:
			{
				SerialUSB.println("New Sound....");
				dumpLine(params.engine.algo, params.engine.modulationIndex1, params.engine.modulationIndex2, params.engine.modulationIndex3 );
				OscillatorParams * o = (OscillatorParams *)(&(params.osc1));
				for (int k=0; k<4; k++) {
					dumpLine(o[k].shape, o[k].frequencyType, o[k].frequencyMul, o[k].detune);
				}
				EnvelopeParams * e = (EnvelopeParams*)(&(params.env1));
				for (int k=0; k<4; k++) {
					dumpLine(e[k].attack, e[k].decay, e[k].sustain, e[k].release);
				}
				MatrixRowParams* m = (MatrixRowParams*)(&(params.matrixRowState1));
				for (int k=0; k<6; k++) {
					dumpLine(m[k].source, m[k].mul, m[k].destination, 0);
				}
				LfoParams* l = (LfoParams*)(&(params.lfo1));
				for (int k=0; k<4; k++) {
					dumpLine(l[k].shape, l[k].freq, 0, 0);
				}
				SerialUSB.println(params.presetName);
				break;
			}

				// MENU MODE
		}
	}
	if (oldMenuSelect != fullState.menuSelect) {
		propagateNewMenuSelect();
	}
	if (oldSynthMode != fullState.synthMode) {
		propagateNewSynthMode();
	}
	if (oldCurrentRow != currentRow) {
		propagateNewCurrentRow(currentRow);
	}
	if (oldMenuState != fullState.currentMenuState) {
		propagateNewMenuState();
	}
}


void SynthState::pruneToEEPROM(int preset) {
	uint8 deviceaddress = 0b1010000;
	i2c_msg msgWrite1, msgWrite2;
	int block1Size = 64;
	int address = preset * 128;
	uint8 bufWrite1[block1Size + 2];

	bufWrite1[0] = (uint8)address >> 8;
	bufWrite1[1] = (uint8)address & 0xff;
	for (int k=0; k<block1Size; k++) {
		bufWrite1[k+2] = ((uint8*)&params)[k];
	}
	/* Write test pattern  */
	msgWrite1.addr = deviceaddress;
	msgWrite1.flags = 0;
	msgWrite1.length = block1Size +2;
	msgWrite1.data = bufWrite1;
	i2c_master_xfer(I2C1, &msgWrite1, 1);
	delay(5);

	int block2Size = sizeof(struct AllSynthParams) - block1Size;
	uint8 bufWrite2[block2Size + 2];
	address = address + block1Size;
	bufWrite2[0] = (uint8)address >> 8;
	bufWrite2[1] = (uint8)address & 0xff;
	for (int k=0; k<block2Size; k++) {
		bufWrite2[k+2] = ((uint8*)&params)[k+block1Size];
	}
	msgWrite2.addr = deviceaddress;
	msgWrite2.flags = 0;
	msgWrite2.length = block2Size + 2;
	msgWrite2.data = bufWrite2;
	i2c_master_xfer(I2C1, &msgWrite2, 1);
	delay(5);

}



void SynthState::readFromEEPROM(int preset) {
	uint8 deviceaddress = 0b1010000;
	int address = preset*128;
	uint8 bufReadAddress[2];
	i2c_msg msgsRead[2];
	int block1Size = 64;

	bufReadAddress[0] = (uint8)address >> 8;
	bufReadAddress[1] = (uint8)address & 0xff;

	msgsRead[0].addr = deviceaddress;
	msgsRead[0].flags = 0;
	msgsRead[0].length = 2;
	msgsRead[0].data = bufReadAddress;

	msgsRead[1].addr = deviceaddress;
	msgsRead[1].flags = I2C_MSG_READ;
	msgsRead[1].length = block1Size;
	msgsRead[1].data = (uint8*)&params;

	i2c_master_xfer(I2C1, msgsRead, 2);
	delay(5);

	int block2Size = sizeof(struct AllSynthParams) - block1Size;

	address = address + block1Size;
	bufReadAddress[0] = (uint8)address>>8;
	bufReadAddress[1] = (uint8)address & 0xff;

	msgsRead[0].addr = deviceaddress;
	msgsRead[0].flags = 0;
	msgsRead[0].length = 2;
	msgsRead[0].data = bufReadAddress;

	msgsRead[1].addr = deviceaddress;
	msgsRead[1].flags = I2C_MSG_READ;
	msgsRead[1].length = block2Size;
	msgsRead[1].data = &((uint8*)&params)[block1Size];
	i2c_master_xfer(I2C1, msgsRead, 2);

	delay(5);
}


void SynthState::setNewValue(int row, int number, int newValue) {
	int index = row * NUMBER_OF_ENCODERS + number;
	struct ParameterDisplay* param = &(allParameterRows.row[row]->params[number]);
	if (param->displayType == DISPLAY_TYPE_SIGNED_CHAR) {
		((char*)&params)[index] = newValue;
	} else {
		((unsigned char*)&params)[index] = newValue;
	}
	int oldValue = ((char*)&params)[index];
	propagateNewParamValueFromExternal(row, number, param, oldValue, newValue);
}
