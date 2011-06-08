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
const char* allChars  = "_ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz 0123456789 &*$,;:";

const char* nullNames []= {};
const char* algoNames []= { "alg1", "alg2", "alg3", "alg4", "alg5", "alg6", "alg7", "alg8", "alg9" };
struct ParameterRowDisplay engine1ParameterRow = {
        "Engine" ,
        { "Algo", "Velo", "Voic", "Glid" },
        {
                {ALGO1, ALGO_END-1, DISPLAY_TYPE_STRINGS, algoNames},
                {0, 16, DISPLAY_TYPE_UNSIGNED_CHAR, nullNames },
                {1, 4, DISPLAY_TYPE_VOICES, nullNames },
                {0, 10, DISPLAY_TYPE_UNSIGNED_CHAR, nullNames }
        }
};

struct ParameterRowDisplay engine2ParameterRow = {
        "Modulation" ,
        { "IM1 ", "IM2 ", "IM3 ", "IM4 "},
        {
                {0, 255, DISPLAY_TYPE_FLOAT_5_3, nullNames },
                {0, 255, DISPLAY_TYPE_FLOAT_5_3, nullNames },
                {0, 255, DISPLAY_TYPE_FLOAT_5_3, nullNames },
                {0, 255, DISPLAY_TYPE_FLOAT_5_3, nullNames }
        }
};

struct ParameterRowDisplay engine3ParameterRow = {
        "Mixer" ,
        { "Mix1", "Mix2", "Mix3", "Mix4" },
        {
                {1, 128, DISPLAY_TYPE_FLOAT_1_7, nullNames },
                {1, 128, DISPLAY_TYPE_FLOAT_1_7, nullNames },
                {1, 128, DISPLAY_TYPE_FLOAT_1_7, nullNames },
                {1, 128, DISPLAY_TYPE_FLOAT_1_7, nullNames }
        }
};

const char* oscShapeNames []=  {"sin ", "s^2 ", "1/2s", "spos", "rand", "off " } ;
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
        { "Attk", "Deca", "Sust", "Rele" },
        {
                { 0, 255, DISPLAY_TYPE_UNSIGNED_CHAR, nullNames },
                { 0, 255, DISPLAY_TYPE_UNSIGNED_CHAR, nullNames },
                { 0, 255, DISPLAY_TYPE_UNSIGNED_CHAR, nullNames },
                { 0, 255, DISPLAY_TYPE_UNSIGNED_CHAR, nullNames }
        }
};


const char* matrixSourceNames [] = { "None", "lfo1", "lfo2", "lfo3", "lfo4", "PitB", "AftT", "ModW", "Velo", "CC1 ", "CC2 ", "CC3 ", "CC4 "} ;
const char* matrixDestNames [] = { "None", "o1Fr", "o2Fr", "o3Fr", "o4Fr", "IM1 ", "IM2 ", "IM3 ", "IM4 ", "Mix1", "Mix2", "Mix3", "Mix4"} ;
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
        { "Shap", "Freq", "Bias", "KSyn" },
        {
                { LFO_SAW, LFO_TYPE_MAX-1, DISPLAY_TYPE_STRINGS,  lfoShapeNames},
                { 0, 255, DISPLAY_TYPE_FLOAT_4_4, nullNames },
                { (char)-127, 127, DISPLAY_TYPE_SIGNED_CHAR, nullNames },
                { 0, 255, DISPLAY_TYPE_UNSIGNED_CHAR, nullNames },
        }
};


struct AllParameterRowsDisplay allParameterRows = {
        {
                &engine1ParameterRow,
                &engine2ParameterRow,
                &engine3ParameterRow,
                &oscParameterRow,
                &oscParameterRow,
                &oscParameterRow,
                &oscParameterRow,
                &oscParameterRow,
                &oscParameterRow,
                &envParameterRow,
                &envParameterRow,
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
                &matrixParameterRow,
                &matrixParameterRow,
                &lfoParameterRow,
                &lfoParameterRow,
                &lfoParameterRow,
                &lfoParameterRow
        }
};


