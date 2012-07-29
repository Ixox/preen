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
const char* allChars  = "_ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz 0123456789 <>&*$,;:";

const char* nullNames []= {};
const unsigned char* nullNamesOrder = NULL;
const char* algoNames [] = { "alg1", "alg2", "alg3", "alg4", "alg5", "alg6", "alg7", "alg8", "alg9" };
struct ParameterRowDisplay engine1ParameterRow  = {
        "Engine" ,
        { "Algo", "Velo", "Voic", "Glid" },
        {
                {ALGO1, ALGO_END-1, DISPLAY_TYPE_STRINGS, algoNames, nullNamesOrder, nullNamesOrder},
                {0, 16, DISPLAY_TYPE_UNSIGNED_CHAR, nullNames,nullNamesOrder, nullNamesOrder },
                {1, 4, DISPLAY_TYPE_VOICES, nullNames, nullNamesOrder, nullNamesOrder },
                {0, 10, DISPLAY_TYPE_UNSIGNED_CHAR, nullNames, nullNamesOrder, nullNamesOrder }
        }
};

struct ParameterRowDisplay engine2ParameterRow = {
        "Modulation" ,
        { "IM1 ", "IM2 ", "IM3 ", "IM4 "},
        {
                {0, 255, DISPLAY_TYPE_FLOAT_5_3, nullNames, nullNamesOrder, nullNamesOrder },
                {0, 255, DISPLAY_TYPE_FLOAT_5_3, nullNames, nullNamesOrder, nullNamesOrder },
                {0, 255, DISPLAY_TYPE_FLOAT_5_3, nullNames, nullNamesOrder, nullNamesOrder },
                {0, 255, DISPLAY_TYPE_FLOAT_5_3, nullNames, nullNamesOrder, nullNamesOrder }
        }
};

struct ParameterRowDisplay engine3ParameterRow = {
        "Mixer" ,
        { "Mix1", "Mix2", "Mix3", "Mix4" },
        {
                {1, 128, DISPLAY_TYPE_FLOAT_1_7, nullNames, nullNamesOrder, nullNamesOrder },
                {1, 128, DISPLAY_TYPE_FLOAT_1_7, nullNames, nullNamesOrder, nullNamesOrder },
                {1, 128, DISPLAY_TYPE_FLOAT_1_7, nullNames, nullNamesOrder, nullNamesOrder },
                {1, 128, DISPLAY_TYPE_FLOAT_1_7, nullNames, nullNamesOrder, nullNamesOrder }
        }
};



const char* oscShapeNames []=  {"sin ", "s^2 ", "1/2s", "spos", "rand", "squa", "saw ", "off " } ;
/*
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
*/
unsigned char oscShameOrder[] = { OSC_SHAPE_SIN, OSC_SHAPE_SQUARE, OSC_SHAPE_SAW, OSC_SHAPE_RAND, OSC_SHAPE_SIN2, OSC_SHAPE_SIN3, OSC_SHAPE_SIN4, OSC_SHAPE_OFF };
unsigned char oscShameOrderReversed[] = { 0, 4, 5, 6, 3, 1, 2, 7};

const char* oscTypeNames [] = { "keyb", "fixe"};
struct ParameterRowDisplay oscParameterRow = {
        "Oscillator",
        { "Shap", "FTyp", "Freq", "FTun" },
        {
                { OSC_SHAPE_SIN, OSC_SHAPE_OFF, DISPLAY_TYPE_STRINGS, oscShapeNames, oscShameOrder, oscShameOrderReversed },
                { OSC_FT_KEYBOARD, OSC_FT_FIXE, DISPLAY_TYPE_STRINGS, oscTypeNames, nullNamesOrder, nullNamesOrder},
                { 0, 128, DISPLAY_TYPE_OSC_FREQUENCY , nullNames, nullNamesOrder, nullNamesOrder },
                { (char)-127, 127, DISPLAY_TYPE_OSC_FREQUENCY, nullNames, nullNamesOrder, nullNamesOrder }
        }
};

struct ParameterRowDisplay envParameterRow = {
        "Enveloppe",
        { "Attk", "Deca", "Sust", "Rele" },
        {
                { 0, 255, DISPLAY_TYPE_UNSIGNED_CHAR, nullNames, nullNamesOrder, nullNamesOrder },
                { 0, 255, DISPLAY_TYPE_UNSIGNED_CHAR, nullNames, nullNamesOrder, nullNamesOrder },
                { 0, 255, DISPLAY_TYPE_UNSIGNED_CHAR, nullNames, nullNamesOrder, nullNamesOrder },
                { 0, 255, DISPLAY_TYPE_UNSIGNED_CHAR, nullNames, nullNamesOrder, nullNamesOrder }
        }
};

struct ParameterRowDisplay lfoEnvParameterRow = {
        "LFO Env",
        { "Attk", "Deca", "Sust", "Rele" },
        {
                { 0, 255, DISPLAY_TYPE_UNSIGNED_CHAR, nullNames, nullNamesOrder, nullNamesOrder },
                { 0, 255, DISPLAY_TYPE_UNSIGNED_CHAR, nullNames, nullNamesOrder, nullNamesOrder },
                { 0, 255, DISPLAY_TYPE_UNSIGNED_CHAR, nullNames, nullNamesOrder, nullNamesOrder },
                { 0, 255, DISPLAY_TYPE_UNSIGNED_CHAR_OR_NONE, nullNames, nullNamesOrder, nullNamesOrder }
        }
};

