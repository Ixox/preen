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

#include "SynthStatus.h"



// FLASH :  __attribute__ ((section (".USER_FLASH")))
// Ex : const char* nullNames [] __attribute__ ((section (".USER_FLASH")))= {};
// DISPLAY structures
const char* nullNames []= {};
const char* algoNames []= { "alg1", "alg2", "alg3", "alg4" };
struct ParameterRow engineParameterRow= {
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
struct ParameterRow oscParameterRow = {
		"Oscillator",
		{ "Shap", "FTyp", "Freq", "FTun" },
		{
					{ OSC_SHAPE_SIN, OSC_SHAPE_OFF, oscShapeNames },
					{ OSC_FT_KEYBOARD, OSC_FT_FIXE, oscTypeNames },
					{ 0, 127, nullNames },
					{ (char)-127, 127, nullNames }
		}
};

struct ParameterRow envParameterRow = {
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
struct ParameterRow matrixParameterRow = {
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
struct ParameterRow lfoParameterRow = {
		"LFO",
		{ "Shap", "Freq", "    ", "    " },
		{
					{ LFO_SAW, LFO_TYPE_MAX-1, lfoShapeNames},
					{ 0, 255, nullNames },
					{ 0, 0, nullNames },
					{ 0, 0, nullNames }
		}
};


struct AllParameterRows allParameterRows = {
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

struct SynthState presets[]  =  {{
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
SynthStatus::SynthStatus() {
	preset = 0;
	bank = BANK_INTERNAL;
	oscRow = 1;
	envRow = 5;
	matrixRow = 9;
	lfoRow = 15;
	// First default preset
	state =  &presets[0];
	editMode = MODE_EDIT;
	currentRow = 0;
}

void SynthStatus::incParameter(int encoder) {
	if (editMode == MODE_EDIT) {
		int num = currentRow * NUMBER_OF_ENCODERS + encoder;
		struct Parameter* param = &(allParameterRows.row[currentRow]->params[encoder]);
		int newValue;
		int oldValue;
		if (param->minValue<0) {
			char &value = ((char*)state)[num];
			oldValue = value;
			if (value<param->maxValue) {
				value++;
			}
			newValue = value;
		} else {
			unsigned char &value = ((unsigned char*)synthStatus.state)[num];
			oldValue = value;
			if (value<param->maxValue) {
				value++;
			}
			newValue = value;
		}
		if (newValue != oldValue) {
			if (isMatrixRow(currentRow)) {
				// synth->getMatrix()->reinitUsage(k, oldValue, newValue);
			}
			if (isEnvelopeRow(currentRow)) {
				// synth->reloadADSR();
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

void SynthStatus::decParameter(int encoder) {
	if (editMode == MODE_EDIT) {
		int num = currentRow * NUMBER_OF_ENCODERS + encoder;
		struct Parameter* param = &(allParameterRows.row[currentRow]->params[encoder]);
		int newValue;
		int oldValue;
		if (param->minValue<0) {
			char &value = ((char*)state)[num];
			oldValue = value;
			if (value>param->minValue) {
				value--;
			}
			newValue = value;
		} else {
			unsigned char &value = ((unsigned char*)synthStatus.state)[num];
			oldValue = value;
			if (value>param->minValue) {
				value--;
			}
			newValue = value;
		}
		if (newValue != oldValue) {
			if (isMatrixRow(currentRow)) {
				// TO REAAD !!!!!!!!!!!!!!!!!!
	//			synth->getMatrix()->reinitUsage(encoder, oldValue, newValue);
			}
			if (isEnvelopeRow(currentRow)) {
				// TO REAAD !!!!!!!!!!!!!!!!!!
	//			synth->reloadADSR();
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


void SynthStatus::buttonPressed(int button) {
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
			dumpLine(synthStatus.state->engine.algo, synthStatus.state->engine.modulationIndex1, synthStatus.state->engine.modulationIndex2, synthStatus.state->engine.modulationIndex3 );
			Oscillator * o = (Oscillator *)(&(synthStatus.state->osc1));
			for (int k=0; k<4; k++) {
				dumpLine(o[k].shape, o[k].frequencyType, o[k].frequencyMul, o[k].detune);
			}
			Envelope * e = (Envelope*)(&(synthStatus.state->env1));
			for (int k=0; k<4; k++) {
				dumpLine(e[k].attack, e[k].decay, e[k].sustain, e[k].release);
			}
			MatrixRowState* m = (MatrixRowState*)(&(synthStatus.state->matrixRowState1));
			for (int k=0; k<6; k++) {
				dumpLine(m[k].source, m[k].mul, m[k].destination, 0);
			}
			LfoState* l = (LfoState*)(&(synthStatus.state->lfo1));
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

/*
 * 	if (!menuMode) {
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
				if (b2) {
					decParameter(index+k);
				} else {
					incParameter(index+k);
				}
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
					dumpLine(synthStatus.state->engine.algo, synthStatus.state->engine.modulationIndex1, synthStatus.state->engine.modulationIndex2, synthStatus.state->engine.modulationIndex3 );
					Oscillator * o = (Oscillator *)(&(synthStatus.state->osc1));
					for (int k=0; k<4; k++) {
						dumpLine(o[k].shape, o[k].frequencyType, o[k].frequencyMul, o[k].detune);
					}
					Envelope * e = (Envelope*)(&(synthStatus.state->env1));
					for (int k=0; k<4; k++) {
						dumpLine(e[k].attack, e[k].decay, e[k].sustain, e[k].release);
					}
					MatrixRowState* m = (MatrixRowState*)(&(synthStatus.state->matrixRowState1));
					for (int k=0; k<6; k++) {
						dumpLine(m[k].source, m[k].mul, m[k].destination, 0);
					}
					LfoState* l = (LfoState*)(&(synthStatus.state->lfo1));
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

 */