struct ShowUpAlgo showUp[9] = {
        { 3, 3, 0}, // ALGO1
        { 3, 2, 2}, // ALGO2
        { 4, 4, 0}, // ALGO3
        { 4, 4, 2}, // ALGO4
        { 4, 4, 0}, // ALGO5
        { 4, 3, 3}, // ALGO6
        { 6, 4, 3},  // ALGO7
        { 6, 4, 2},  // ALGO8
        { 6, 4, 2}   // ALGO9
};


/******************** PRESET **********************/

//struct AllSynthParams presets[]  =  {{

const struct AllSynthParams presets[] __attribute__ ((section (".USER_FLASH"))) = {
        {
                // Engine
                { ALGO1, 14, 4, 6},
                { 16, 21, 0, 0 },
                { 128, 128, 128,0 },

                // OSC1
                { OSC_SHAPE_SIN, OSC_FT_KEYBOARD, 16,  0 },
                { OSC_SHAPE_SIN, OSC_FT_KEYBOARD, 8,  0 },
                { OSC_SHAPE_SIN, OSC_FT_KEYBOARD, 32, 0 },
                { OSC_SHAPE_SIN, OSC_FT_KEYBOARD, 8,  0 },
                { OSC_SHAPE_SIN, OSC_FT_KEYBOARD, 16,  0 },
                { OSC_SHAPE_SIN, OSC_FT_KEYBOARD, 16,  0 },
                // ENV 1
                { 0,   150, 160,  60 },
                { 100, 65,  150, 100 },
                { 50,  100, 255, 100 },
                { 3,   100, 100, 100 },
                { 0,   150, 160,  60 },
                { 0,   150, 160,  60 },

                // Matrix row

                { MODWHEEL ,   16,   INDEX_MODULATION1, 0 },
                { PITCHBEND,   64,  OSC1_FREQ,   0 },
                { LFO1,        0,   OSC1_FREQ, 0 },
                { SOURCE_NONE, 0,  DESTINATION_NONE, 0 },
                { SOURCE_NONE, 0,  DESTINATION_NONE, 0 },
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
                { 1, 14, 4, 0 },
                { 3, 2, 0, 0 },
                { 128, 128, 128, 0 },
                // OSC1
                { 0, 0, 16, 0} ,
                { 0, 0, 8, 0} ,
                { 0, 0, 32, 0} ,
                { 1, 0, 3, 0} ,
                { OSC_SHAPE_OFF, 0, 0, 0} ,
                { OSC_SHAPE_OFF, 0, 0, 0} ,
                // ENV 1
                { 72, 150, 160, 60} ,
                { 81, 66, 196, 77} ,
                { 50, 100, 255, 100} ,
                { 3, 100, 100, 100} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                // Matrix row
                { 7, 16, 5, 0} ,
                { 5, 64, 1, 0} ,
                { 1, 0, 1, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
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
                { 0, 0, 4, 0},
                { 20, 28, 0, 0 } ,
                { 128, 128, 128,0 },
                // OSC1
                { 0, 0, 16, 0} ,
                { 0, 0, 8, 0} ,
                { 0, 0, 32, 0} ,
                { 1, 0, 3, 0} ,
                { OSC_SHAPE_OFF, 0, 0, 0} ,
                { OSC_SHAPE_OFF, 0, 0, 0} ,
                // ENV 1
                { 0, 150, 160, 60} ,
                { 100, 65, 150, 100} ,
                { 50, 100, 255, 100} ,
                { 3, 100, 100, 100} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                // Matrix row
                { 7, 2, 6, 0} ,
                { 5, 64, 1, 0} ,
                { 1, 0, 1, 0} ,
                { 7, 6, 6, 0} ,
                { 2, 11, 6, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
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
                { 0, 14, 4, 0} ,
                { 15, 3 , 0, 0},
                { 128, 128, 128,0 },
                // OSC1
                { 0, 0, 16, 0} ,
                { 0, 0, 32, 0} ,
                { 0, 0, 1, 0} ,
                { 0, 0, 3, 0} ,
                { OSC_SHAPE_OFF, 0, 0, 0} ,
                { OSC_SHAPE_OFF, 0, 0, 0} ,
                // ENV 1
                { 100, 150, 63, 11} ,
                { 0, 122, 125, 253} ,
                { 50, 100, 255, 100} ,
                { 3, 100, 100, 100} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                // Matrix row
                { 7, 2, 5, 0} ,
                { 5, 64, 1, 0} ,
                { 1, 15, 2, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
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
                { 0, 14, 4, 0},
                { 11, 28, 0, 0} ,
                { 128, 128, 128,0 },
                { 0, 0, 16, 0} ,
                { 0, 0, 8, 0} ,
                { 0, 0, 32, 0} ,
                { 1, 0, 3, 0} ,
                { OSC_SHAPE_OFF, 0, 0, 0} ,
                { OSC_SHAPE_OFF, 0, 0, 0} ,
                // ENV 1

                { 0, 150, 160, 236} ,
                { 0, 66, 196, 77} ,
                { 50, 100, 255, 210} ,
                { 3, 100, 100, 100} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                // Matrix row
                { 7, 6, 5, 0} ,
                { 5, 64, 1, 0} ,
                { 1, 0, 1, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                // LFOS
                { 0, 36, 0, 0} ,
                { 0, 20, 0, 0} ,
                { 0, 3, 0, 0} ,
                { 0, 4, 0, 0} ,
                "Medieval"
        },
        {
                { 3, 8, 3, 0} ,
                { 14, 20, 0, 1} ,
                { 128, 128, 128, 0} ,
                { 0, 0, 16, 0} ,
                { 0, 0, 8, 0} ,
                { 0, 0, 24, 0} ,
                { 0, 0, 4, 0} ,
                { OSC_SHAPE_OFF, 0, 0, 0} ,
                { OSC_SHAPE_OFF, 0, 0, 0} ,

                { 0, 120, 0, 120} ,
                { 0, 136, 0, 130} ,
                { 0, 120, 0, 100} ,
                { 0, 88, 0, 81} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,

                { 7, 8, 5, 0} ,
                { 7, 24, 6, 0} ,
                { 1, 9, 1, 0} ,
                { 5, 40, 2, 0} ,
                { 5, 6, 4, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,

                { 0, 83, 0, 0} ,
                { 0, 20, 0, 0} ,
                { 0, 3, 0, 0} ,
                { 0, 4, 0, 0} ,
                "Klong"
        },
        {
                { 0, 10, 1, 6} ,
                { 16, 36, 0, 81} ,
                { 128, 128, 128, 0} ,
                // Osc
                { 0, 0, 16, 0} ,
                { 0, 0, 8, 0} ,
                { 3, 1, 1, 2} ,
                { 4, 0, 3, 0} ,
                { OSC_SHAPE_OFF, 0, 0, 0} ,
                { OSC_SHAPE_OFF, 0, 0, 0} ,
                // env
                { 0, 253, 0, 49} ,
                { 0, 0, 255, 100} ,
                { 0, 33, 0, 100} ,
                { 3, 100, 100, 100} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,

                { 7, 16, 5, 0} ,
                { 5, 64, 1, 0} ,
                { 1, 0, 1, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,

                { 0, 36, 0, 0} ,
                { 0, 20, 0, 0} ,
                { 0, 3, 0, 0} ,
                { 0, 4, 0, 0} ,
                "Bass 1"
        },
        {
                { 5, 14, 3, 0} ,
                { 19, 42, 84, 0} ,
                { 96, 128, 87, 30} ,
                // Osc
                { 1, 0, 64, 0} ,
                { 0, 0, 8, 0} ,
                { 0, 0, 32, 0} ,
                { 0, 0, 24, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                // Env
                { 0, 150, 0, 60} ,
                { 0, 0, 255, 48} ,
                { 121, 204, 125, 100} ,
                { 100, 100, 100, 92} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                // Matrix
                { 7, 16, 10, 0} ,
                { 7, 13, 5, 0} ,
                { 1, 0, 1, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                // LFO
                { 0, 36, 0, 0} ,
                { 0, 20, 0, 0} ,
                { 0, 3, 0, 0} ,
                { 0, 4, 0, 0} ,
                "Crystal Blow"
        },
        {
                { 6, 14, 2, 0} ,
                { 21, 73, 125, 0} ,
                { 128, 114, 72, 0} ,
                { 1, 0, 41, 0} ,
                { 0, 0, 8, 0} ,
                { 3, 0, 32, 0} ,
                { 0, 0, 8, 0} ,
                { 0, 0, 16, 0} ,
                { 0, 0, 16, 0} ,
                { 0, 150, 160, 60} ,
                { 100, 65, 150, 100} ,
                { 0, 100, 162, 100} ,
                { 79, 100, 171, 100} ,
                { 0, 150, 160, 60} ,
                { 122, 150, 160, 60} ,
                { 7, 16, 5, 0} ,
                { 5, 64, 1, 0} ,
                { 1, 0, 1, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 36, 0, 0} ,
                { 0, 20, 0, 0} ,
                { 0, 3, 0, 0} ,
                { 0, 4, 0, 0} ,
                "6 osc weird"
        },
        {
                { 0, 14, 4, 0} ,
                { 0, 36, 0, 0} ,
                { 128, 128, 128, 0} ,
                { 2, 0, 16, 0} ,
                { 4, 0, 8, 0} ,
                { 0, 0, 4, 0} ,
                { 0, 0, 8, 0} ,
                { 0, 0, 16, 0} ,
                { 0, 0, 16, 0} ,
                { 0, 255, 0, 138} ,
                { 12, 70, 0, 52} ,
                { 0, 100, 129, 255} ,
                { 3, 100, 100, 100} ,
                { 0, 150, 160, 60} ,
                { 0, 150, 160, 60} ,
                { 8, 8, 5, 0} ,
                { 8, 10, 1, 0} ,
                { 1, 6, 1, 0} ,
                { 7, 9, 6, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 36, 0, 0} ,
                { 0, 20, 0, 0} ,
                { 0, 3, 0, 0} ,
                { 0, 4, 0, 0},
                "Old String"
        },
        {
                { 0, 8, 4, 6} ,
                { 7, 21, 19, 0} ,
                { 128, 128, 128, 0} ,
                { 3, 0, 16, 0} ,
                { 0, 0, 4, -127} ,
                { 0, 0, 24, 0} ,
                { 0, 0, 8, 0} ,
                { 0, 0, 16, 0} ,
                { 0, 0, 16, 0} ,
                { 0, 255, 113, 111} ,
                { 0, 96, 0, 0} ,
                { 0, 208, 139, 109} ,
                { 3, 100, 100, 100} ,
                { 0, 150, 160, 60} ,
                { 0, 150, 160, 60} ,
                { 1, 3, 6, 0} ,
                { 5, 22, 1, 0} ,
                { 7, 4, 6, 0} ,
                { 8, 4, 5, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 36, 127, 150} ,
                { 0, 16, 0, 0} ,
                { 0, 16, 0, 0} ,
                { 0, 16, 0, 0} ,
                "Metal dream"
        },
        {
                { 6, 14, 2, 6} ,
                { 22, 52, 57, 0} ,
                { 128, 88, 128, 0} ,
                { 3, 0, 8, 0} ,
                { 0, 0, 23, 0} ,
                { 0, 0, 16, 0} ,
                { 0, 0, 16, -17} ,
                { 2, 0, 48, 0} ,
                { 0, 0, 16, 0} ,
                { 127, 83, 217, 166} ,
                { 44, 95, 36, 209} ,
                { 162, 100, 255, 168} ,
                { 53, 113, 137, 255} ,
                { 138, 150, 160, 159} ,
                { 218, 150, 255, 204} ,
                { 7, 16, 5, 0} ,
                { 5, 64, 1, 0} ,
                { 1, 23, 1, 0} ,
                { 2, 9, 6, 0} ,
                { 0, 0, 0, 0} ,
                { 0, -128, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 36, 0, 128} ,
                { 0, 72, 127, 6} ,
                { 0, 3, 0, 0} ,
                { 0, 4, 0, 0} ,
                "Space Wave"
        },
        {
                { 0,0,0,0} ,
                { 5, 14, 0, 0},
                { 128, 128, 128,0 },
                { 0, 0, 16, 0} ,
                { 0, 0, 8, 0} ,
                { 0, 0, 32, 0} ,
                { 1, 0, 3, 0} ,
                { OSC_SHAPE_OFF, 0, 0, 0} ,
                { OSC_SHAPE_OFF, 0, 0, 0} ,

                { 72, 150, 160, 60} ,
                { 81, 66, 196, 77} ,
                { 50, 100, 255, 100} ,
                { 3, 100, 100, 100} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,

                { 7, 16, 9, 0} ,
                { 5, 64, 1, 0} ,
                { 1, 0, 1, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,

                { 0, 36, 127, 150} ,
                { 0, 16, 0, 0} ,
                { 0, 16, 0, 0} ,
                { 0, 16, 0, 0} ,
                "*"
        }
};


SynthState::SynthState() {
    engineRow =  ROW_ENGINE;
    oscRow    = ROW_OSC1;
    envRow    = ROW_ENV1;
    matrixRow = ROW_MATRIX1;
    lfoRow    = ROW_LFO1;
    // First default preset
    fullState.synthMode = SYNTH_MODE_EDIT;
    fullState.bankNumber = 0;
    fullState.midiChannel = 0;
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
            // Must use newValue (int) so that the minValue comparaison works
            newValue = value + ticks;
            if (ticks>0 && newValue>param->maxValue) {
                newValue = param->maxValue;
            }
            if (ticks<0 && newValue<param->minValue) {
                newValue = param->minValue;
            }
            value = (char)newValue;
        } else {
            unsigned char &value = ((unsigned char*)&params)[num];
            oldValue = value;
            // Must use newValue (int) so that the minValue comparaison works
            newValue = value + ticks;
            if (ticks>0 && newValue>param->maxValue) {
                newValue = param->maxValue;
            }
            if (ticks<0 && newValue<param->minValue) {
                newValue = param->minValue;
            }
            value = (unsigned char)newValue;
        }

        if (newValue != oldValue) {
            propagateNewParamValue(currentRow, encoder, param, oldValue, newValue);
            if (currentRow == ROW_ENGINE && encoder == ENCODER_ENGINE_ALGO) {
                int voiceMax = 4;
                switch (showUp[newValue].osc) {
                case 4:
                    voiceMax = 3;
                    break;
                case 6:
                    voiceMax = 2;
                    break;
                }

                if (params.engine1.numberOfVoice > voiceMax) {
                    setNewValue(ROW_ENGINE, ENCODER_ENGINE_VOICE, voiceMax);
                }
                engine1ParameterRow.params[ENCODER_ENGINE_VOICE].maxValue = voiceMax;
            }
        }
    } else {
        if (encoder==0) {
            int oldMenuSelect = fullState.menuSelect;
            if (ticks>0) {
                if (fullState.menuSelect< fullState.currentMenuItem->maxValue-1) {
                    fullState.menuSelect = fullState.menuSelect + 1;
                }
            } else if (ticks<0) {
                if (fullState.menuSelect>0) {
                    fullState.menuSelect = fullState.menuSelect - 1;
                }
            }
            if (fullState.menuSelect != oldMenuSelect) {
                if (fullState.currentMenuItem->menuState == MENU_LOAD_INTERNAL_BANK) {
                    char* preset = (char*)&(presets[fullState.menuSelect].engine1);
                    copyPatch(preset, (char*)&params, true);
                } else if (fullState.currentMenuItem->menuState == MENU_LOAD_USER_BANK) {
                    propagateBeforeNewParamsLoad();
                    PresetUtil::readFromEEPROM(fullState.bankNumber, fullState.menuSelect);
                }

                propagateNewMenuSelect();
            }
        } else if (encoder==3) {
            if (fullState.currentMenuItem->menuState == MENU_ENTER_NAME) {
                fullState.name[fullState.menuSelect] = (fullState.name[fullState.menuSelect] + (ticks>0? 1: -1));
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
    propagateBeforeNewParamsLoad();

    for (unsigned int k=0; k<sizeof(struct AllSynthParams); k++) {
        int currentRow = k / NUMBER_OF_ENCODERS;
        char oldValue = dest[k];
        dest[k] = source[k];

        if ((currentRow < NUMBER_OF_ROWS) && propagate) {
            int encoder = k % NUMBER_OF_ENCODERS;
            struct ParameterDisplay* param = &(allParameterRows.row[currentRow]->params[encoder]);
            propagateNewParamValue(currentRow, encoder, param, oldValue, source[k]);
        }
    }
}


void SynthState::resetDisplay() {
    fullState.synthMode = SYNTH_MODE_EDIT;
    currentRow = 0;
    propagateNewSynthMode();
}



void SynthState::buttonPressed(int button) {
    SynthMode oldSynthMode = fullState.synthMode;
    int oldCurrentRow = currentRow;
    // int oldMenuSelect = fullState.menuSelect;
    MenuState oldMenuState = fullState.currentMenuItem->menuState;

    if (fullState.synthMode == SYNTH_MODE_EDIT)  {
        switch (button) {
        case BUTTON_SYNTH:
            if (currentRow<ROW_ENGINE_FIRST || currentRow>ROW_ENGINE_LAST) {
                currentRow = engineRow;
            } else {
                currentRow ++;
                if (currentRow>ROW_ENGINE_LAST) {
                    currentRow = ROW_ENGINE_FIRST;
                }
            }
            if (currentRow == ROW_OSC_MIX && showUp[params.engine1.algo].mix == 0) {
                currentRow = ROW_ENGINE_FIRST;
            }
            engineRow = currentRow;
            break;
        case BUTTON_OSC:
            if (currentRow<ROW_OSC_FIRST || currentRow>ROW_OSC_LAST) {
                currentRow = oscRow;
            } else {
                currentRow ++;
            }
            if (currentRow>= (ROW_OSC_FIRST + showUp[params.engine1.algo].osc)) {
                currentRow = ROW_OSC_FIRST;
            }
            oscRow = currentRow;
            break;
        case BUTTON_ENV:
            if (currentRow<ROW_ENV_FIRST || currentRow>ROW_ENV_LAST) {
                currentRow = envRow;
            } else {
                currentRow ++;
            }
            if (currentRow>= (ROW_ENV_FIRST + showUp[params.engine1.algo].osc)) {
                currentRow = ROW_ENV_FIRST;
            }
            envRow = currentRow;
            break;
        case BUTTON_MATRIX:
            if (currentRow<ROW_MATRIX_FIRST || currentRow>ROW_MATRIX_LAST) {
                currentRow = matrixRow;
            } else {
                currentRow ++;
                if (currentRow>ROW_MATRIX_LAST) {
                    currentRow = ROW_MATRIX_FIRST;
                }
            }
            matrixRow = currentRow;
            break;
        case BUTTON_LFO:
            if (currentRow<ROW_LFO_FIRST || currentRow>ROW_LFO_LAST) {
                currentRow = lfoRow;
            } else {
                currentRow ++;
                if (currentRow>ROW_LFO_LAST) {
                    currentRow = ROW_LFO_FIRST;
                }
            }
            lfoRow = currentRow;
            break;
        case BUTTON_MENUSELECT:
            fullState.synthMode = SYNTH_MODE_MENU;
            fullState.menuSelect = 0;
            // allow undo event after trying some patches
            copyPatch((char*)&params, (char*)&backupParams, true);
            fullState.currentMenuItem = MenuItemUtil::getMenuItem(MAIN_MENU);
            break;
        }
    } else {
        switch (button) {
        case BUTTON_MENUSELECT:
            fullState.currentMenuItem = afterButtonPressed();
            break;
        case BUTTON_BACK:
            fullState.currentMenuItem = menuBack();
            propagateMenuBack();
            break;
        case BUTTON_DUMP:
        {
            PresetUtil::dumpPatch();
            break;
        }

        // MENU MODE
        }
    }
    /*
	if (oldMenuSelect != fullState.menuSelect) {
		propagateNewMenuSelect();
	}
     */
    if (oldSynthMode != fullState.synthMode) {
        propagateNewSynthMode();
        return;
    }
    if (oldCurrentRow != currentRow) {
        propagateNewCurrentRow(currentRow);
    }
    if (oldMenuState != fullState.currentMenuItem->menuState) {
        propagateNewMenuState();
    }
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

MenuItem* SynthState::afterButtonPressed() {
    MenuItem* rMenuItem = 0;

    if (fullState.currentMenuItem->hasSubMenu ) {
        rMenuItem = MenuItemUtil::getMenuItem(fullState.currentMenuItem->subMenu[fullState.menuSelect]);
    } else {
        rMenuItem = MenuItemUtil::getMenuItem(fullState.currentMenuItem->subMenu[0]);
    }

    switch (fullState.currentMenuItem->menuState) {
    case MAIN_MENU:
        break;
    case MENU_MIDI_CHANNEL:
        fullState.midiChannel = fullState.menuSelect;
        break;
    case MENU_SAVE_CHOOSE_USER_BANK:
        fullState.bankNumber = fullState.menuSelect;
        break;
    case MENU_LOAD_CHOOSE_USER_BANK:
        fullState.bankNumber = fullState.menuSelect;
        propagateBeforeNewParamsLoad();
        PresetUtil::readFromEEPROM(fullState.bankNumber, 0);
        break;
    case MENU_LOAD_INTERNAL_BANK:
        copyPatch((char*)&params, (char*)&backupParams, true);
        fullState.presetModified = false;
        break;
    case MENU_LOAD_USER_BANK:
        propagateBeforeNewParamsLoad();
        PresetUtil::readFromEEPROM(fullState.bankNumber, fullState.menuSelect);
        copyPatch((char*)&params, (char*)&backupParams, true);
        fullState.presetModified = false;
        break;
    case MENU_SAVE_CHOOSE_PRESET:
        for (int k=0; k<12 && params.presetName[k] != 0; k++) {
            for (int j=0; j<getLength(allChars); j++) {
                if (params.presetName[k] == allChars[j]) {
                    fullState.name[k] = j;
                }
            }
        }
        fullState.presetNumber = fullState.menuSelect;
        break;
    case MENU_ENTER_NAME:
    {
        int length;
        for (length=12; fullState.name[length-1] == 0; length--);
        for (int k=0; k<length; k++) {
            params.presetName[k] = allChars[(int)fullState.name[k]];
        }
        params.presetName[length] = '\0';
        PresetUtil::pruneToEEPROM(fullState.bankNumber, fullState.presetNumber);
        break;
    }
    case MENU_MIDI_PATCH_DUMP:
        PresetUtil::midiPatchDump();
        break;
    case MENU_DONE:
        fullState.synthMode = SYNTH_MODE_EDIT;
        break;
    case MENU_FORMAT_BANK:
        PresetUtil::formatEEPROM();
        break;
    case MENU_LOAD:
        switch (rMenuItem->menuState) {
        case MENU_LOAD_INTERNAL_BANK:
            copyPatch((char*)&presets[0], (char*)&params, true);
            break;
        case MENU_LOAD_USER_BANK:
            break;
        }
        break;
    case MENU_MIDI:
        switch (rMenuItem->menuState) {
        case MENU_MIDI_CHANNEL:
            fullState.menuSelect = fullState.midiChannel;
            // We return now, don't want to set menuSelect to 0
            return rMenuItem;
            break;
        }
        break;
            default:
                break;
    }

    // Save menu select for menuBack Action
    fullState.previousMenuSelect = fullState.menuSelect;
    fullState.menuSelect = 0;
    return rMenuItem;
}


MenuItem* SynthState::menuBack() {
    MenuItem* rMenuItem = 0;

    // default menuSelect value
    fullState.menuSelect = MenuItemUtil::getParentMenuSelect(fullState.currentMenuItem->menuState);

    switch (fullState.currentMenuItem->menuState) {
    case MENU_ENTER_NAME:
        fullState.menuSelect = fullState.presetNumber;
        break;
    case MENU_SAVE_CHOOSE_PRESET:
        fullState.menuSelect = fullState.bankNumber;
        break;
    case MENU_LOAD_USER_BANK:
        fullState.menuSelect = fullState.bankNumber;
        copyPatch((char*)&backupParams, (char*)&params, true);
        break;
    case MENU_LOAD_INTERNAL_BANK:
        copyPatch((char*)&backupParams, (char*)&params, true);
        break;
    case MAIN_MENU:
        fullState.synthMode = SYNTH_MODE_EDIT;
        // put back old patch (has been overwritten if a new patch has been loaded)
        break;
    case MENU_FORMAT_BANK:
    case MENU_LOAD_CHOOSE_USER_BANK:
    case MENU_MIDI:
    case MENU_MIDI_PATCH:
    case MENU_SAVE_CHOOSE_USER_BANK:
    case MENU_MIDI_BANK:
        break;
    }

    rMenuItem = MenuItemUtil::getParentMenuItem(fullState.currentMenuItem->menuState);
    return rMenuItem;
}