const char* matrixSourceNames [] = { "None", "lfo1", "lfo2", "lfo3", "lfo4", "PitB", "AftT", "ModW", "Velo", "CC1 ", "CC2 ", "CC3 ", "CC4 ", "lfo5", "lfo6", "key "} ;
/*
enum SourceEnum {
	MATRIX_SOURCE_NONE = 0,
	MATRIX_SOURCE_LFO1=1,
	MATRIX_SOURCE_LFO2=2,
	MATRIX_SOURCE_LFO3=3,
	MATRIX_SOURCE_LFO4=4,
	MATRIX_SOURCE_PITCHBEND=5,
	MATRIX_SOURCE_AFTERTOUCH=6,
	MATRIX_SOURCE_MODWHEEL=7,
    MATRIX_SOURCE_VELOCITY=8,
    MATRIX_SOURCE_CC1=9,
    MATRIX_SOURCE_CC2=10,
    MATRIX_SOURCE_CC3=11,
    MATRIX_SOURCE_CC4=12,
	MATRIX_SOURCE_LFO5=13,
	MATRIX_SOURCE_LFO6=14,
	MATRIX_SOURCE_KEY=15,
	MATRIX_SOURCE_MAX
};
*/

unsigned char  matrixSourceNamesOrder[] = { MATRIX_SOURCE_NONE, MATRIX_SOURCE_LFO1,MATRIX_SOURCE_LFO2,MATRIX_SOURCE_LFO3,MATRIX_SOURCE_LFO4,MATRIX_SOURCE_LFO5,MATRIX_SOURCE_LFO6,
/* 7 */		MATRIX_SOURCE_MODWHEEL, /* 8 */MATRIX_SOURCE_VELOCITY, MATRIX_SOURCE_KEY, MATRIX_SOURCE_PITCHBEND,MATRIX_SOURCE_AFTERTOUCH, /* 12 */ MATRIX_SOURCE_CC1,MATRIX_SOURCE_CC2,MATRIX_SOURCE_CC3,MATRIX_SOURCE_CC4 };
unsigned char  matrixSourceNamesOrderReversed[] = {0, 1, 2, 3, 4, 10, 11, 7, 8, 12, 13, 14, 15, 5, 6, 9 };


/*
enum DestinationEnum {
	DESTINATION_NONE = 0, 	OSC1_FREQ, 	OSC2_FREQ,
	OSC3_FREQ = 3 ,	OSC4_FREQ,	OSC5_FREQ,
	OSC6_FREQ = 6,	INDEX_MODULATION1,	INDEX_MODULATION2,
	INDEX_MODULATION3 = 9,    INDEX_MODULATION4,	MIX_OSC1,
    MIX_OSC2 = 12,    MIX_OSC3,    MIX_OSC4,
	LFO1_FREQ = 15,	LFO2_FREQ,	LFO3_FREQ,
	LFO4_FREQ = 18,	MTX1_MUL,	MTX2_MUL,
	MTX3_MUL = 21,	MTX4_MUL,	MTX5_MUL,
	MTX6_MUL = 24,	MTX7_MUL,	MTX8_MUL,
	MTX9_MUL = 27,	MTX10_MUL,	MTX11_MUL,
	MTX12_MUL= 30,	ALL_OSC_FREQ,	LFO5_GATE,
	LFO6_GATE,

*/

const char* matrixDestNames [] = { "None", "o1Fq", "o2Fq", "o3Fq", "o4Fq", "o5Fq", "o6Fq", "IM1 ", "IM2 ", "IM3 ", "IM4 ", "Mix1", "Mix2", "Mix3", "Mix4",
		"l1Fq", "l2Fq", "l3Fq", "l4Fq", "mx01", "mx02", "mx03", "mx04", "mx05", "mx06", "mx07", "mx08", "mx09", "mx10", "mx11", "mx12", "o*Fq", "l5gt", "l6gt", "gate" } ;
unsigned char  matrixDestNamesOrder[] = { DESTINATION_NONE, OSC1_FREQ, 	OSC2_FREQ,
/* 3 */		OSC3_FREQ  ,	OSC4_FREQ,	OSC5_FREQ, OSC6_FREQ ,
/* 7 */     ALL_OSC_FREQ, INDEX_MODULATION1,	INDEX_MODULATION2,
/* 10 */	INDEX_MODULATION3 ,    INDEX_MODULATION4,	MIX_OSC1,
/* 13 */    MIX_OSC2 ,    MIX_OSC3,    MIX_OSC4,
/* 16 */	LFO1_FREQ ,	LFO2_FREQ,	LFO3_FREQ,
/* 19 */	LFO4_FREQ , LFO5_GATE, 	LFO6_GATE, MTX1_MUL,	MTX2_MUL,
/* 24 */	MTX3_MUL, MTX4_MUL,	MTX5_MUL,
/* 27 */	MTX6_MUL, 	MTX7_MUL,	MTX8_MUL,
/* 30 */	MTX9_MUL ,	MTX10_MUL,	MTX11_MUL,
/* 33 */	MTX12_MUL, MAIN_GATE	  };

