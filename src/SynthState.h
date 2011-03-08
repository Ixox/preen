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

#ifndef SYNTHSTATUS_H_
#define SYNTHSTATUS_H_

#include "libmaple_types.h"
#include "wirish.h"
#include "LiquidCrystal.h"
#include "Wire.h"
#include "EncodersListener.h"
#include "SynthParamListener.h"

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


#define NUMBER_OF_ENCODERS 4
#define NUMBER_OF_BUTTONS 6

typedef unsigned char uchar;

enum Algorithm {
	ALGO1 = 0,
	ALGO2,
	ALGO3,
	ALGO4,
	ALGO_END
};

enum OscShape {
	OSC_SHAPE_SIN = 0,
	OSC_SHAPE_OFF
};

enum LfoType {
	LFO_SAW= 0,
	LFO_RAMP,
	LFO_SQUARE,
	LFO_TYPE_MAX
};

enum SourceEnum {
	SOURCE_NONE = 0,
	LFO1,
	LFO2,
	LFO3,
	LFO4,
	PITCHBEND,
	AFTERTOUCH,
	MODWHEEL,
	SOURCE_MAX
};


enum DestinationEnum {
	DESTINATION_NONE = 0,
	OSC1_FREQ,
	OSC2_FREQ,
	OSC3_FREQ,
	OSC4_FREQ,
	OSC2_AMP,
	OSC1_AMP,
	OSC3_AMP,
	OSC4_AMP,
	INDEX_MODULATION1,
	INDEX_MODULATION2,
	INDEX_MODULATION3,
	DESTINATION_MAX
};

enum OscFrequencyType {
	OSC_FT_KEYBOARD = 0,
	OSC_FT_FIXE
};


struct OscillatorParams {
	uchar shape; // OSC_SHAPE_*
	uchar frequencyType; // OSC_FT_*
	uchar frequencyMul;
	char  detune;
};

struct EnvelopeParams {
	uchar attack;
	uchar decay;
	uchar sustain;
	uchar release;
};

struct LfoParams {
	uchar shape; // LFO_SHAPE_*
	uchar freq;  // lfoFreq[]*
	char notused1;
	char notused2;
};

struct MatrixRowParams {
	uchar source;
	char mul;
	uchar destination;
	char not_used;
};

struct EngineParams {
	uchar algo;
	uchar modulationIndex1;
	uchar modulationIndex2;
	uchar modulationIndex3;
};

#define NUMBER_OF_ROWS 19


struct AllSynthParams {
	struct EngineParams engine;
	struct OscillatorParams osc1;
	struct OscillatorParams osc2;
	struct OscillatorParams osc3;
	struct OscillatorParams osc4;
	struct EnvelopeParams env1;
	struct EnvelopeParams env2;
	struct EnvelopeParams env3;
	struct EnvelopeParams env4;
	struct MatrixRowParams matrixRowState1;
	struct MatrixRowParams matrixRowState2;
	struct MatrixRowParams matrixRowState3;
	struct MatrixRowParams matrixRowState4;
	struct MatrixRowParams matrixRowState5;
	struct MatrixRowParams matrixRowState6;
	struct LfoParams lfo1;
	struct LfoParams lfo2;
	struct LfoParams lfo3;
	struct LfoParams lfo4;
	const char presetName[13];
};



// Display information

struct ParameterDisplay {
	char minValue;
	unsigned char maxValue;
	const char** valueName;
};

struct ParameterRowDisplay {
	const char* rowName;
	const char* paramName[4];
	struct ParameterDisplay params[4];
};


struct AllParameterRowsDisplay {
	struct ParameterRowDisplay* row[NUMBER_OF_ROWS];
};


// Class define to allow initalization



enum PresetBank {
	BANK_INTERNAL = 0,
	BANK_USER
};

enum EditMode {
	MODE_EDIT,
	MODE_MENU
};


class SynthState : public EncodersListener {
public:
	SynthState();
	void incParameter(int num);
	void decParameter(int num);

	SynthParamListenerType getListenerType(int row) {
		if (row == 0) {
			return SYNTH_PARAM_ENGINE_LISTENER;
		} else if (row>=1 && row<=4) {
			return SYNTH_PARAM_OSCILLATOR_LISTENER;
		} else if (row>=5 && row<=8) {
			return SYNTH_PARAM_ENVELOPE_LISTENER;
		} else if (row>=9 && row<=14) {
			return SYNTH_PARAM_MATRIX_LISTENER;
		} else if (row>=15 && row<=18) {
			return SYNTH_PARAM_LFO_LISTENER;
		}
		return SYNTH_PARAM_INVALID_LISTENER;
	}

	void buttonPressed(int number);

	int getCurrentRow() {
		return currentRow;
	}
	EditMode getEditMode() {
		return editMode;
	}

	void setBank(PresetBank bank) {
		this->bank = bank;
	}
	void pruneToEEPROM(int preset);
	void readFromEEPROM(int preset);

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

	void insertListener(SynthParamListener *listener) {
		if (firstListener!=0) {
			listener->nextListener = firstListener;
		}
		firstListener = listener;
	}


	struct AllSynthParams params;

private:
	PresetBank bank;
	int preset;

	int oscRow, envRow, matrixRow, lfoRow;
	int currentRow;

	EditMode editMode;

	MenuState currentMenuState;
	int menuSelect;

	SynthParamListener* firstListener;
};

// Global structure used all over the code
extern struct AllParameterRowsDisplay allParameterRows;
extern SynthState	synthState;



#endif /* SYNTHSTATUS_H_ */
