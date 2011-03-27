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
#include "i2c.h"
#include "EncodersListener.h"
#include "SynthParamListener.h"
#include "SynthMenuListener.h"
#include "Menu.h"

#define BUTTON_SYNTH  0
#define BUTTON_OSC    1
#define BUTTON_ENV    2
#define BUTTON_MATRIX 3
#define BUTTON_LFO    4

#define BUTTON_BACK         5
#define BUTTON_MENUSELECT   6

#define BUTTON_DUMP   0


#define NUMBER_OF_ENCODERS 4
#define NUMBER_OF_BUTTONS 7

enum {
    ENCODER_ENGINE_ALGO = 0,
    ENCODER_ENGINE_IM1,
    ENCODER_ENGINE_IM2,
    ENCODER_ENGINE_IM3
};

enum {
    ENCODER_OSC_SHAP = 0,
    ENCODER_OSC_FTYPE,
    ENCODER_OSC_FREQ,
    ENCODER_OSC_FTUNE
};

enum {
    ENCODER_ENV_A = 0,
    ENCODER_ENV_D,
    ENCODER_ENV_S,
    ENCODER_ENV_R
};

enum {
    ENCODER_MATRIX_SOURCE = 0,
    ENCODER_MATRIX_MUL,
    ENCODER_MATRIX_DEST,
};

enum {
    ENCODER_LFO_SHAPE = 0,
    ENCODER_LFO_FREQ
};

typedef unsigned char uchar;

enum Algorithm {
	ALGO1 = 0,
	ALGO2,
	ALGO3,
	ALGO4,
	ALGO5,
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
	INDEX_MODULATION1,
	INDEX_MODULATION2,
	INDEX_MODULATION3,
	DESTINATION_MAX
};

enum OscFrequencyType {
	OSC_FT_KEYBOARD = 0,
	OSC_FT_FIXE
};

struct Engine1Params {
    uchar algo;
    uchar numberOfVoice;
    uchar velocity;
    uchar glide;
};

struct Engine2Params {
    uchar modulationIndex1;
    uchar modulationIndex2;
    uchar modulationIndex3;
    uchar notused;
};

struct Engine3Params {
    uchar mixOsc1;
    uchar mixOsc2;
    uchar mixOsc3;
    uchar notused;
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




enum {
    ROW_ENGINE_FIRST = 0,
    ROW_ENGINE1 = ROW_ENGINE_FIRST,
    ROW_ENGINE2 ,
    ROW_ENGINE3 ,
    ROW_ENGINE_LAST = ROW_ENGINE3
};

enum {
    ROW_OSC_FIRST = ROW_ENGINE_LAST+1,
    ROW_OSC1 = ROW_OSC_FIRST,
    ROW_OSC2 ,
    ROW_OSC3 ,
    ROW_OSC4 ,
    ROW_OSC_LAST = ROW_OSC4
};


enum {
    ROW_ENV_FIRST = ROW_OSC_LAST+1,
    ROW_ENV1 = ROW_ENV_FIRST,
    ROW_ENV2 ,
    ROW_ENV3 ,
    ROW_ENV4 ,
    ROW_ENV_LAST = ROW_ENV4
};

enum {
    ROW_MATRIX_FIRST = ROW_ENV_LAST+1,
    ROW_MATRIX1 = ROW_MATRIX_FIRST,
    ROW_MATRIX2 ,
    ROW_MATRIX3 ,
    ROW_MATRIX4 ,
    ROW_MATRIX5 ,
    ROW_MATRIX6 ,
    ROW_MATRIX_LAST = ROW_MATRIX6
};

enum {
    ROW_LFO_FIRST = ROW_MATRIX_LAST+1,
    ROW_LFO1 = ROW_LFO_FIRST,
    ROW_LFO2 ,
    ROW_LFO3 ,
    ROW_LFO4 ,
    ROW_LFO_LAST = ROW_LFO4
};

#define NUMBER_OF_ROWS ROW_LFO_LAST+1


struct AllSynthParams {
	struct Engine1Params engine1;
    struct Engine2Params engine2;
    struct Engine3Params engine3;
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
	char presetName[13];
};



// Display information



struct ParameterRowDisplay {
	const char* rowName;
	const char* paramName[4];
	struct ParameterDisplay params[4];
};


struct AllParameterRowsDisplay {
	struct ParameterRowDisplay* row[NUMBER_OF_ROWS];
};


// Class define to allow initalization





class SynthState : public EncodersListener {
public:
	SynthState();
	void encoderTurned(int num, int ticks);