unsigned char  matrixDestNamesOrderReversed[] = {0,1,2,3,4,5,6,8,9,10,11,12,13,14,15,16,17,18,19,22,23,24,25,26,27,28,29,30,31,32,33,7,20,21,34};

struct ParameterRowDisplay matrixParameterRow = {
        "Matrix",
        { "Srce", "Mult", "Dest", "    " },
        {
                { MATRIX_SOURCE_NONE, MATRIX_SOURCE_MAX-1, DISPLAY_TYPE_STRINGS, matrixSourceNames, matrixSourceNamesOrder, matrixSourceNamesOrderReversed},
                { (char)-127, 127, DISPLAY_TYPE_SIGNED_CHAR, nullNames, nullNamesOrder, nullNamesOrder },
                { DESTINATION_NONE, DESTINATION_MAX-1, DISPLAY_TYPE_STRINGS, matrixDestNames, matrixDestNamesOrder, matrixDestNamesOrderReversed},
                { 0, 0, DISPLAY_TYPE_NONE, nullNames, nullNamesOrder, nullNamesOrder }
        }
};

const char* lfoOscMidiClock[] =  { "M/16", "MC/8", "MC/4", "MC/2", "MClk", "MC*2", "MC*3", "MC*4", "MC*8"};

const char* lfoShapeNames [] =  { "Saw ", "Ramp", "Squa", "Rand", "Sin "} ;

unsigned char  lfoShapeNamesOrder[] = { LFO_SIN, LFO_SAW, LFO_RAMP, LFO_SQUARE, LFO_RANDOM };
unsigned char  lfoShapeNamesOrderReversed[] = { 1, 2, 3, 4, 0};

struct ParameterRowDisplay lfoParameterRow = {
        "LFO Osc",
        { "Shap", "Freq", "Bias", "KSyn" },
        {
                { LFO_SAW, LFO_TYPE_MAX-1, DISPLAY_TYPE_STRINGS,  lfoShapeNames, lfoShapeNamesOrder, lfoShapeNamesOrderReversed},
                { 0, 255, DISPLAY_TYPE_LFO_HZ, nullNames, nullNamesOrder, nullNamesOrder },
                { (char)-127, 127, DISPLAY_TYPE_SIGNED_CHAR, nullNames, nullNamesOrder, nullNamesOrder },
                { 0, 255, DISPLAY_TYPE_UNSIGNED_CHAR, nullNames, nullNamesOrder, nullNamesOrder },
        }
};



const char* lfoSeqMidiClock[] =  { "MC/4", "MC/2", "MC  ", "MC*2", "MC*4" };

struct ParameterRowDisplay lfoStepParameterRow = {
        "LFO Seq",
        { "Bpm ", "Gate", "    ", "    " },
        {
                { 10 ,245, DISPLAY_TYPE_STEP_SEQ_BPM, nullNames, nullNamesOrder, nullNamesOrder},
                { 0 ,32, DISPLAY_TYPE_FLOAT_4_4, nullNames, nullNamesOrder, nullNamesOrder},
                { 0, 0, DISPLAY_TYPE_STEP_SEQ1, nullNames, nullNamesOrder, nullNamesOrder },
                { 0, 0, DISPLAY_TYPE_STEP_SEQ2, nullNames, nullNamesOrder, nullNamesOrder }
        }
};

const struct AllParameterRowsDisplay allParameterRows = {
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
                &matrixParameterRow,
                &matrixParameterRow,
                &matrixParameterRow,
                &matrixParameterRow,
                &lfoParameterRow,
                &lfoParameterRow,
                &lfoParameterRow,
                &lfoEnvParameterRow,
                &lfoStepParameterRow,
                &lfoStepParameterRow
        }
};


