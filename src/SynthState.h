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
#include "EncodersListener.h"
#include "SynthParamListener.h"
#include "SynthMenuListener.h"
#include "PresetUtil.h"
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
    ENCODER_ENGINE_VELOCITY,
    ENCODER_ENGINE_VOICE,
    ENCODER_ENGINE_GLIDE
};

enum {
    ENCODER_ENGINE_IM1 = 0,
    ENCODER_ENGINE_IM2,
    ENCODER_ENGINE_IM3,
    ENCODER_ENGINE_IM4
};

enum {
    ENCODER_ENGINE_MIX1 = 0,
    ENCODER_ENGINE_MIX2,
    ENCODER_ENGINE_MIX3,
    ENCODER_ENGINE_MIX4
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
    ENCODER_LFO_FREQ,
    ENCODER_LFO_BIAS,
    ENCODER_LFO_KSYNC
};

enum {
    ENCODER_STEPSEQ_BPM = 0,
    ENCODER_STEPSEQ_GATE
};


typedef unsigned char uchar;

enum Algorithm {
	ALGO1 = 0,
	ALGO2,
	ALGO3,
	ALGO4,
	ALGO5,
    ALGO6,
    ALGO7,
    ALGO8,
    ALGO9,
	ALGO_END
};

struct ShowUpAlgo {
    unsigned char osc;
    unsigned char im;
    unsigned char mix;
};


enum OscShape {
	OSC_SHAPE_SIN = 0,
    OSC_SHAPE_SIN2,
    OSC_SHAPE_SIN3,
    OSC_SHAPE_SIN4,
    OSC_SHAPE_RAND,
    OSC_SHAPE_SQUARE,
    OSC_SHAPE_SAW,
	OSC_SHAPE_OFF
};

enum LfoType {
	LFO_SAW= 0,
	LFO_RAMP,
	LFO_SQUARE,
	LFO_RANDOM,
	LFO_TYPE_MAX
};

enum SourceEnum {
	MATRIX_SOURCE_NONE = 0,
	MATRIX_SOURCE_LFO1,
	MATRIX_SOURCE_LFO2,
	MATRIX_SOURCE_LFO3,
	MATRIX_SOURCE_LFO4,
	MATRIX_SOURCE_PITCHBEND,
	MATRIX_SOURCE_AFTERTOUCH,
	MATRIX_SOURCE_MODWHEEL,
    MATRIX_SOURCE_VELOCITY,
    MATRIX_SOURCE_CC1,
    MATRIX_SOURCE_CC2,
    MATRIX_SOURCE_CC3,
    MATRIX_SOURCE_CC4,
	MATRIX_SOURCE_LFO5,
	MATRIX_SOURCE_LFO6,
	MATRIX_SOURCE_MAX
};


enum DestinationEnum {
	DESTINATION_NONE = 0,
	OSC1_FREQ,
	OSC2_FREQ,
	OSC3_FREQ,
	OSC4_FREQ,
	OSC5_FREQ,
	OSC6_FREQ,
	INDEX_MODULATION1,
	INDEX_MODULATION2,
	INDEX_MODULATION3,
    INDEX_MODULATION4,
	MIX_OSC1,
    MIX_OSC2,
    MIX_OSC3,
    MIX_OSC4,
	LFO1_FREQ,
	LFO2_FREQ,
	LFO3_FREQ,
	LFO4_FREQ,
	MTX1_MUL,
	MTX2_MUL,
	MTX3_MUL,
	MTX4_MUL,
	MTX5_MUL,
	MTX6_MUL,
	MTX7_MUL,
	MTX8_MUL,
	MTX9_MUL,
	MTX10_MUL,
	MTX11_MUL,
	MTX12_MUL,
	ALL_OSC_FREQ,
	LFO5_GATE,
	LFO6_GATE,
	DESTINATION_MAX
};

enum OscFrequencyType {
	OSC_FT_KEYBOARD = 0,
	OSC_FT_FIXE
};

struct Engine1Params {
    uchar algo;
    uchar velocity;
    uchar numberOfVoice;
    uchar glide;
};

struct Engine2Params {
    uchar modulationIndex1;
    uchar modulationIndex2;
    uchar modulationIndex3;
    uchar modulationIndex4;
};

struct Engine3Params {
    uchar mixOsc1;
    uchar mixOsc2;
    uchar mixOsc3;
    uchar mixOsc4;
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
    char bias;
	uchar keybRamp;
};

struct MatrixRowParams {
	uchar source;
	char mul;
	uchar destination;
	char not_used;
};

struct StepSequencerParams {
	uchar bpm;
	uchar gate;
	uchar unused1;
	uchar unused2;
};

struct StepSequencerSteps {
	char steps[16];
};

enum {
    ROW_ENGINE_FIRST = 0,
    ROW_ENGINE = ROW_ENGINE_FIRST,
    ROW_MODULATION ,
    ROW_OSC_MIX,
    ROW_ENGINE_LAST = ROW_OSC_MIX
};

enum {
    ROW_OSC_FIRST = ROW_ENGINE_LAST+1,
    ROW_OSC1 = ROW_OSC_FIRST,
    ROW_OSC2 ,
    ROW_OSC3 ,
    ROW_OSC4 ,
    ROW_OSC5 ,
    ROW_OSC6 ,
    ROW_OSC_LAST = ROW_OSC6
};


enum {
    ROW_ENV_FIRST = ROW_OSC_LAST+1,
    ROW_ENV1 = ROW_ENV_FIRST,
    ROW_ENV2 ,
    ROW_ENV3 ,
    ROW_ENV4 ,
    ROW_ENV5 ,
    ROW_ENV6 ,
    ROW_ENV_LAST = ROW_ENV6
};

