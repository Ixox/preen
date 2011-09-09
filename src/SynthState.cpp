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
const char* algoNames [] = { "alg1", "alg2", "alg3", "alg4", "alg5", "alg6", "alg7", "alg8", "alg9" };
struct ParameterRowDisplay engine1ParameterRow  = {
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

const char* oscShapeNames []=  {"sin ", "s^2 ", "1/2s", "spos", "rand", "squa", "saw ", "off " } ;
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

struct ParameterRowDisplay lfoEnvParameterRow = {
        "LFO (Env)",
        { "Attk", "Deca", "Sust", "Rele" },
        {
                { 0, 255, DISPLAY_TYPE_UNSIGNED_CHAR, nullNames },
                { 0, 255, DISPLAY_TYPE_UNSIGNED_CHAR, nullNames },
                { 0, 255, DISPLAY_TYPE_UNSIGNED_CHAR, nullNames },
                { 0, 255, DISPLAY_TYPE_UNSIGNED_CHAR_OR_NONE, nullNames }
        }
};

const char* matrixSourceNames [] = { "None", "lfo1", "lfo2", "lfo3", "lfo4", "PitB", "AftT", "ModW", "Velo", "CC1 ", "CC2 ", "CC3 ", "CC4 "} ;
const char* matrixDestNames [] = { "None", "o1Fr", "o2Fr", "o3Fr", "o4Fr", "o5Fr", "o6Fr", "IM1 ", "IM2 ", "IM3 ", "IM4 ", "Mix1", "Mix2", "Mix3", "Mix4",
		"lfo1", "lfo2", "lfo3", "lfo4", "mtx1", "mtx2", "mtx3", "mtx4", "mtx5", "mtx6", "mtx7", "mtx8" } ;
struct ParameterRowDisplay matrixParameterRow = {
        "Matrix",
        { "Srce", "Mult", "Dest", "    " },
        {
                { MATRIX_SOURCE_NONE, MATRIX_SOURCE_MAX-1, DISPLAY_TYPE_STRINGS, matrixSourceNames},
                { (char)-127, 127, DISPLAY_TYPE_SIGNED_CHAR, nullNames },
                { DESTINATION_NONE, DESTINATION_MAX-1, DISPLAY_TYPE_STRINGS, matrixDestNames},
                { 0, 0, DISPLAY_TYPE_NONE, nullNames }
        }
};


const char* lfoShapeNames [] =  { "Saw ", "Ramp", "Squa", "Rand"} ;
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
                &lfoParameterRow,
                &lfoParameterRow,
                &lfoParameterRow,
                &lfoEnvParameterRow
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
    oscRow    = ROW_OSC1;
    envRow    = ROW_ENV1;
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
    fullState.firstMenu = 0;

    for (int k=0; k<12; k++) {
        fullState.name[k] = 0;
    }

    currentRow = 0;
    for (unsigned int k=0; k<sizeof(struct AllSynthParams); k++) {
        ((char*)&params)[k] = ((char*)presets)[k];
    }
}

void SynthState::encoderTurned(int encoder, int ticks) {
    if (fullState.synthMode == SYNTH_MODE_EDIT) {

        int num = currentRow * NUMBER_OF_ENCODERS + encoder;
        struct ParameterDisplay* param = &(allParameterRows.row[currentRow]->params[encoder]);
        int newValue;
        int oldValue;

        // Store lastAction
        lastAction = encoder;

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
                PresetUtil::copyPatch(preset, (char*)&params);
                propagateAfterNewParamsLoad();
                fullState.internalPresetNumber = fullState.menuSelect;
            } else if (fullState.currentMenuItem->menuState == MENU_LOAD_USER_SELECT_PRESET) {
                propagateBeforeNewParamsLoad();
                PresetUtil::readFromEEPROM(fullState.bankNumber, fullState.menuSelect, (char*)&params);
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
				currentRow = oscRow;
			} else {
				currentRow += step;
			}
			if (currentRow>lastBecauseOfAlgo) {
				currentRow = ROW_OSC_FIRST;
			} else if (currentRow<ROW_OSC_FIRST) {
				currentRow = lastBecauseOfAlgo;
			}
			oscRow = currentRow;
			break;
			case BUTTON_ENV:
			lastBecauseOfAlgo = ROW_ENV_FIRST + showUp[params.engine1.algo].osc - 1;
			if (currentRow<ROW_ENV_FIRST || currentRow>lastBecauseOfAlgo) {
				currentRow = envRow;
			} else {
				currentRow += step;
			}
			if (currentRow>lastBecauseOfAlgo) {
				currentRow = ROW_ENV_FIRST;
			} else if (currentRow<ROW_ENV_FIRST) {
				currentRow = lastBecauseOfAlgo;
			}
			envRow = currentRow;
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
        	lastAction = button + NUMBER_OF_ENCODERS;
        	changeSynthModeRow(button , 1);
            break;
        case BUTTON_MENUSELECT:
            fullState.synthMode = SYNTH_MODE_MENU;
            fullState.menuSelect = fullState.firstMenu;
            // allow undo event after trying some patches
            PresetUtil::copyPatch((char*)&params, (char*)&backupParams);
            fullState.currentMenuItem = MenuItemUtil::getMenuItem(MAIN_MENU);
            break;
        case BUTTON_BACK:
        	if (this->lastAction < NUMBER_OF_ENCODERS) {
        		encoderTurned(lastAction, 1);
        	} else {
            	changeSynthModeRow(lastAction-NUMBER_OF_ENCODERS , -1);
        	}
        }
    } else {
    	// MENU MODE
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
        PresetUtil::copyPatch((char*)&params, (char*)&backupParams);
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
        PresetUtil::readFromEEPROM(fullState.bankNumber, fullState.menuSelect, (char*)&params);
        PresetUtil::copyPatch((char*)&params, (char*)&backupParams);
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
    case MENU_FORMAT_BANK:
        PresetUtil::formatEEPROM();
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
        PresetUtil::readFromEEPROM(fullState.bankNumber, fullState.presetNumber, (char*)&params);
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
        PresetUtil::copyPatch((char*)&presets[fullState.internalPresetNumber], (char*)&params);
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
        PresetUtil::copyPatch((char*)&backupParams, (char*)&params);
        propagateAfterNewParamsLoad();
        break;
    case MENU_LOAD_INTERNAL:
        propagateBeforeNewParamsLoad();
        PresetUtil::copyPatch((char*)&backupParams, (char*)&params);
        propagateAfterNewParamsLoad();
        break;
    case MENU_SAVE_BANK:
    case MAIN_MENU:
        fullState.synthMode = SYNTH_MODE_EDIT;
        // put back old patch (has been overwritten if a new patch has been loaded)
        break;
    case MENU_FORMAT_BANK:
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