const struct ShowUpAlgo  showUp[9] = {
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




SynthState::SynthState() {
    engineRow =  ROW_ENGINE;
    // operator works for both osc and env
    operatorRow  = 0;
    matrixRow = ROW_MATRIX1;
    lfoRow    = ROW_LFO1;

    // First default preset
    fullState.synthMode = SYNTH_MODE_EDIT;
    fullState.bankNumber = 0;
    fullState.presetNumber = 0;
    fullState.internalPresetNumber = 0;
    fullState.loadWhat = 0;
    fullState.saveWhat = 0;
    fullState.midiConfigValue[MIDICONFIG_CHANNEL] = 0;
    fullState.midiConfigValue[MIDICONFIG_THROUGH] = 0;
    fullState.midiConfigValue[MIDICONFIG_RECEIVES] = 3;
    fullState.midiConfigValue[MIDICONFIG_SENDS] = 1;
    fullState.midiConfigValue[MIDICONFIG_REALTIME_SYSEX] = 0;
    fullState.midiConfigValue[MIDICONFIG_TEST_NOTE] = 60;
    fullState.midiConfigValue[MIDICONFIG_TEST_VELOCITY] = 120;
    fullState.firstMenu = 0;

    for (int k=0; k<12; k++) {
        fullState.name[k] = 0;
    }

    currentRow = 0;
    for (unsigned int k=0; k<sizeof(struct AllSynthParams); k++) {
        ((char*)&params)[k] = ((char*)presets)[k];
    }

    stepSelect[0] = 0;
    stepSelect[1] = 0;

    isPlayingNote = false;

}



void SynthState::encoderTurnedForStepSequencer(int row, int encoder, int ticks) {
	int whichStepSeq = row - ROW_LFO5;
	StepSequencerSteps * seqSteps = &((StepSequencerSteps * )(&params.steps5))[whichStepSeq];

	if (encoder == 2) {
		int oldPos = stepSelect[whichStepSeq];
		stepSelect[whichStepSeq] += (ticks>0? 1 : -1);

		if (stepSelect[whichStepSeq]>15) {
			stepSelect[whichStepSeq] = 0;
		} else if (stepSelect[whichStepSeq]<0) {
			stepSelect[whichStepSeq] = 15;
		}

        propagateNewParamValue(row, encoder, (ParameterDisplay*)NULL, oldPos, stepSelect[whichStepSeq]);

	} else if (encoder == 3) {
		char *step = &seqSteps->steps[stepSelect[whichStepSeq]];
		int oldValue = (int)(*step);

		(*step) += ticks;

		if ((*step)>15) {
			(*step) = 15;
		} else if ((*step)<0) {
			(*step) = 0;
		}

        propagateNewParamValue(row, encoder, (ParameterDisplay*)NULL, oldValue, (int)(*step));


	}
}


void SynthState::encoderTurned(int encoder, int ticks) {
    if (fullState.synthMode == SYNTH_MODE_EDIT) {
        int num = encoder + currentRow * NUMBER_OF_ENCODERS;

        // Step sequencer special case
        if (currentRow >= ROW_LFO5) {
    		if (encoder >= 2) {
    			encoderTurnedForStepSequencer(currentRow, encoder, ticks);
    			return;
    		}
    	};

        struct ParameterDisplay* param = &(allParameterRows.row[currentRow]->params[encoder]);
        int newValue;
        int oldValue;


		if (param->displayType == DISPLAY_TYPE_STRINGS) {
			// Do not use encoder acceleration
			ticks = ticks>0? 1 : -1;
		}

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
        } else if (param->valueNameOrder != NULL) {
            unsigned char *value = &((unsigned char*)&params)[num];
            int index;

            newValue = oldValue = (*value);

            // Must use newValue (int) so that the minValue comparaison works
            // Is there any other order than the default one
			int pos = param->valueNameOrderReversed[(*value)];
            if (ticks>0 && pos < param->maxValue) {
                newValue = param->valueNameOrder[pos+1];
            }
            if (ticks<0 && pos>param->minValue) {
                newValue = param->valueNameOrder[pos-1];
            }

            (*value) = (unsigned char)newValue;
        } else {
            unsigned char *value;

			value = &((unsigned char*)&params)[num];

            oldValue = (*value);
            // Must use newValue (int) so that the minValue comparaison works
            // Is there any other order than the default one
			newValue = (*value) + ticks;
            if (ticks>0 && newValue>param->maxValue) {
                newValue = param->maxValue;
            }
            if (ticks<0 && newValue<param->minValue) {
                newValue = param->minValue;
            }

            (*value) = (unsigned char)newValue;
        }

        if (newValue != oldValue) {
            propagateNewParamValue(currentRow, encoder, param, oldValue, newValue);
        }
    } else {
        int oldMenuSelect = fullState.menuSelect;
        if (encoder==0) {
            if (ticks>0) {
                fullState.menuSelect = fullState.menuSelect + 1;
            } else if (ticks<0) {
                fullState.menuSelect = fullState.menuSelect - 1;
            }
        } else if (encoder==1) {
            if (fullState.currentMenuItem->menuState == MENU_SAVE_ENTER_NAME) {
                fullState.name[fullState.menuSelect] = (fullState.name[fullState.menuSelect] + (ticks>0? 1: -1));
                if (fullState.name[fullState.menuSelect]<28) {
                    fullState.name[fullState.menuSelect]=28;
                }
                if (fullState.name[fullState.menuSelect]> 53) {
                    fullState.name[fullState.menuSelect] = 53;
                }
                propagateNewMenuSelect();
            } else if (fullState.currentMenuItem->maxValue == 128) {
                if (ticks>0) {
                    fullState.menuSelect = fullState.menuSelect + 5;
                } else if (ticks<0) {
                    fullState.menuSelect = fullState.menuSelect - 5;
                }
            }
        } else if (encoder==2) {
            if (fullState.currentMenuItem->menuState == MENU_SAVE_ENTER_NAME) {
                fullState.name[fullState.menuSelect] = (fullState.name[fullState.menuSelect] + (ticks>0? 1: -1));
                if (fullState.name[fullState.menuSelect]<1) {
                    fullState.name[fullState.menuSelect]=1;
                }
                if (fullState.name[fullState.menuSelect]> 26) {
                    fullState.name[fullState.menuSelect] = 26;
                }
                propagateNewMenuSelect();
            } else if (fullState.currentMenuItem->maxValue == 128) {
                if (ticks>0) {
                    fullState.menuSelect = fullState.menuSelect + 10;
                } else if (ticks<0) {
                    fullState.menuSelect = fullState.menuSelect - 10;
                }
            }
        } else if (encoder==3) {
            if (fullState.currentMenuItem->menuState == MENU_SAVE_ENTER_NAME) {
                fullState.name[fullState.menuSelect] = (fullState.name[fullState.menuSelect] + (ticks>0? 1: -1));
                if (fullState.name[fullState.menuSelect]<0) {
                    fullState.name[fullState.menuSelect]=0;
                }
                if (fullState.name[fullState.menuSelect]>= getLength(allChars)) {
                    fullState.name[fullState.menuSelect]= getLength(allChars)-1;
                }
                propagateNewMenuSelect();
            } else if (fullState.currentMenuItem->menuState == MENU_CONFIG_MIDI) {
            	fullState.midiConfigValue[fullState.menuSelect] = fullState.midiConfigValue[fullState.menuSelect] + (ticks>0? 1: -1);
            	if (fullState.midiConfigValue[fullState.menuSelect] >= midiConfig[fullState.menuSelect].maxValue) {
            		fullState.midiConfigValue[fullState.menuSelect] = midiConfig[fullState.menuSelect].maxValue - 1;
            	}
            	if (fullState.midiConfigValue[fullState.menuSelect] < 0 ) {
            		fullState.midiConfigValue[fullState.menuSelect] = 0;
            	}
                propagateNewMenuSelect();
            } else if (fullState.currentMenuItem->maxValue == 128) {
                if (ticks>0) {
                    fullState.menuSelect = fullState.menuSelect + 25;
                } else if (ticks<0) {
                    fullState.menuSelect = fullState.menuSelect - 25;
                }
            }
        }

        if (fullState.menuSelect> fullState.currentMenuItem->maxValue - 1) {
            fullState.menuSelect = fullState.currentMenuItem->maxValue - 1;
        }
        if (fullState.menuSelect< 0) {
            fullState.menuSelect = 0;
        }

        if (fullState.menuSelect != oldMenuSelect) {
            if (fullState.currentMenuItem->menuState == MENU_LOAD_INTERNAL) {
                char* preset = (char*)&(presets[fullState.menuSelect].engine1);
                propagateBeforeNewParamsLoad();
                PresetUtil::copySynthParams(preset, (char*)&params);
                propagateAfterNewParamsLoad();
                fullState.internalPresetNumber = fullState.menuSelect;
            } else if (fullState.currentMenuItem->menuState == MENU_LOAD_USER_SELECT_PRESET) {
                propagateBeforeNewParamsLoad();
                PresetUtil::readSynthParamFromEEPROM(fullState.bankNumber, fullState.menuSelect, &params);
                propagateAfterNewParamsLoad();
                fullState.presetNumber = fullState.menuSelect;
            }
            propagateNewMenuSelect();
        }
    }

}