enum {
    ROW_MATRIX_FIRST = ROW_ENV_LAST+1,
    ROW_MATRIX1 = ROW_MATRIX_FIRST,
    ROW_MATRIX2 ,
    ROW_MATRIX3 ,
    ROW_MATRIX4 ,
    ROW_MATRIX5 ,
    ROW_MATRIX6 ,
    ROW_MATRIX7 ,
    ROW_MATRIX8 ,
    ROW_MATRIX9 ,
    ROW_MATRIX10 ,
    ROW_MATRIX11 ,
    ROW_MATRIX12 ,
    ROW_MATRIX_LAST = ROW_MATRIX12
};

enum {
    ROW_LFO_FIRST = ROW_MATRIX_LAST+1,
    ROW_LFO1 = ROW_LFO_FIRST,
    ROW_LFO2 ,
    ROW_LFO3 ,
    ROW_LFO4 ,
    ROW_LFO5 ,
    ROW_LFO6 ,
    ROW_LFO_LAST = ROW_LFO6
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
    struct OscillatorParams osc5;
    struct OscillatorParams osc6;
	struct EnvelopeParams env1;
	struct EnvelopeParams env2;
	struct EnvelopeParams env3;
	struct EnvelopeParams env4;
    struct EnvelopeParams env5;
    struct EnvelopeParams env6;
	struct MatrixRowParams matrixRowState1;
	struct MatrixRowParams matrixRowState2;
	struct MatrixRowParams matrixRowState3;
	struct MatrixRowParams matrixRowState4;
	struct MatrixRowParams matrixRowState5;
	struct MatrixRowParams matrixRowState6;
    struct MatrixRowParams matrixRowState7;
    struct MatrixRowParams matrixRowState8;
	struct MatrixRowParams matrixRowState9;
	struct MatrixRowParams matrixRowState10;
    struct MatrixRowParams matrixRowState11;
    struct MatrixRowParams matrixRowState12;
	struct LfoParams lfo1;
	struct LfoParams lfo2;
	struct LfoParams lfo3;
	struct EnvelopeParams lfo4;
	struct StepSequencerParams lfo5;
	struct StepSequencerParams lfo6;
	struct StepSequencerSteps steps5;
	struct StepSequencerSteps steps6;
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
	void encoderTurnedForStepSequencer(int row, int num, int ticks);

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

	void changeSynthModeRow(int button, int step);
	void buttonPressed(int number);
	void buttonLongPressed(int number);
	void setNewValue(int row, int number, int newValue);
	void setNewStepValue(int whichStepSeq, int step, int newValue);

	const MenuItem* afterButtonPressed();
	const MenuItem* menuBack();

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
        fullState.presetModified = true;
		for (SynthParamListener* listener = firstParamListener; listener !=0; listener = listener->nextListener) {
			listener->newParamValue(getListenerType(currentRow), currentRow, encoder, param, oldValue, newValue);
		}
	}

	void propagateNewParamValueFromExternal(int currentRow, int encoder, ParameterDisplay* param, int oldValue, int newValue) {
        fullState.presetModified = true;
		for (SynthParamListener* listener = firstParamListener; listener !=0; listener = listener->nextListener) {
			listener->newParamValueFromExternal(getListenerType(currentRow), currentRow, encoder, param, oldValue, newValue);
		}
	}

	void propagateNewCurrentRow(int newCurrentRow) {
		for (SynthParamListener* listener = firstParamListener; listener !=0; listener = listener->nextListener) {
			listener->newcurrentRow(newCurrentRow);
		}
	}

    void propagateBeforeNewParamsLoad() {
        for (SynthParamListener* listener = firstParamListener; listener !=0; listener = listener->nextListener) {
            listener->beforeNewParamsLoad();
        }
    }

    void propagateNoteOff() {
        for (SynthParamListener* listener = firstParamListener; listener !=0; listener = listener->nextListener) {
            listener->stopNote(playingNote);
        }
		this->isPlayingNote = false;
    }

    void propagateNoteOn() {
        for (SynthParamListener* listener = firstParamListener; listener !=0; listener = listener->nextListener) {
            listener->playNote(fullState.midiConfigValue[MIDICONFIG_TEST_NOTE], fullState.midiConfigValue[MIDICONFIG_TEST_VELOCITY]);
        }
		this->isPlayingNote = true;
		playingNote = fullState.midiConfigValue[MIDICONFIG_TEST_NOTE];
    }

    void propagateAfterNewParamsLoad();

    SynthMode getSynthMode() {
		return fullState.synthMode;
	}

    void newBankReady();

    void tempoClick();

    void resetDisplay();
	struct AllSynthParams params;
    struct AllSynthParams backupParams;
	struct FullState fullState;
	char stepSelect[2];

private:
	char engineRow, operatorRow, matrixRow, lfoRow;
	char currentRow;
	unsigned char lastButtonSelected;
	boolean isPlayingNote ;
	char playingNote;

	// Done menu temporisation
	uint32 doneClick;

	SynthParamListener* firstParamListener;
	SynthMenuListener* firstMenuListener;


    int getLength(const char *str) {
        int length = 0;
        for (const char *c = str; *c != '\0'; c++) {
            length++;
        }
        return length;
    }

};

// Global structure used all over the code
extern const struct AllParameterRowsDisplay allParameterRows;
extern const struct AllSynthParams presets[];
extern const char* allChars;
extern const struct ShowUpAlgo showUp[];


#endif /* SYNTHSTATUS_H_ */
