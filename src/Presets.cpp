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



const struct AllSynthParams presets[]  = {
        {
        		// patch name : 'Preen'
        		// Engine
        		{ ALGO1, 14, 4, 6} ,
        		{ 16, 21, 0, 0} ,
        		{ 128, 128, 128, 0} ,
        		// Oscillator
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 32, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		// Enveloppe
        		{ 0, 150, 160, 60} ,
        		{ 100, 65, 150, 100} ,
        		{ 50, 100, 255, 100} ,
        		{ 3, 100, 100, 100} ,
        		{ 0, 150, 160, 60} ,
        		{ 0, 150, 160, 60} ,
        		// Modulation matrix
        		{ MATRIX_SOURCE_MODWHEEL, 16, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_PITCHBEND, 64, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_LFO1, 0, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		// LFOs
        		{ LFO_SAW, 36, 0, 0} ,
        		{ LFO_SAW, 20, 0, 0} ,
        		{ LFO_SAW, 3, 0, 0} ,
        		{ 0, 4, 0, 0} ,
        		{ 70, 12,  0, 0}  ,
        		{ 140, 18, 0, 0},
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 ,0}} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15, 0}} ,
        		"Preen"
        } ,
        {
        		// patch name : 'Bubble mw'
        		// Engine
        		{ ALGO1, 0, 4, 0} ,
        		{ 20, 28, 0, 0} ,
        		{ 128, 128, 128, 0} ,
        		// Oscillator
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 32, 0} ,
        		{ OSC_SHAPE_OFF, OSC_FT_KEYBOARD , 0, 0} ,
        		{ OSC_SHAPE_OFF, OSC_FT_KEYBOARD , 0, 0} ,
        		{ OSC_SHAPE_OFF, OSC_FT_KEYBOARD , 0, 0} ,
        		// Enveloppe
        		{ 0, 150, 160, 60} ,
        		{ 100, 65, 150, 100} ,
        		{ 50, 100, 255, 100} ,
        		{ 3, 100, 100, 100} ,
        		{ 0, 0, 0, 0} ,
        		{ 0, 0, 0, 0} ,
        		// Modulation matrix
        		{ MATRIX_SOURCE_MODWHEEL, 2, INDEX_MODULATION2, 0} ,
        		{ MATRIX_SOURCE_PITCHBEND, 64, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_LFO1, 0, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_MODWHEEL, 6, INDEX_MODULATION2, 0} ,
        		{ MATRIX_SOURCE_LFO2, 11, INDEX_MODULATION2, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		// LFOs
        		{ LFO_SQUARE, 57, 0, 0} ,
        		{ LFO_SQUARE, 97, 0, 0} ,
        		{ LFO_SQUARE, 96, 0, 0} ,
        		{ 0, 12, 0, 0} ,
        		{ 70, 12,  0, 0}  ,
        		{ 140, 18, 0, 0},
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 ,0}} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15, 0}} ,
        		"Bubble  mw"

        },
        {
        		// patch name : 'Old and Sad'
        		// Engine
        		{ ALGO1, 0, 4, 0} ,
        		{ 15, 3, 0, 0} ,
        		{ 128, 128, 128, 0} ,
        		// Oscillator
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 32, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 1, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 3, 0} ,
        		{ OSC_SHAPE_OFF, OSC_FT_KEYBOARD , 0, 0} ,
        		{ OSC_SHAPE_OFF, OSC_FT_KEYBOARD , 0, 0} ,
        		// Enveloppe
        		{ 100, 150, 111, 11} ,
        		{ 0, 122, 125, 253} ,
        		{ 50, 100, 255, 100} ,
        		{ 3, 100, 100, 100} ,
        		{ 0, 0, 0, 0} ,
        		{ 0, 0, 0, 0} ,
        		// Modulation matrix
        		{ MATRIX_SOURCE_MODWHEEL, 2, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_PITCHBEND, 64, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_LFO1, 15, OSC2_FREQ, 0} ,
        		{ MATRIX_SOURCE_LFO2, 5, OSC2_FREQ, 0} ,
        		{ MATRIX_SOURCE_LFO2, 5, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		// LFOs
        		{ LFO_SAW, 23, 0, 0} ,
        		{ LFO_RANDOM, 40, 0, 0} ,
        		{ LFO_SAW, 3, 0, 0} ,
        		{ 0, 4, 0, 0} ,
        		{ 70, 12,  0, 0}  ,
        		{ 140, 18, 0, 0},
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 ,0}} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15, 0}} ,
        		"Old and Sad"
        },
        {
        		// patch name : 'Power'
        		// Engine
        		{ ALGO8, 14, 2, 0} ,
        		{ 32, 44, 156, 56} ,
        		{ 128, 128, 128, 0} ,
        		// Oscillator
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 32, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 48, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 80, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SQUARE, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 4, 0} ,
        		// Enveloppe
        		{ 0, 90, 209, 90} ,
        		{ 94, 75, 174, 77} ,
        		{ 0, 179, 0, 173} ,
        		{ 3, 100, 118, 100} ,
        		{ 15, 91, 188, 111} ,
        		{ 0, 130, 87, 108} ,
        		// Modulation matrix
        		{ MATRIX_SOURCE_MODWHEEL, 10, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_PITCHBEND, 12, OSC2_FREQ, 0} ,
        		{ MATRIX_SOURCE_LFO1, 1, INDEX_MODULATION2, 0} ,
        		{ MATRIX_SOURCE_LFO2, 13, INDEX_MODULATION3, 0} ,
        		{ MATRIX_SOURCE_LFO4, 14, LFO2_FREQ, 0} ,
        		{ MATRIX_SOURCE_MODWHEEL, 11, INDEX_MODULATION3, 0} ,
        		{ MATRIX_SOURCE_NONE, 49, INDEX_MODULATION4, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		// LFOs
        		{ LFO_SAW, 36, 0, 0} ,
        		{ LFO_SAW, 36, 127, 255} ,
        		{ LFO_SAW, 3, 0, 0} ,
        		{ 255, 4, -1, 255} ,
        		{ 70, 12,  0, 0}  ,
        		{ 140, 18, 0, 0},
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 ,0}} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15, 0}} ,
        		"Power"
        },
        {
        		// patch name : 'Organ'
        		// Engine
        		{ ALGO6, 0, 3, 0} ,
        		{ 8, 13, 19, 0} ,
        		{ 128, 128, 128, 0} ,
        		// Oscillator
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 32, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_OFF, OSC_FT_KEYBOARD , 0, 0} ,
        		{ OSC_SHAPE_OFF, OSC_FT_KEYBOARD , 0, 0} ,
        		// Enveloppe
        		{ 70, 89, 145, 60} ,
        		{ 84, 66, 194, 60} ,
        		{ 90, 87, 150, 60} ,
        		{ 0, 0, 255, 255} ,
        		{ 0, 0, 0, 0} ,
        		{ 0, 0, 0, 0} ,
        		// Modulation matrix
        		{ MATRIX_SOURCE_MODWHEEL, 16, INDEX_MODULATION3, 0} ,
        		{ MATRIX_SOURCE_PITCHBEND, 64, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_LFO1, 0, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		// LFOs
        		{ LFO_SAW, 36, 0, 0} ,
        		{ LFO_SAW, 20, 0, 0} ,
        		{ LFO_SAW, 3, 0, 0} ,
        		{ 0, 4, 0, 0} ,
        		{ 70, 12,  0, 0}  ,
        		{ 140, 18, 0, 0},
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 ,0}} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15, 0}} ,
        		"Organ"
        },
        {
        		// patch name : 'Klong'
        		// Engine
        		{ ALGO4, 8, 3, 0} ,
        		{ 44, 89, 31, 7} ,
        		{ 128, 128, 128, 0} ,
        		// Oscillator
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 24, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 4, 0} ,
        		{ OSC_SHAPE_OFF, OSC_FT_KEYBOARD , 0, 0} ,
        		{ OSC_SHAPE_OFF, OSC_FT_KEYBOARD , 0, 0} ,
        		// Enveloppe
        		{ 0, 120, 0, 120} ,
        		{ 0, 130, 0, 130} ,
        		{ 0, 0, 255, 255} ,
        		{ 0, 88, 0, 81} ,
        		{ 0, 0, 0, 0} ,
        		{ 0, 0, 0, 0} ,
        		// Modulation matrix
        		{ MATRIX_SOURCE_MODWHEEL, 8, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_MODWHEEL, 11, INDEX_MODULATION2, 0} ,
        		{ MATRIX_SOURCE_LFO1, 14, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		// LFOs
        		{ LFO_SAW, 83, 0, 0} ,
        		{ LFO_SAW, 20, 0, 0} ,
        		{ LFO_SAW, 3, 0, 0} ,
        		{ 0, 0, 0, 0} ,
        		{ 70, 12,  0, 0}  ,
        		{ 140, 18, 0, 0},
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 ,0}} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15, 0}} ,
        		"Klong"
        },
        {
        		// patch name : 'Simple Bass'
        		// Engine
        		{ ALGO1, 10, 1, 6} ,
        		{ 16, 36, 0, 81} ,
        		{ 128, 128, 128, 0} ,
        		// Oscillator
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN4, OSC_FT_FIXE, 1, 2} ,
        		{ OSC_SHAPE_RAND, OSC_FT_KEYBOARD , 3, 0} ,
        		{ OSC_SHAPE_OFF, OSC_FT_KEYBOARD , 0, 0} ,
        		{ OSC_SHAPE_OFF, OSC_FT_KEYBOARD , 0, 0} ,
        		// Enveloppe
        		{ 0, 253, 0, 49} ,
        		{ 0, 0, 255, 100} ,
        		{ 0, 33, 0, 100} ,
        		{ 3, 100, 100, 100} ,
        		{ 0, 0, 0, 0} ,
        		{ 0, 0, 0, 0} ,
        		// Modulation matrix
        		{ MATRIX_SOURCE_MODWHEEL, 16, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_PITCHBEND, 64, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_LFO1, 0, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		// LFOs
        		{ LFO_SAW, 36, 0, 0} ,
        		{ LFO_SAW, 20, 0, 0} ,
        		{ LFO_SAW, 3, 0, 0} ,
        		{ 0, 4, 0, 0} ,
        		{ 70, 12,  0, 0}  ,
        		{ 140, 18, 0, 0},
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 ,0}} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15, 0}} ,
        		"Simple Bass"
        },
        {
        		// patch name : 'Medieval'
        		// Engine
        		{ ALGO1, 14, 4, 0} ,
        		{ 27, 28, 0, 0} ,
        		{ 128, 128, 128, 0} ,
        		// Oscillator
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 12, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 32, 0} ,
        		{ OSC_SHAPE_SIN2, OSC_FT_KEYBOARD , 3, 0} ,
        		{ OSC_SHAPE_OFF, OSC_FT_KEYBOARD , 0, 0} ,
        		{ OSC_SHAPE_OFF, OSC_FT_KEYBOARD , 0, 0} ,
        		// Enveloppe
        		{ 0, 0, 255, 175} ,
        		{ 0, 66, 196, 132} ,
        		{ 50, 100, 255, 210} ,
        		{ 3, 100, 100, 100} ,
        		{ 0, 0, 0, 0} ,
        		{ 0, 0, 0, 0} ,
        		// Modulation matrix
        		{ MATRIX_SOURCE_MODWHEEL, 6, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_PITCHBEND, 3, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_LFO1, 2, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		// LFOs
        		{ LFO_SAW, 38, 0, 117} ,
        		{ LFO_SAW, 20, 0, 0} ,
        		{ LFO_SAW, 3, 0, 0} ,
        		{ 0, 0, 0, 0} ,
        		{ 70, 12,  0, 0}  ,
        		{ 140, 18, 0, 0},
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 ,0}} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15, 0}} ,
        		"Medieval"
        },
        {
        		// patch name : 'Crystal Blow'
        		// Engine
        		{ ALGO6, 14, 3, 0} ,
        		{ 19, 42, 84, 0} ,
        		{ 96, 128, 87, 30} ,
        		// Oscillator
        		{ OSC_SHAPE_SIN2, OSC_FT_KEYBOARD , 64, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 32, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 24, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 0, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 0, 0} ,
        		// Enveloppe
        		{ 0, 150, 0, 60} ,
        		{ 0, 0, 255, 48} ,
        		{ 121, 204, 125, 100} ,
        		{ 100, 100, 100, 92} ,
        		{ 0, 0, 0, 0} ,
        		{ 0, 0, 0, 0} ,
        		// Modulation matrix
        		{ MATRIX_SOURCE_MODWHEEL, 16, MIX_OSC2, 0} ,
        		{ MATRIX_SOURCE_MODWHEEL, 13, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_LFO1, 0, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		// LFOs
        		{ LFO_SAW, 36, 0, 0} ,
        		{ LFO_SAW, 20, 0, 0} ,
        		{ LFO_SAW, 3, 0, 0} ,
        		{ 0, 0, 0, 0} ,
        		{ 70, 12,  0, 0}  ,
        		{ 140, 18, 0, 0},
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 ,0}} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15, 0}} ,
        		"Crystal Blow"
        },
        {
        		// patch name : '6 osc first'
        		// Engine
        		{ ALGO7, 14, 2, 0} ,
        		{ 53, 64, 41, 0} ,
        		{ 88, 128, 113, 0} ,
        		// Oscillator
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 24, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN4, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 4, 0} ,
        		{ OSC_SHAPE_SQUARE, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 12, 0} ,
        		// Enveloppe
        		{ 0, 80, 173, 60} ,
        		{ 100, 108, 154, 255} ,
        		{ 0, 100, 162, 100} ,
        		{ 79, 100, 171, 100} ,
        		{ 0, 150, 160, 60} ,
        		{ 0, 155, 79, 60} ,
        		// Modulation matrix
        		{ MATRIX_SOURCE_MODWHEEL, 32, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_PITCHBEND, 64, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_LFO1, 12, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_LFO4, 16, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		// LFOs
        		{ LFO_SAW, 36, 0, 140} ,
        		{ LFO_SAW, 20, 0, 0} ,
        		{ LFO_SAW, 3, 0, 0} ,
        		{ 108, 103, 0, 0} ,
        		{ 70, 12,  0, 0}  ,
        		{ 140, 18, 0, 0},
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 ,0}} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15, 0}} ,
        		"6 osc first"
        },
        {
        		// patch name : 'Old String'
        		// Engine
        		{ ALGO1, 14, 4, 0} ,
        		{ 0, 36, 0, 0} ,
        		{ 128, 128, 128, 0} ,
        		// Oscillator
        		{ OSC_SHAPE_SIN3, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_RAND, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 4, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		// Enveloppe
        		{ 0, 255, 0, 138} ,
        		{ 12, 70, 0, 52} ,
        		{ 0, 100, 129, 255} ,
        		{ 3, 100, 100, 100} ,
        		{ 0, 150, 160, 60} ,
        		{ 0, 150, 160, 60} ,
        		// Modulation matrix
        		{ MATRIX_SOURCE_VELOCITY, 8, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_VELOCITY, 10, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_LFO1, 6, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_MODWHEEL, 9, INDEX_MODULATION2, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		// LFOs
        		{ LFO_SAW, 36, 0, 0} ,
        		{ LFO_SAW, 20, 0, 0} ,
        		{ LFO_SAW, 3, 0, 0} ,
        		{ 0, 4, 0, 0} ,
        		{ 70, 12,  0, 0}  ,
        		{ 140, 18, 0, 0},
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 ,0}} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15, 0}} ,
        		"Old String"
        },
        {
        		// patch name : 'Metal dream'
        		// Engine
        		{ ALGO1, 8, 4, 6} ,
        		{ 7, 21, 19, 0} ,
        		{ 128, 128, 128, 0} ,
        		// Oscillator
        		{ OSC_SHAPE_SIN4, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 4, -127} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 24, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		// Enveloppe
        		{ 0, 255, 113, 111} ,
        		{ 0, 96, 0, 0} ,
        		{ 0, 208, 139, 109} ,
        		{ 3, 100, 100, 100} ,
        		{ 0, 150, 160, 60} ,
        		{ 0, 150, 160, 60} ,
        		// Modulation matrix
        		{ MATRIX_SOURCE_LFO1, 3, INDEX_MODULATION2, 0} ,
        		{ MATRIX_SOURCE_PITCHBEND, 22, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_MODWHEEL, 8, INDEX_MODULATION2, 0} ,
        		{ MATRIX_SOURCE_VELOCITY, 4, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		// LFOs
        		{ LFO_SAW, 36, 127, 150} ,
        		{ LFO_SAW, 16, 0, 0} ,
        		{ LFO_SAW, 16, 0, 0} ,
        		{ 0, 16, 0, 0} ,
        		{ 70, 12,  0, 0}  ,
        		{ 140, 18, 0, 0},
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 ,0}} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15, 0}} ,
        		"Metal dream"
        },
        {
        		// patch name : 'Space Wave'
        		// Engine
        		{ ALGO7, 14, 2, 6} ,
        		{ 22, 52, 57, 0} ,
        		{ 128, 88, 128, 0} ,
        		// Oscillator
        		{ OSC_SHAPE_SIN4, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 23, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, -17} ,
        		{ OSC_SHAPE_SIN3, OSC_FT_KEYBOARD , 48, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		// Enveloppe
        		{ 127, 83, 217, 166} ,
        		{ 44, 95, 36, 209} ,
        		{ 162, 100, 255, 168} ,
        		{ 53, 113, 137, 255} ,
        		{ 138, 150, 160, 159} ,
        		{ 218, 150, 255, 204} ,
        		// Modulation matrix
        		{ MATRIX_SOURCE_MODWHEEL, 5, INDEX_MODULATION3, 0} ,
        		{ MATRIX_SOURCE_PITCHBEND, 64, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_LFO1, 23, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_LFO2, 9, OSC6_FREQ, 0} ,
        		{ MATRIX_SOURCE_MODWHEEL, 92, MTX3_MUL, 0} ,
        		{ MATRIX_SOURCE_MODWHEEL, 35, MTX4_MUL, 0} ,
        		{ MATRIX_SOURCE_MODWHEEL, 127, LFO2_FREQ, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		// LFOs
        		{ LFO_SAW, 36, 0, 128} ,
        		{ LFO_SAW, 72, 127, 6} ,
        		{ LFO_SAW, 3, 0, 0} ,
        		{ 0, 4, 0, 0} ,
        		{ 70, 12,  0, 0}  ,
        		{ 140, 18, 0, 0},
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 ,0}} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15, 0}} ,
        		"Space Wave"
        },
        {
        		// patch name : 'Armonica'
        		// Engine
        		{ ALGO1, 14, 4, 6} ,
        		{ 16, 0, 47, 0} ,
        		{ 128, 128, 128, 0} ,
        		// Oscillator
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 32, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		// Enveloppe
        		{ 60, 180, 160, 71} ,
        		{ 100, 65, 150, 100} ,
        		{ 50, 100, 255, 100} ,
        		{ 3, 100, 100, 100} ,
        		{ 0, 150, 160, 60} ,
        		{ 0, 150, 160, 60} ,
        		// Modulation matrix
        		{ MATRIX_SOURCE_MODWHEEL, 3, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_PITCHBEND, 64, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_LFO1, 1, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		// LFOs
        		{ LFO_SAW, 37, 96, 214} ,
        		{ LFO_SAW, 20, 0, 0} ,
        		{ LFO_SAW, 3, 0, 0} ,
        		{ 0, 4, 0, 0} ,
        		{ 70, 12,  0, 0}  ,
        		{ 140, 18, 0, 0},
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 ,0}} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15, 0}} ,
        		"Armonica"

        },
        {
        		// patch name : 'Drum 0'
        		// Engine
        		{ ALGO3, 14, 2, 6} ,
        		{ 82, 0, 116, 197} ,
        		{ 128, 128, 128, 0} ,
        		// Oscillator
        		{ OSC_SHAPE_SIN, OSC_FT_FIXE, 1, -28} ,
        		{ OSC_SHAPE_SQUARE, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_RAND, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		// Enveloppe
        		{ 0, 127, 0, 65} ,
        		{ 47, 65, 128, 100} ,
        		{ 0, 0, 255, 90} ,
        		{ 0, 120, 0, 64} ,
        		{ 0, 150, 160, 60} ,
        		{ 0, 150, 160, 60} ,
        		// Modulation matrix
        		{ MATRIX_SOURCE_MODWHEEL, 16, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_PITCHBEND, 64, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_LFO4, 0, INDEX_MODULATION2, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		// LFOs
        		{ LFO_SAW, 36, 0, 0} ,
        		{ LFO_SAW, 20, 0, 0} ,
        		{ LFO_SAW, 3, 0, 0} ,
        		{ 115, 12, -1, 255} ,
        		{ 70, 12,  0, 0}  ,
        		{ 140, 18, 0, 0},
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 ,0}} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15, 0}} ,
        		"Drum 0"
        },
        {
        		// patch name : 'Frrrr'
        		// Engine
        		{ ALGO1, 14, 2, 7} ,
        		{ 61, 39, 0, 6} ,
        		{ 128, 128, 128, 0} ,
        		// Oscillator
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 64, 27} ,
        		{ OSC_SHAPE_RAND, OSC_FT_FIXE, 3, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 32, -88} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		// Enveloppe
        		{ 41, 11, 120, 97} ,
        		{ 7, 108, 43, 113} ,
        		{ 50, 100, 255, 100} ,
        		{ 3, 100, 100, 100} ,
        		{ 0, 150, 160, 60} ,
        		{ 0, 150, 160, 60} ,
        		// Modulation matrix
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_LFO4, 76, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_LFO2, 23, OSC3_FREQ, 0} ,
        		{ MATRIX_SOURCE_LFO2, 13, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		// LFOs
        		{ LFO_SAW, 36, 0, 158} ,
        		{ LFO_SAW, 37, 0, 114} ,
        		{ LFO_SAW, 3, 0, 0} ,
        		{ 58, 93, 0, 255} ,
        		{ 70, 12,  0, 0}  ,
        		{ 140, 18, 0, 0},
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 ,0}} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15, 0}} ,
        		"Frrrrrrr"
        },
        {
        		// patch name : 'Vibes'
        		// Engine
        		{ ALGO1, 14, 4, 6} ,
        		{ 0, 16, 0, 0} ,
        		{ 128, 128, 128, 0} ,
        		// Oscillator
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 32, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		// Enveloppe
        		{ 28, 150, 160, 244} ,
        		{ 69, 65, 171, 244} ,
        		{ 80, 0, 255, 100} ,
        		{ 3, 100, 100, 100} ,
        		{ 0, 150, 160, 60} ,
        		{ 0, 150, 160, 60} ,
        		// Modulation matrix
        		{ MATRIX_SOURCE_LFO4, 35, LFO1_FREQ, 0} ,
        		{ MATRIX_SOURCE_LFO1, 16, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_LFO1, 0, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_MODWHEEL, -20, LFO1_FREQ, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		// LFOs
        		{ LFO_SAW, 11, 0, 113} ,
        		{ LFO_RAMP, 20, 0, 0} ,
        		{ LFO_SAW, 3, 0, 0} ,
        		{ 245, 12, -1, 255} ,
        		{ 70, 12,  0, 0}  ,
        		{ 140, 18, 0, 0},
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 ,0}} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15, 0}} ,
        		"Vibes"

        },
        {
        		// patch name : 'Vibrato Accel'
        		// Engine
        		{ ALGO1, 14, 4, 6} ,
        		{ 0, 25, 24, 0} ,
        		{ 128, 128, 128, 0} ,
        		// Oscillator
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 32, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 24, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		// Enveloppe
        		{ 58, 150, 160, 157} ,
        		{ 61, 51, 255, 244} ,
        		{ 61, 0, 255, 255} ,
        		{ 3, 100, 100, 100} ,
        		{ 0, 150, 160, 60} ,
        		{ 0, 150, 160, 60} ,
        		// Modulation matrix
        		{ MATRIX_SOURCE_LFO4, 52, LFO1_FREQ, 0} ,
        		{ MATRIX_SOURCE_LFO1, 7, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_LFO1, 8, INDEX_MODULATION2, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		// LFOs
        		{ LFO_SAW, 2, 127, 107} ,
        		{ LFO_RAMP, 20, 0, 0} ,
        		{ LFO_SAW, 3, 0, 0} ,
        		{ 183, 0, -1, 255} ,
        		{ 70, 12,  0, 0}  ,
        		{ 140, 18, 0, 0},
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 ,0}} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15, 0}} ,
        		"Vibrato Acc"
        },
        {
        		// patch name : 'Futur Zone'
        		// Engine
        		{ ALGO7, 10, 2, 6} ,
        		{ 10, 0, 16, 0} ,
        		{ 128, 92, 128, 0} ,
        		// Oscillator
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 20, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 32, 0} ,
        		{ OSC_SHAPE_SQUARE, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 12, 0} ,
        		{ OSC_SHAPE_SIN2, OSC_FT_KEYBOARD , 48, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		// Enveloppe
        		{ 0, 185, 0, 124} ,
        		{ 0, 60, 52, 60} ,
        		{ 66, 0, 255, 216} ,
        		{ 16, 100, 255, 100} ,
        		{ 125, 150, 96, 229} ,
        		{ 184, 209, 0, 6} ,
        		// Modulation matrix
        		{ MATRIX_SOURCE_MODWHEEL, 5, INDEX_MODULATION2, 0} ,
        		{ MATRIX_SOURCE_VELOCITY, 6, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_LFO1, 1, INDEX_MODULATION2, 0} ,
        		{ MATRIX_SOURCE_LFO4, 127, LFO1_FREQ, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		// LFOs
        		{ LFO_SQUARE, 0, 127, 1} ,
        		{ LFO_SAW, 20, 0, 0} ,
        		{ LFO_SAW, 3, 0, 0} ,
        		{ 0, 255, 0, 255} ,
        		{ 70, 12,  0, 0}  ,
        		{ 140, 18, 0, 0},
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 ,0}} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15, 0}} ,
        		"Futur Zone"
        },
        {
        		// patch name : 'Squared line'
        		// Engine
        		{ ALGO1, 14, 4, 6} ,
        		{ 8, 18, 0, 0} ,
        		{ 128, 128, 128, 0} ,
        		// Oscillator
        		{ OSC_SHAPE_SQUARE, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SQUARE, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		// Enveloppe
        		{ 11, 150, 160, 60} ,
        		{ 25, 0, 255, 52} ,
        		{ 156, 0, 255, 100} ,
        		{ 3, 100, 100, 100} ,
        		{ 0, 150, 160, 60} ,
        		{ 0, 150, 160, 60} ,
        		// Modulation matrix
        		{ MATRIX_SOURCE_MODWHEEL, 8, INDEX_MODULATION2, 0} ,
        		{ MATRIX_SOURCE_LFO4, 127, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		// LFOs
        		{ LFO_SAW, 16, 127, 0} ,
        		{ LFO_SAW, 20, 0, 0} ,
        		{ LFO_SAW, 3, 0, 0} ,
        		{ 40, 40, 0, 49} ,
        		{ 70, 12,  0, 0}  ,
        		{ 140, 18, 0, 0},
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 ,0}} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15, 0}} ,
        		"Squared line"
        },
        {
        		// patch name : 'Iron Stick'
        		// Engine
        		{ ALGO1, 10, 4, 6} ,
        		{ 15, 11, 0, 0} ,
        		{ 128, 128, 128, 0} ,
        		// Oscillator
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 40, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 26, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		// Enveloppe
        		{ 0, 175, 0, 124} ,
        		{ 0, 55, 0, 55} ,
        		{ 41, 0, 255, 255} ,
        		{ 3, 100, 100, 100} ,
        		{ 0, 150, 160, 60} ,
        		{ 0, 150, 160, 60} ,
        		// Modulation matrix
        		{ MATRIX_SOURCE_MODWHEEL, 8, INDEX_MODULATION2, 0} ,
        		{ MATRIX_SOURCE_VELOCITY, 3, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		// LFOs
        		{ LFO_SAW, 36, 0, 0} ,
        		{ LFO_SAW, 20, 0, 0} ,
        		{ LFO_SAW, 3, 0, 0} ,
        		{ 0, 4, 0, 0} ,
        		{ 70, 12,  0, 0}  ,
        		{ 140, 18, 0, 0},
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 ,0}} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15, 0}} ,
        		"Iron Stick"
        },
        {
        		// patch name : 'Danger Now'
        		// Engine
        		{ ALGO7, 10, 2, 6} ,
        		{ 24, 24, 30, 0} ,
        		{ 82, 128, 128, 0} ,
        		// Oscillator
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN2, OSC_FT_KEYBOARD , 24, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 48, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 64, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		// Enveloppe
        		{ 0, 185, 200, 132} ,
        		{ 0, 67, 204, 243} ,
        		{ 72, 0, 255, 132} ,
        		{ 0, 100, 235, 255} ,
        		{ 62, 150, 87, 146} ,
        		{ 0, 209, 54, 255} ,
        		// Modulation matrix
        		{ MATRIX_SOURCE_MODWHEEL, 0, INDEX_MODULATION2, 0} ,
        		{ MATRIX_SOURCE_VELOCITY, 0, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_LFO1, 14, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_LFO1, 10, INDEX_MODULATION2, 0} ,
        		{ MATRIX_SOURCE_LFO1, 13, INDEX_MODULATION3, 0} ,
        		{ MATRIX_SOURCE_LFO4, 77, LFO1_FREQ, 0} ,
        		{ MATRIX_SOURCE_LFO2, 15, MIX_OSC2, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		// LFOs
        		{ LFO_SAW, 0, 127, 119} ,
        		{ LFO_SAW, 64, 0, 2} ,
        		{ LFO_SAW, 3, 0, 0} ,
        		{ 144, 157, 89, 255} ,
        		{ 70, 12,  0, 0}  ,
        		{ 140, 18, 0, 0},
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 ,0}} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15, 0}} ,
        		"Danger Now"
        },
        {
        		// patch name : 'Space Brass'
        		// Engine
        		{ ALGO2, 10, 4, 6} ,
        		{ 32, 40, 0, 0} ,
        		{ 128, 1, 128, 0} ,
        		// Oscillator
        		{ OSC_SHAPE_SQUARE, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN2, OSC_FT_KEYBOARD , 128, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		// Enveloppe
        		{ 0, 0, 255, 124} ,
        		{ 0, 0, 255, 55} ,
        		{ 0, 0, 255, 255} ,
        		{ 3, 100, 100, 100} ,
        		{ 0, 150, 160, 60} ,
        		{ 0, 150, 160, 60} ,
        		// Modulation matrix
        		{ MATRIX_SOURCE_LFO1, 10, MIX_OSC2, 0} ,
        		{ MATRIX_SOURCE_LFO4, 46, LFO1_FREQ, 0} ,
        		{ MATRIX_SOURCE_LFO1, -10, MIX_OSC1, 0} ,
        		{ MATRIX_SOURCE_LFO3, 5, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_MODWHEEL, 16, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		// LFOs
        		{ LFO_SAW, 36, 127, 123} ,
        		{ LFO_SAW, 39, 0, 0} ,
        		{ LFO_SAW, 49, 127, 60} ,
        		{ 255, 0, -1, 255} ,
        		{ 70, 12,  0, 0}  ,
        		{ 140, 18, 0, 0},
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 ,0}} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15, 0}} ,
        		"Space Brass"
        },
        {
        		// patch name : 'Indus 437'
        		// Engine
        		{ ALGO1, 14, 4, 6} ,
        		{ 24, 0, 16, 0} ,
        		{ 128, 128, 128, 0} ,
        		// Oscillator
        		{ OSC_SHAPE_SQUARE, OSC_FT_KEYBOARD , 32, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_RAND, OSC_FT_FIXE, 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		// Enveloppe
        		{ 49, 150, 255, 116} ,
        		{ 0, 0, 255, 255} ,
        		{ 0, 132, 30, 97} ,
        		{ 3, 100, 100, 100} ,
        		{ 0, 150, 160, 60} ,
        		{ 0, 150, 160, 60} ,
        		// Modulation matrix
        		{ MATRIX_SOURCE_LFO1, 4, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_MODWHEEL, 6, INDEX_MODULATION3, 0} ,
        		{ MATRIX_SOURCE_NONE, 6, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_NONE, 127, LFO1_FREQ, 0} ,
        		{ MATRIX_SOURCE_NONE, 2, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		// LFOs
        		{ LFO_RANDOM, 82, 127, 70} ,
        		{ LFO_SAW, 9, 0, 0} ,
        		{ LFO_SAW, 3, 0, 0} ,
        		{ 60, 150, 0, 133} ,
        		{ 70, 12,  0, 0}  ,
        		{ 140, 18, 0, 0},
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 ,0}} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15, 0}} ,
        		"Indus 437"
        },
        {
        		// patch name : 'Mystere'
        		// Engine
        		{ ALGO6, 10, 3, 8} ,
        		{ 1, 94, 42, 0} ,
        		{ 128, 74, 128, 0} ,
        		// Oscillator
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 32, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 63} ,
        		{ OSC_SHAPE_SIN3, OSC_FT_KEYBOARD , 12, -41} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		// Enveloppe
        		{ 0, 184, 58, 126} ,
        		{ 121, 18, 53, 12} ,
        		{ 0, 58, 0, 51} ,
        		{ 7, 144, 164, 90} ,
        		{ 0, 150, 160, 60} ,
        		{ 0, 150, 160, 60} ,
        		// Modulation matrix
        		{ MATRIX_SOURCE_MODWHEEL, 127, OSC2_FREQ, 0} ,
        		{ MATRIX_SOURCE_PITCHBEND, 64, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_LFO1, 52, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_VELOCITY, 4, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_LFO2, 5, OSC6_FREQ, 0} ,
        		{ MATRIX_SOURCE_NONE, -128, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		// LFOs
        		{ LFO_SAW, 30, 0, 197} ,
        		{ LFO_SAW, 6, -33, 158} ,
        		{ LFO_SAW, 3, 0, 0} ,
        		{ 0, 4, 0, 0} ,
        		{ 70, 12,  0, 0}  ,
        		{ 140, 18, 0, 0},
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 ,0}} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15, 0}} ,
        		"Mystere"
        },
        {
        		// patch name : 'Up Bass'
        		// Engine
        		{ ALGO3, 14, 3, 6} ,
        		{ 57, 99, 26, 4} ,
        		{ 128, 128, 128, 0} ,
        		// Oscillator
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 32, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SAW, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 48, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		// Enveloppe
        		{ 40, 117, 167, 63} ,
        		{ 0, 121, 255, 100} ,
        		{ 50, 79, 163, 60} ,
        		{ 3, 100, 100, 100} ,
        		{ 0, 150, 160, 60} ,
        		{ 0, 150, 160, 60} ,
        		// Modulation matrix
        		{ MATRIX_SOURCE_MODWHEEL, 16, INDEX_MODULATION1, 0} ,
        		{ MATRIX_SOURCE_PITCHBEND, 64, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_LFO1, 1, INDEX_MODULATION2, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		// LFOs
        		{ LFO_SAW, 38, 0, 98} ,
        		{ LFO_SAW, 20, 0, 0} ,
        		{ LFO_SAW, 3, 0, 0} ,
        		{ 255, 4, -1, 255} ,
        		{ 70, 12,  0, 0}  ,
        		{ 140, 18, 0, 0},
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 ,0}} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15, 0}} ,
        		"Up Bass"
        },
        {
        		// patch name : 'Broken game'
        		// Engine
        		{ ALGO3, 16, 3, 6} ,
        		{ 16, 32, 64, 133} ,
        		{ 128, 0, 128, 0} ,
        		// Oscillator
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 8, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 23, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, -17} ,
        		{ OSC_SHAPE_SIN3, OSC_FT_KEYBOARD , 48, 0} ,
        		{ OSC_SHAPE_SIN, OSC_FT_KEYBOARD , 16, 0} ,
        		// Enveloppe
        		{ 0, 83, 231, 243} ,
        		{ 44, 95, 36, 209} ,
        		{ 140, 100, 255, 168} ,
        		{ 0, 113, 137, 255} ,
        		{ 138, 150, 160, 159} ,
        		{ 218, 150, 255, 204} ,
        		// Modulation matrix
        		{ MATRIX_SOURCE_MODWHEEL, 55, OSC3_FREQ, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_LFO1, 97, OSC1_FREQ, 0} ,
        		{ MATRIX_SOURCE_LFO2, 9, OSC6_FREQ, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, -128, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		{ MATRIX_SOURCE_NONE, 0, DESTINATION_NONE, 0} ,
        		// LFOs
        		{ LFO_SAW, 16, 0, 128} ,
        		{ LFO_SAW, 0, 127, 6} ,
        		{ LFO_SAW, 16, 0, 0} ,
        		{ 0, 16, 0, 0} ,
        		{ 70, 12,  0, 0}  ,
        		{ 140, 18, 0, 0},
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 ,0}} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15, 0}} ,
        		"Broken game"
        },
        {
                { 0,0,0,0} ,
                { 0,0,0,0} ,
                { 0,0,0,0} ,

                { 0,0,0,0} ,
                { 0,0,0,0} ,
                { 0,0,0,0} ,
                { 0,0,0,0} ,
                { 0,0,0,0} ,
                { 0,0,0,0} ,

                { 0,0,0,0} ,
                { 0,0,0,0} ,
                { 0,0,0,0} ,
                { 0,0,0,0} ,
                { 0,0,0,0} ,
                { 0,0,0,0} ,

                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,

                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
                { 0, 0, 0, 0} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15 ,0}} ,
        		{{ 15, 4, 2, 0, 15, 2, 0, 8, 15, 0, 12, 0, 8, 0, 15, 0}} ,
                "*"
        }
};