void SynthState::resetDisplay() {
    fullState.synthMode = SYNTH_MODE_EDIT;
    currentRow = 0;
    propagateNewSynthMode();
}


void SynthState::changeSynthModeRow(int button, int step) {
	unsigned char lastBecauseOfAlgo;

	switch (button) {
		case BUTTON_SYNTH:
			if (showUp[params.engine1.algo].mix == 0) {
				lastBecauseOfAlgo = ROW_MODULATION;
			} else {
				lastBecauseOfAlgo = ROW_ENGINE_LAST;
			}
			if (currentRow<ROW_ENGINE_FIRST || currentRow>lastBecauseOfAlgo) {
				currentRow = engineRow;
			} else {
				currentRow += step;
			}
			if (currentRow>lastBecauseOfAlgo) {
				currentRow = ROW_ENGINE_FIRST;
			} else if (currentRow<ROW_ENGINE_FIRST) {
				currentRow = lastBecauseOfAlgo;
			}
			engineRow = currentRow;
		break;
		case BUTTON_OSC:
			lastBecauseOfAlgo = ROW_OSC_FIRST + showUp[params.engine1.algo].osc - 1;
			if (currentRow<ROW_OSC_FIRST || currentRow>lastBecauseOfAlgo) {
				currentRow = ROW_OSC_FIRST + operatorRow;
			} else {
				currentRow += step;
			}
			if (currentRow>lastBecauseOfAlgo) {
				currentRow = ROW_OSC_FIRST;
			} else if (currentRow<ROW_OSC_FIRST) {
				currentRow = lastBecauseOfAlgo;
			}
			operatorRow = currentRow - ROW_OSC_FIRST;
			break;
		case BUTTON_ENV:
			lastBecauseOfAlgo = ROW_ENV_FIRST + showUp[params.engine1.algo].osc - 1;
			if (currentRow<ROW_ENV_FIRST || currentRow>lastBecauseOfAlgo) {
				currentRow = ROW_ENV_FIRST + operatorRow;
			} else {
				currentRow += step;
			}
			if (currentRow>lastBecauseOfAlgo) {
				currentRow = ROW_ENV_FIRST;
			} else if (currentRow<ROW_ENV_FIRST) {
				currentRow = lastBecauseOfAlgo;
			}
			operatorRow = currentRow - ROW_ENV_FIRST;
		break;
		case BUTTON_MATRIX:
			if (currentRow<ROW_MATRIX_FIRST || currentRow>ROW_MATRIX_LAST) {
				currentRow = matrixRow;
			} else {
				currentRow += step;
				if (currentRow>ROW_MATRIX_LAST) {
					currentRow = ROW_MATRIX_FIRST;
				} else if (currentRow<ROW_MATRIX_FIRST) {
					currentRow = ROW_MATRIX_LAST;
				}
			}
			matrixRow = currentRow;
			break;
		case BUTTON_LFO:
			if (currentRow<ROW_LFO_FIRST || currentRow>ROW_LFO_LAST) {
				currentRow = lfoRow;
			} else {
				currentRow += step;
				if (currentRow>ROW_LFO_LAST) {
					currentRow = ROW_LFO_FIRST;
				} else if (currentRow<ROW_LFO_FIRST) {
					currentRow = ROW_LFO_LAST;
				}
			}
			lfoRow = currentRow;
		break;
	}
}

