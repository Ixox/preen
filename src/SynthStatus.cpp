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

//unsigned char rawSynthState[] = {
struct SynthState currentSynthState =  {
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
	{ LFO_SAW, 3, 0, 0}, { LFO_SAW, 4, 0, 0 }
};


// DISPLAY structures
const char* nullNames [] = {};
const char* algoNames [] = { "alg1", "alg2", "alg3" };
struct ParameterRow engineParameterRow= {
		{ "Algo", "IM1 ", "IM2 ", "IM3 " },
		{
					{ALGO1, ALGO_END-1, algoNames},
					{0, 255, nullNames },
					{0, 255, nullNames },
					{0, 255, nullNames }
		}
};

const char* oscShapeNames []  {"sin ", "off " } ;
const char* oscTypeNames [] = { "keyb", "fixe"};
struct ParameterRow oscParameterRow= {
		{ "Shap", "FTyp", "Freq", "FTun" },
		{
					{ OSC_SHAPE_SIN, OSC_SHAPE_OFF, oscShapeNames },
					{ OSC_FT_KEYBOARD, OSC_FT_FIXE, oscTypeNames },
					{ 0, 127, nullNames },
					{ (char)-127, 127, nullNames }
		}
};

struct ParameterRow envParameterRow = {
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




