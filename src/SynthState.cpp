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
const char* nullNames []= {};
const char* algoNames []= { "alg1", "alg2", "alg3", "alg4" };
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
					{ 0, 128, DISPLAY_TYPE_FLOAT_4_4 , nullNames },
					{ (char)-127, 127, DISPLAY_TYPE_SIGNED_CHAR, nullNames }
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
const char* matrixDestNames [] = { "None", "o1Fr", "o2Fr", "o3Fr", "o4Fr", "o1Am", "o2Am", "o3Am", "o4Am", "IM1 ", "IM2 ", "IM3 "} ;
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

struct AllSynthParams presets[]  =  {{
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
	"Organ"
},
{
	{ 0, 20, 28, 7} ,
	{ 0, 0, 16, 0} ,
	{ 0, 0, 8, 0} ,
	{ 0, 0, 32, 0} ,
	{ 1, 0, 3, 0} ,
	{ 0, 150, 160, 60} ,
	{ 100, 65, 150, 100} ,
	{ 50, 100, 255, 100} ,
	{ 3, 100, 100, 100} ,
	{ 7, 2, 10, 0} ,
	{ 5, 64, 1, 0} ,
	{ 1, 0, 1, 0} ,
	{ 7, 6, 10, 0} ,
	{ 2, 11, 10, 0} ,
	{ 0, 0, 0, 0} ,
	{ 2, 57, 0, 0} ,
	{ 2, 97, 0, 0} ,
	{ 2, 96, 0, 0} ,
	{ 0, 12, 0, 0} ,
	"Weird Bubble"
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
	currentRow = 0;
	// enable the i2c bus
	for (unsigned int k=0; k<sizeof(struct AllSynthParams); k++) {
		((char*)&params)[k] = ((char*)presets)[k];
	}
    i2c_master_enable(I2C1, 0);
}

void SynthState::incParameter(int encoder) {
	if (fullState.synthMode == SYNTH_MODE_EDIT) {
		int num = currentRow * NUMBER_OF_ENCODERS + encoder;
		struct ParameterDisplay* param = &(allParameterRows.row[currentRow]->params[encoder]);
		int newValue;
		int oldValue;
		if (param->minValue<0) {
			char &value = ((char*)&params)[num];
			oldValue = value;
			if (value<param->maxValue) {
				value++;
			}
			newValue = value;
		} else {
			unsigned char &value = ((unsigned char*)&params)[num];
			oldValue = value;
			if (value<param->maxValue) {
				value++;
			}
			newValue = value;
		}

		if (newValue != oldValue) {
			propagateNewParamValue(currentRow, encoder, oldValue, newValue);
		}
	} else {
		int oldMenuSelect = fullState.menuSelect;
		if (fullState.currentMenuState == MENU_LOAD_INTERNAL_BANK) {
			if (fullState.menuSelect< INTERNAL_LAST_BANK) {
				fullState.menuSelect = fullState.menuSelect + 1;
				copyPatch((char*)&presets[fullState.menuSelect], (char*)&params);
			}
		} else if (fullState.currentMenuState == MENU_NONE || fullState.currentMenuState == MENU_LOAD) {
			if (fullState.menuSelect<1) {
				fullState.menuSelect = fullState.menuSelect + 1;
			}
		} else {
			if (fullState.menuSelect<255) {
				fullState.menuSelect = fullState.menuSelect + 1;
			}
		}
		if (fullState.menuSelect != oldMenuSelect) {
			propagateNewMenuSelect();
		}
	}

}

void SynthState::decParameter(int encoder) {
	if (fullState.synthMode == SYNTH_MODE_EDIT) {
		int num = currentRow * NUMBER_OF_ENCODERS + encoder;
		struct ParameterDisplay* param = &(allParameterRows.row[currentRow]->params[encoder]);
		int newValue;
		int oldValue;
		if (param->minValue<0) {
			char &value = ((char*)&params)[num];
			oldValue = value;
			if (value>param->minValue) {
				value--;
			}
			newValue = value;
		} else {
			unsigned char &value = ((unsigned char*)&params)[num];
			oldValue = value;
			if (value>param->minValue) {
				value--;
			}
			newValue = value;
		}
		if (newValue != oldValue) {
			propagateNewParamValue(currentRow, encoder, oldValue, newValue);
		}
	} else {
		int oldMenuSelect = fullState.menuSelect;
		if (fullState.menuSelect>0) {
			fullState.menuSelect = fullState.menuSelect - 1;
		}

		if (MENU_LOAD_INTERNAL_BANK) {
			copyPatch((char*)&presets[fullState.menuSelect], (char*)&params);
		}
		if (fullState.menuSelect != oldMenuSelect) {
			propagateNewMenuSelect();
		}
	}
}

void SynthState::copyPatch(char* source, char* dest) {
	for (unsigned int k=0; k<sizeof(struct AllSynthParams); k++) {
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
		case BUTTON_MENU:
			fullState.synthMode = SYNTH_MODE_MENU;
			fullState.currentMenuState = MENU_NONE;
			fullState.menuSelect = 0;
			// allow undo event after trying some patches
			copyPatch((char*)&params, (char*)&backupParams);
			break;
		}
	} else {

		switch (button) {
		case BUTTON_SELECT:
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
						fullState.currentMenuState = MENU_LOAD_INTERNAL_BANK;
						// Load first patch
						copyPatch((char*)&presets[0], (char*)&params);
					} else {
						fullState.currentMenuState = MENU_LOAD_USER_BANK;
					}
					fullState.menuSelect = 0;
					break;
				case MENU_SAVE:
					pruneToEEPROM(fullState.menuSelect);
					break;
				case MENU_LOAD_INTERNAL_BANK:
					fullState.synthMode = SYNTH_MODE_EDIT;
					// without putting back old patch...
					break;
				case MENU_LOAD_USER_BANK:
					readFromEEPROM(fullState.menuSelect);
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
					copyPatch((char*)&backupParams, (char*)&params);
					break;
				default:
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
			SerialUSB.println("\"SoundName\"");
			break;
		}
		case BUTTON_MENU:
			fullState.synthMode = SYNTH_MODE_EDIT;
			// put back old patch (has been overwritten if a new patch has been loaded)
			copyPatch((char*)&backupParams, (char*)&params);

