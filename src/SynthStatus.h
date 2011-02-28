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

#ifndef linux
#include "libmaple_types.h"
#include "wirish.h"
#include "LiquidCrystal.h"
#else
typedef short int16;
#endif


typedef unsigned char uchar;

enum Algorithm {
	ALGO1 = 0,
	ALGO2,
	ALGO3,
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
};



// Display information

struct Parameter {
	char minValue;
	unsigned char maxValue;
	const char** valueName;
};

struct ParameterRow {
	const char* paramName[4];
	struct Parameter params[4];
};


struct AllParameterRows {
	struct ParameterRow *row[NUMBER_OF_ROWS];
};


// Class define to allow initalization


extern struct AllParameterRows allParameterRows;
extern struct SynthState currentSynthState;

#ifndef linux
extern LiquidCrystal      lcd;
#endif



#endif /* SYNTHSTATUS_H_ */
