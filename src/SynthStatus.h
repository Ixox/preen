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
#include "EncodersListener.h"



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


struct Oscillator {
	uchar shape; // OSC_SHAPE_*
	uchar frequencyType; // OSC_FT_*
	uchar frequencyMul;
	char  detune;
};

struct Envelope {
	uchar attack;
	uchar decay;
	uchar sustain;
	uchar release;
};

struct LfoState {
	uchar shape; // LFO_SHAPE_*
	uchar freq;  // lfoFreq[]*
	char notused1;
	char notused2;
};

struct MatrixRowState {
	uchar source;
	char mul;
	uchar destination;
	char not_used;
};

struct EngineState {
	uchar algo;
	uchar modulationIndex1;
	uchar modulationIndex2;
	uchar modulationIndex3;
};

#define NUMBER_OF_ROWS 19


struct SynthState {
	struct EngineState engine;
	struct Oscillator osc1;
	struct Oscillator osc2;
	struct Oscillator osc3;
	struct Oscillator osc4;
	struct Envelope env1;
	struct Envelope env2;
	struct Envelope env3;
	struct Envelope env4;
	struct MatrixRowState matrixRowState1;
	struct MatrixRowState matrixRowState2;
	struct MatrixRowState matrixRowState3;
	struct MatrixRowState matrixRowState4;
	struct MatrixRowState matrixRowState5;
	struct MatrixRowState matrixRowState6;
	struct LfoState lfo1;
	struct LfoState lfo2;
	struct LfoState lfo3;
	struct LfoState lfo4;
	const char presetName[13];
};



// Display information

struct Parameter {
	char minValue;
	unsigned char maxValue;
	const char** valueName;
};

struct ParameterRow {
	const char* rowName;
	const char* paramName[4];
	struct Parameter params[4];
};


struct AllParameterRows {
	struct ParameterRow* row[NUMBER_OF_ROWS];
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

class SynthStatus : public EncodersListener {
public:
	SynthStatus();
	void incParameter(int num);
	void decParameter(int num);
	bool isEnvelopeRow(int row) {
		return row >=5 && row<=8;
	}
	bool isMatrixRow(int row) {
		return row>=9 && row<=14;
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

	struct SynthState *state;

private:
	PresetBank bank;
	int preset;

	int oscRow, envRow, matrixRow, lfoRow;
	int currentRow;

	EditMode editMode;

	MenuState currentMenuState;
	int menuSelect;

};

// Global structure used all over the code
extern struct AllParameterRows allParameterRows;
extern SynthStatus	synthStatus;



#endif /* SYNTHSTATUS_H_ */
