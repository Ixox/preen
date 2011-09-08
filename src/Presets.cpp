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

                { MATRIX_SOURCE_MODWHEEL ,   16,   INDEX_MODULATION1, 0 },
                { MATRIX_SOURCE_PITCHBEND,   64,  OSC1_FREQ,   0 },
                { MATRIX_SOURCE_LFO1,        0,   OSC1_FREQ, 0 },
                { MATRIX_SOURCE_NONE, 0,  DESTINATION_NONE, 0 },
                { MATRIX_SOURCE_NONE, 0,  DESTINATION_NONE, 0 },
                { MATRIX_SOURCE_NONE, 0,  DESTINATION_NONE, 0 },
                { MATRIX_SOURCE_NONE, 0,  DESTINATION_NONE, 0 },
                { MATRIX_SOURCE_NONE, 0,  DESTINATION_NONE, 0 },

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
                { 7, 16, 7, 0} ,
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
                { 7, 2, 8, 0} ,
                { 5, 64, 1, 0} ,
                { 1, 0, 1, 0} ,
                { 7, 6, 8, 0} ,
                { 2, 11, 8, 0} ,
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
                { 7, 2, 7, 0} ,
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
                { 7, 6, 7, 0} ,
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

                { 7, 8, 7, 0} ,
                { 7, 24, 8, 0} ,
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

                { 7, 16, 7, 0} ,
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
                { 7, 16, 12, 0} ,
                { 7, 13, 7, 0} ,
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
                { 7, 16, 7, 0} ,
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
                { 8, 8, 7, 0} ,
                { 8, 10, 1, 0} ,
                { 1, 6, 1, 0} ,
                { 7, 9, 8, 0} ,
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
                { 1, 3, 8, 0} ,
                { 5, 22, 1, 0} ,
                { 7, 4, 8, 0} ,
                { 8, 4, 7, 0} ,
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
                { 7, 16, 7, 0} ,
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
        		"Power"
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
        		{ MATRIX_SOURCE_MODWHEEL, 16, INDEX_MODULATION1, 0} ,
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
        		"Armonica"

        },
        {
        		// patch name : 'Drum 0'
        		// Engine
        		{ ALGO3, 14, 1, 6} ,
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
        		"Vibrato Acc"
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

                { 7, 16, 11, 0} ,
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