	static SynthParamType getListenerType(int row) {
		if (row>= ROW_ENGINE_FIRST && row<=ROW_ENGINE_LAST) {
			return SYNTH_PARAM_TYPE_ENGINE;
		} else if (row>= ROW_OSC_FIRST && row<=ROW_OSC_LAST) {
			return SYNTH_PARAM_TYPE_OSC;
        } else if (row>= ROW_ENV_FIRST && row<=ROW_ENV_LAST) {
			return SYNTH_PARAM_TYPE_ENV;
        } else if (row>= ROW_MATRIX_FIRST && row<=ROW_MATRIX_LAST) {
			return SYNTH_PARAM_TYPE_MATRIX;
        } else if (row>= ROW_LFO_FIRST && row<=ROW_LFO_LAST) {
			return SYNTH_PARAM_TYPE_LFO;
		}
		return SYNTH_PARAM_TYPE_INVALID;
	}

	void buttonPressed(int number);
	void setNewValue(int row, int number, int newValue);

	MenuItem* afterButtonPressed();
	MenuItem* menuBack();

	int getCurrentRow() {
		return currentRow;
	}


	void insertParamListener(SynthParamListener *listener) {
		if (firstParamListener!=0) {
			listener->nextListener = firstParamListener;
		}
		firstParamListener = listener;
	}

	void insertMenuListener(SynthMenuListener *listener) {
		if (firstMenuListener!=0) {
			listener->nextListener = firstMenuListener;
		}
		firstMenuListener = listener;
	}

	void propagateNewSynthMode() {
		for (SynthMenuListener* listener = firstMenuListener; listener !=0; listener = listener->nextListener) {
			listener->newSynthMode(&fullState);
		}
	}

	void propagateMenuBack() {
		for (SynthMenuListener* listener = firstMenuListener; listener !=0; listener = listener->nextListener) {
			listener->menuBack(&fullState);
		}
	}

	void propagateNewMenuState() {
		for (SynthMenuListener* listener = firstMenuListener; listener !=0; listener = listener->nextListener) {
			listener->newMenuState(&fullState);
		}
	}

	void propagateNewMenuSelect() {
		for (SynthMenuListener* listener = firstMenuListener; listener !=0; listener = listener->nextListener) {
			listener->newMenuSelect(&fullState);
		}
	}

	void propagateNewParamValue(int currentRow, int encoder, ParameterDisplay* param, int oldValue, int newValue) {
		for (SynthParamListener* listener = firstParamListener; listener !=0; listener = listener->nextListener) {
			listener->newParamValue(getListenerType(currentRow), currentRow, encoder, param, oldValue, newValue);
		}
	}

	void propagateNewParamValueFromExternal(int currentRow, int encoder, ParameterDisplay* param, int oldValue, int newValue) {
		for (SynthParamListener* listener = firstParamListener; listener !=0; listener = listener->nextListener) {
			listener->newParamValueFromExternal(getListenerType(currentRow), currentRow, encoder, param, oldValue, newValue);
		}
	}

	void propagateNewCurrentRow(int newCurrentRow) {
		for (SynthParamListener* listener = firstParamListener; listener !=0; listener = listener->nextListener) {
			listener->newcurrentRow(newCurrentRow);
		}
	}

	SynthMode getSynthMode() {
		return fullState.synthMode;
	}

	struct AllSynthParams params;
	struct FullState fullState;

private:
	unsigned char engineRow, oscRow, envRow, matrixRow, lfoRow;
	unsigned char currentRow;


	SynthParamListener* firstParamListener;
	SynthMenuListener* firstMenuListener;
	struct AllSynthParams backupParams;

    void copyPatch(char* source, char* dest, bool propagate);
    void pruneToEEPROM(int bankNumber, int preset);
    void readFromEEPROM(int bankNumber, int preset);
    void formatEEPROM();
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

    int getLength(const char *str) {
        int length = 0;
        for (const char *c = str; *c != '\0'; c++) {
            length++;
        }
        return length;
    }

};

// Global structure used all over the code
extern struct AllParameterRowsDisplay allParameterRows;
extern SynthState	synthState;
extern const struct AllSynthParams presets[];
extern const char* allChars;

#endif /* SYNTHSTATUS_H_ */