void SynthState::buttonPressed(int button) {
    SynthMode oldSynthMode = fullState.synthMode;
    int oldCurrentRow = currentRow;
    // int oldMenuSelect = fullState.menuSelect;
    MenuState oldMenuState = fullState.currentMenuItem->menuState;


    if (fullState.synthMode == SYNTH_MODE_EDIT)  {
        switch (button) {
        case BUTTON_SYNTH:
        case BUTTON_OSC:
        case BUTTON_ENV:
        case BUTTON_MATRIX:
        case BUTTON_LFO:
        	lastButtonSelected = button ;
        	changeSynthModeRow(button , 1);
            break;
        case BUTTON_MENUSELECT:
            fullState.synthMode = SYNTH_MODE_MENU;
            fullState.menuSelect = fullState.firstMenu;
            // allow undo event after trying some patches
            PresetUtil::copySynthParams((char*)&params, (char*)&backupParams);
            fullState.currentMenuItem = MenuItemUtil::getMenuItem(MAIN_MENU);
            break;
        case BUTTON_BACK:
        	if (this->isPlayingNote) {
        		propagateNoteOff();
        	}
        	break;
        }
    } else {
        // Any button when done is display makes the synth go back to edit mode.
		// MENU MODE
		switch (button) {
		case BUTTON_MENUSELECT:
			fullState.currentMenuItem = afterButtonPressed();
			break;
		case BUTTON_BACK:
			fullState.currentMenuItem = menuBack();
			propagateMenuBack();
			break;
#ifdef DEBUG
		case BUTTON_DUMP:
		{
			PresetUtil::dumpPatch();
			break;
		}
#endif
		case BUTTON_LFO:
		{
			if (fullState.currentMenuItem->menuState == MENU_SAVE_ENTER_NAME) {
				fullState.name[fullState.menuSelect] = 0;
				propagateNewMenuSelect();
			}
			break;
		}
		case BUTTON_MATRIX:
		{
			if (fullState.currentMenuItem->menuState == MENU_SAVE_ENTER_NAME) {
				fullState.name[fullState.menuSelect] = 27;
				propagateNewMenuSelect();
			}
			break;
		}
		case BUTTON_ENV:
		{
			if (fullState.currentMenuItem->menuState == MENU_SAVE_ENTER_NAME) {
				fullState.name[fullState.menuSelect] = 55;
				propagateNewMenuSelect();
			}
			break;
		}
		case BUTTON_OSC:
		{
			if (fullState.currentMenuItem->menuState == MENU_SAVE_ENTER_NAME) {
				fullState.name[fullState.menuSelect] = 66;
				propagateNewMenuSelect();
			}
			break;
		}
		// MENU MODE
		}
    }

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

void SynthState::buttonLongPressed(int button) {
    int oldCurrentRow = currentRow;

    if (fullState.synthMode == SYNTH_MODE_EDIT)  {
        switch (button) {
        case BUTTON_SYNTH:
        case BUTTON_OSC:
        case BUTTON_ENV:
        case BUTTON_MATRIX:
        case BUTTON_LFO:
        	lastButtonSelected = button ;
        	changeSynthModeRow(button , -1);
            break;
        case BUTTON_BACK:
        	if (!this->isPlayingNote) {
        		propagateNoteOn();
        	}
        	break;
        }
    }
    if (oldCurrentRow != currentRow) {
        propagateNewCurrentRow(currentRow);
    }
}


void SynthState::setNewStepValue(int whichStepSeq, int step, int newValue) {
	if (whichStepSeq <0 || whichStepSeq>1 || step <0 || step > 15 || newValue<0 || newValue>15) {
		return;
	}
	StepSequencerSteps * seqSteps = &((StepSequencerSteps * )(&params.steps5))[whichStepSeq];

	int oldValue = seqSteps->steps[step];

	if (oldValue !=  newValue) {
		int oldStep = stepSelect[whichStepSeq];
		seqSteps->steps[step] = newValue;
		stepSelect[whichStepSeq] = step;
		if (oldStep != step) {
			propagateNewParamValueFromExternal(ROW_LFO5 + whichStepSeq, 2, NULL, oldStep, stepSelect[whichStepSeq]);
		}
		propagateNewParamValueFromExternal(ROW_LFO5 + whichStepSeq, 3, NULL, oldValue, newValue);
	}

}

void SynthState::setNewValue(int row, int number, int newValue) {
    int index = row * NUMBER_OF_ENCODERS + number;
    struct ParameterDisplay* param = &(allParameterRows.row[row]->params[number]);
    int oldValue = ((char*)&params)[index];
    if (newValue > param->maxValue) {
    	newValue= param->maxValue;
    } else if (newValue < param->minValue) {
    	newValue= param->minValue;
    }
    if (param->displayType == DISPLAY_TYPE_SIGNED_CHAR) {
        ((char*)&params)[index] = newValue;
    } else {
        ((unsigned char*)&params)[index] = newValue;
    }
    propagateNewParamValueFromExternal(row, number, param, oldValue, newValue);
}

const MenuItem* SynthState::afterButtonPressed() {
    const MenuItem* rMenuItem = 0;

    if (fullState.currentMenuItem->hasSubMenu ) {
        rMenuItem = MenuItemUtil::getMenuItem(fullState.currentMenuItem->subMenu[fullState.menuSelect]);
    } else {
        rMenuItem = MenuItemUtil::getMenuItem(fullState.currentMenuItem->subMenu[0]);
    }

    // ------------------------
    // Previous state switch

    switch (fullState.currentMenuItem->menuState) {
    case MAIN_MENU:
        fullState.firstMenu = fullState.menuSelect;
        break;
    case MENU_SAVE_SELECT_USER_BANK:
    case MENU_SAVE_BANK:
        fullState.bankNumber = fullState.menuSelect;
        break;
    case MENU_LOAD_INTERNAL:
        // Make change definitive
        PresetUtil::copySynthParams((char*)&params, (char*)&backupParams);
        fullState.presetModified = false;
        break;
    case MENU_SAVE_BANK_CONFIRM:
    	PresetUtil::copyBank(4, fullState.bankNumber);
    	break;
    case MENU_LOAD_USER_SELECT_BANK:
        fullState.bankNumber = fullState.menuSelect;
        break;
    case MENU_LOAD_USER_SELECT_PRESET:
        propagateBeforeNewParamsLoad();
        PresetUtil::readSynthParamFromEEPROM(fullState.bankNumber, fullState.menuSelect, &params);
        PresetUtil::copySynthParams((char*)&params, (char*)&backupParams);
        propagateAfterNewParamsLoad();
        fullState.presetModified = false;
        break;
    case MENU_SAVE_SELECT_PRESET:
        for (int k=0; k<12 && params.presetName[k] != 0; k++) {
            for (int j=0; j<getLength(allChars); j++) {
                if (params.presetName[k] == allChars[j]) {
                    fullState.name[k] = j;
                }
            }
        }
        fullState.presetNumber = fullState.menuSelect;
        break;
    case MENU_SAVE_ENTER_NAME:
    {
        int length;
        for (length=12; fullState.name[length-1] == 0; length--);
        for (int k=0; k<length; k++) {
            params.presetName[k] = allChars[(int)fullState.name[k]];
        }
        params.presetName[length] = '\0';
        PresetUtil::saveCurrentPatchToEEPROM(fullState.bankNumber, fullState.presetNumber);
        break;
    }
    case MENU_MIDI_PATCH_DUMP:
        PresetUtil::sendCurrentPatchToSysex();
        break;
    case MENU_MIDI_BANK_SELECT_DUMP:
    {
    	const MenuItem *cmi = fullState.currentMenuItem;
    	// Update display while sending
    	fullState.currentMenuItem = MenuItemUtil::getMenuItem(MENU_IN_PROGRESS);
    	propagateNewMenuState();
        PresetUtil::sendBankToSysex(fullState.menuSelect);
        fullState.currentMenuItem = cmi;
        break;
    }
    case MENU_DONE:
        fullState.synthMode = SYNTH_MODE_EDIT;
        break;
    case MENU_FORMAT_ALL:
    	if (fullState.menuSelect == 25) {
        	const MenuItem *cmi = fullState.currentMenuItem;
        	// Update display while formating
        	fullState.currentMenuItem = MenuItemUtil::getMenuItem(MENU_IN_PROGRESS);
        	propagateNewMenuState();
            PresetUtil::formatEEPROM();
        	PresetUtil::resetConfigAndSaveToEEPROM();
            fullState.currentMenuItem = cmi;
    	} else if (fullState.menuSelect == 2) {
    		PresetUtil::sendCurrentPatchAsNrpns();
    	} else if (fullState.menuSelect == 1) {
			PresetUtil::checkReadEEPROM();
    	} else {
    		return fullState.currentMenuItem;
    	}
        break;
    case MENU_FORMAT_V1_10:
    	if (fullState.menuSelect == 25) {
        	const MenuItem *cmi = fullState.currentMenuItem;
        	// Update display while formating
        	fullState.currentMenuItem = MenuItemUtil::getMenuItem(MENU_IN_PROGRESS);
        	propagateNewMenuState();
            PresetUtil::upgradeEEPROMToV1_10();
        	PresetUtil::resetConfigAndSaveToEEPROM();
            fullState.currentMenuItem = cmi;
    	} else {
    		return fullState.currentMenuItem;
    	}
        break;
    case MENU_CONFIG_MIDI_SAVE:
        PresetUtil::saveConfigToEEPROM();
        break;
    case MENU_SAVE_DEFAULT:
        PresetUtil::saveCurrentPatchAsDefault();
        break;
    case MENU_LOAD:
        fullState.loadWhat = fullState.menuSelect;
        break;
    case MENU_SAVE:
        fullState.saveWhat = fullState.menuSelect;
        break;
     default:
        break;
    }

    // ---------------------
    // Next state switch

    switch (rMenuItem->menuState) {
    case MENU_SAVE_SELECT_PRESET:
        fullState.menuSelect = fullState.presetNumber;
        break;
    case MENU_LOAD_USER_SELECT_PRESET:
        propagateBeforeNewParamsLoad();
        PresetUtil::readSynthParamFromEEPROM(fullState.bankNumber, fullState.presetNumber, &params);
        propagateAfterNewParamsLoad();
        fullState.menuSelect = fullState.presetNumber;
        break;
    case MENU_SAVE_SELECT_USER_BANK:
        fullState.menuSelect = fullState.bankNumber;
        break;
    case MENU_LOAD:
        fullState.menuSelect = fullState.loadWhat;
        break;
    case MENU_SAVE:
        fullState.menuSelect = fullState.saveWhat;
        break;
    case MENU_LOAD_INTERNAL:
        propagateBeforeNewParamsLoad();
        PresetUtil::copySynthParams((char*)&presets[fullState.internalPresetNumber], (char*)&params);
        propagateAfterNewParamsLoad();
        fullState.menuSelect = fullState.internalPresetNumber;
        break;
    case MENU_LOAD_USER_SELECT_BANK:
        fullState.menuSelect = fullState.bankNumber;
        break;
    default:
        fullState.menuSelect = 0;
    }

    // Save menu select for menuBack Action
    fullState.previousMenuSelect = fullState.menuSelect;
    return rMenuItem;
}


const MenuItem* SynthState::menuBack() {
    const MenuItem* rMenuItem = 0;

    // default menuSelect value
    fullState.menuSelect = MenuItemUtil::getParentMenuSelect(fullState.currentMenuItem->menuState);

    switch (fullState.currentMenuItem->menuState) {
    case MENU_SAVE_ENTER_NAME:
        fullState.menuSelect = fullState.presetNumber;
        break;
    case MENU_SAVE_SELECT_PRESET:
        fullState.menuSelect = fullState.bankNumber;
        break;
    case MENU_LOAD_USER_SELECT_PRESET:
        fullState.menuSelect = fullState.bankNumber;
        propagateBeforeNewParamsLoad();
        PresetUtil::copySynthParams((char*)&backupParams, (char*)&params);
        propagateAfterNewParamsLoad();
        break;
    case MENU_LOAD_INTERNAL:
        propagateBeforeNewParamsLoad();
        PresetUtil::copySynthParams((char*)&backupParams, (char*)&params);
        propagateAfterNewParamsLoad();
        break;
    case MENU_SAVE_BANK:
    	// After back, bank should noe be receivable anymore
        fullState.synthMode = SYNTH_MODE_EDIT;
    	return MenuItemUtil::getMenuItem(MENU_LOAD);
    	break;
    case MAIN_MENU:
        fullState.synthMode = SYNTH_MODE_EDIT;
        // put back old patch (has been overwritten if a new patch has been loaded)
        break;
    case MENU_FORMAT_ALL:
    case MENU_FORMAT_V1_10:
    case MENU_LOAD_USER_SELECT_BANK:
    case MENU_MIDI_PATCH:
    case MENU_SAVE_SELECT_USER_BANK:
    case MENU_MIDI_BANK:
        break;
    }

    rMenuItem = MenuItemUtil::getParentMenuItem(fullState.currentMenuItem->menuState);
    return rMenuItem;
}


void SynthState::newBankReady() {
	fullState.synthMode = SYNTH_MODE_MENU;
	fullState.menuSelect = 0;
	fullState.currentMenuItem = MenuItemUtil::getMenuItem(MENU_SAVE_BANK);
	propagateNewSynthMode();
}



void SynthState::propagateAfterNewParamsLoad() {
   for (SynthParamListener* listener = firstParamListener; listener !=0; listener = listener->nextListener) {
	   listener->afterNewParamsLoad();
   }
}

void SynthState::tempoClick() {
	if (fullState.synthMode == SYNTH_MODE_MENU && fullState.currentMenuItem->menuState == MENU_DONE) {
		if (doneClick > 7) {
		    fullState.synthMode = SYNTH_MODE_EDIT;
		    propagateNewSynthMode();
		}
		doneClick ++;
	} else {
		doneClick = 0;
	}
}
