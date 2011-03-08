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
					{ALGO1, ALGO_END-1, algoNames},
					{0, 255, nullNames },
					{0, 255, nullNames },
					{0, 255, nullNames }
		}
};

const char* oscShapeNames []=  {"sin ", "off " } ;
const char* oscTypeNames [] = { "keyb", "fixe"};
struct ParameterRowDisplay oscParameterRow = {
		"Oscillator",
		{ "Shap", "FTyp", "Freq", "FTun" },
		{
					{ OSC_SHAPE_SIN, OSC_SHAPE_OFF, oscShapeNames },
					{ OSC_FT_KEYBOARD, OSC_FT_FIXE, oscTypeNames },
					{ 0, 127, nullNames },
					{ (char)-127, 127, nullNames }
		}
};

struct ParameterRowDisplay envParameterRow = {
		"Enveloppe",
		{ "Attk", "Rele", "Sust", "Deca" },
		{
					{ 0, 255, nullNames },
					{ 0, 255, nullNames },
					{ 0, 255, nullNames },
					{ 0, 255, nullNames }
		}
};


const char* matrixSourceNames [] = { "None", "lfo1", "lfo2", "lfo3", "lfo4", "PitB", "AftT", "ModW"} ;
const char* matrixDestNames [] = { "None", "o1Fr", "o2Fr", "o3Fr", "o4Fr", "o1Am", "o2Am", "o3Am", "o4Am", "IM1 ", "IM2 ", "IM3 "} ;
struct ParameterRowDisplay matrixParameterRow = {
		"Matrix",
		{ "Srce", "Mult", "Dest", "    " },
		{
					{ SOURCE_NONE, SOURCE_MAX-1, matrixSourceNames},
					{ (char)-127, 127, nullNames },
					{ DESTINATION_NONE, DESTINATION_MAX-1, matrixDestNames},
					{ 0, 0, nullNames }
		}
};


const char* lfoShapeNames [] =  { "Saw ", "Ramp", "Squa"} ;
struct ParameterRowDisplay lfoParameterRow = {
		"LFO",
		{ "Shap", "Freq", "    ", "    " },
		{
					{ LFO_SAW, LFO_TYPE_MAX-1, lfoShapeNames},
					{ 0, 255, nullNames },
					{ 0, 0, nullNames },
					{ 0, 0, nullNames }
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
}};
/*
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
*/
SynthState::SynthState() {
	preset = 0;
	bank = BANK_INTERNAL;
	oscRow = 1;
	envRow = 5;
	matrixRow = 9;
	lfoRow = 15;
	// First default preset
	editMode = MODE_EDIT;
	currentRow = 0;
	// enable the i2c bus
	for (unsigned int k=0; k<sizeof(struct AllSynthParams); k++) {
		((char*)&params)[k] = ((char*)presets)[k];
	}
	Wire.begin(2, 3);
}

void SynthState::incParameter(int encoder) {
	if (editMode == MODE_EDIT) {
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
			for (SynthParamListener* listener = firstListener; listener !=0; listener = listener->nextListener) {
				SynthParamListenerType listenerType = getListenerType(currentRow);
				if (listener->getListenerType() == listenerType) {
					listener->newParamValue(getListenerType(currentRow), encoder, oldValue, newValue);
				}
			}
		}
	} else {
		if (currentMenuState == MENU_NONE || currentMenuState == MENU_LOAD) {
			if (menuSelect<1) {
				menuSelect = menuSelect + 1;
			}
		} else {
			if (menuSelect<255) {
				menuSelect = menuSelect + 1;
			}
		}
	}

}

void SynthState::decParameter(int encoder) {
	if (editMode == MODE_EDIT) {
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
			for (SynthParamListener* listener = firstListener; listener !=0; listener = listener->nextListener) {
				SynthParamListenerType listenerType = getListenerType(currentRow);
				if (listener->getListenerType() == listenerType) {
					listener->newParamValue(getListenerType(currentRow), encoder, oldValue, newValue);
				}
			}
		}
	} else {
		if (currentMenuState == MENU_NONE || currentMenuState == MENU_LOAD) {
			if (menuSelect>0) {
				menuSelect = menuSelect - 1;
			}
		} else {
			if (menuSelect>0) {
				menuSelect = menuSelect - 1;
			}
		}
	}
}


void SynthState::buttonPressed(int button) {
	if (editMode == MODE_EDIT)  {
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
			editMode = MODE_MENU;
			currentMenuState = MENU_NONE;
			menuSelect = 0;
			break;
		}
	} else {
		switch (button) {
		case BUTTON_SELECT:
			switch (currentMenuState) {
				case MENU_NONE:
					if (menuSelect == 0) {
						currentMenuState = MENU_LOAD;
					} else {
						currentMenuState = MENU_SAVE;
					}
					menuSelect = 0;
					break;
				case MENU_LOAD:
					if (menuSelect == 0) {
						currentMenuState = MENU_LOAD_INTERNAL_BANK;
					} else {
						currentMenuState = MENU_LOAD_USER_BANK;
					}
					menuSelect = 0;
					break;
				case MENU_SAVE:
					pruneToEEPROM(menuSelect);
					break;
				case MENU_LOAD_INTERNAL_BANK:
					// load internal bank
					break;
				case MENU_LOAD_USER_BANK:
					readFromEEPROM(menuSelect);
					break;
				default:
					break;
			}
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
			editMode = MODE_EDIT;
			break;
		}

		// MENU MODE
	}
}


extern LiquidCrystal lcd;
void SynthState::pruneToEEPROM(int preset) {
	int deviceaddress = 0x50;
	unsigned int eeaddress = preset * 128;
	eeaddress = 0;

//or (unsigned int k=0; k<sizeof(struct SynthState); k++) {
	    Wire.beginTransmission(deviceaddress);
	    Wire.send((int)(eeaddress >> 8)); // MSB
	    Wire.send((int)(eeaddress & 0xFF)); // LSB
	    Wire.send(((uint8*)&params), 32);
	    Wire.endTransmission();
	    delay(10);
	    eeaddress++;
//	}
}


void SynthState::readFromEEPROM(int preset) {
	int deviceaddress = 0x50;
	int eeaddress = preset * 128;
	eeaddress = 0;

//	for (unsigned int k=0; k<sizeof(struct SynthState); k++) {
		Wire.beginTransmission(deviceaddress);
		Wire.send((int)(eeaddress >> 8)); // MSB
		Wire.send((int)(eeaddress & 0xFF)); // LSB
		Wire.endTransmission();
		Wire.requestFrom(eeaddress,32);
		delay(10);
		for (int k=0; k<32; k++) {
			if (Wire.available()) {
				((char*)&params)[k]= Wire.receive();
			}
		}
		delay(10);
		eeaddress++;
//


}