			break;
		}

		// MENU MODE
	}
	if (oldSynthMode != fullState.synthMode) {
		propagateNewSynthMode();
	}
	if (oldCurrentRow != currentRow) {
		propagateNewCurrentRow(currentRow);
	}
	if (oldMenuSelect != fullState.menuSelect) {
		propagateNewMenuSelect();
	}
	if (oldMenuState != fullState.currentMenuState) {
		propagateNewMenuState();
	}
}



void SynthState::pruneToEEPROM(int preset) {
//	uint8 deviceaddress = 0x50;
	uint8 deviceaddress = 0b1010000;
	i2c_msg msgs[2];

	uint8 toSend[32];
	toSend[0] = 0;
	toSend[1] = 0;

	for (unsigned int k=0; k<30; k++) {
		 toSend[k+2] = ((char*)&params)[k];
	}

    msgs[0].addr = deviceaddress;
    msgs[0].flags = 0;
    msgs[0].length = 32;
    msgs[0].data = toSend;

    i2c_master_xfer(I2C1, msgs, 1);
    delay(5);

}


static const uint8 slave_address = 0b1010000;

void SynthState::readFromEEPROM(int preset) {
	//uint8 deviceaddress = 0x50;
	uint8 deviceaddress = 0b1010000;
	uint8 address[] = {0x0, 0x0};
	i2c_msg msgs[2];

	/* Write slave address to read */
    msgs[0].addr = deviceaddress;
    msgs[0].flags = 0;
    msgs[0].length = 2;
    msgs[0].data = address;

    /* Repeated start condition, then read NR_ELEMENTS bytes back */
    msgs[1].addr = deviceaddress;
    msgs[1].flags = I2C_MSG_READ;
    msgs[1].length = 30;
    msgs[1].data = (uint8*)&params;
    i2c_master_xfer(I2C1, msgs, 2);

}
