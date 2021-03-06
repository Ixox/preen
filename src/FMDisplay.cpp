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

#include "FMDisplay.h"


extern const char* lfoSeqMidiClock[];
extern const char* lfoOscMidiClock[];

const char* stepChars  = "_123456789ABCDEF";

FMDisplay::FMDisplay() {
}

FMDisplay::~FMDisplay() {
}

void FMDisplay::init(LiquidCrystal* lcd) {
	presetModifed = false;
	refreshStatus = 0;
	this->lcd = lcd;
	lcd->clear();
}

void FMDisplay::printValueWithSpace(int value) {
    lcd->print(value);

	if (value>99) {
		lcd->print(' ');
	} else if (value>9) {
		lcd->print("  ");
	} else if (value>-1) {
		lcd->print("   ");
	} else if (value>-10) {
		lcd->print("  ");
	} else if (value>-100) {
		lcd->print(' ');
	}
}


boolean FMDisplay::shouldThisValueShowUp(int row, int encoder) {
    int algo = this->synthState->params.engine1.algo;
    if (row == ROW_MODULATION && (encoder+1)> showUp[algo].im) {
        return false;
    }
    if (row == ROW_OSC_MIX && (encoder+1)> showUp[algo].mix) {
        return false;
    }
    if (row == ROW_ENGINE && encoder == ENCODER_ENGINE_GLIDE && this->synthState->params.engine1.numberOfVoice >1) {
        return false;
    }

    return true;
}

void FMDisplay::updateEncoderValue(int row, int encoder, ParameterDisplay* param, int newValue) {
    lcd->setCursor(encoder*5, 3);

    if (!shouldThisValueShowUp(row, encoder)) {
        lcd->print("    ");
        return;
    }

    switch (param->displayType) {
	case DISPLAY_TYPE_STRINGS :
		lcd->print(param->valueName[newValue]);
		break;
    case DISPLAY_TYPE_LFO_HZ:
    	if (newValue > 246) {
    		lcd->print(lfoOscMidiClock[newValue-247]);
    		break;
    	}
    	// else what follows :
	case DISPLAY_TYPE_FLOAT_4_4:
displayFloat44:
		lcd->print(newValue>>4);
		lcd->print(".");
		if (newValue < 160) {
			int v = ((newValue & 0xf) *100) >>4;
			if (v > 10) {
				lcd->print(v);
			} else if (v > 0){
				lcd->print("0");
				lcd->print(v);
			} else {
				lcd->print("00");
			}
		} else {
			int v = ((newValue & 0xf) *10) >>4;
			if (v == 0) {
				lcd->print("0");
			} else {
				lcd->print(v);
			}
		}
        lcd->print(' ');
		break;
	case DISPLAY_TYPE_FLOAT_5_3:
displayFloat53:
		lcd->print(newValue>>3);
		lcd->print(".");
		if (newValue < 80) {
			int v= ((newValue & 0x7) *100) >>3;
			if (v > 10) {
				lcd->print(v);
			} else if (v > 0){
				lcd->print("0");
				lcd->print(v);
			} else {
				lcd->print("00");
			}
		} else {
			int v = ((newValue & 0x7) *10) >>3;
			if (v == 0) {
				lcd->print("0");
			} else {
				lcd->print(v);
			}
		}
        lcd->print(' ');
		break;
    case DISPLAY_TYPE_FLOAT_1_7:
    {
        lcd->print(newValue>>7);
        lcd->print(".");
        int v = ((newValue & 0x7f) *100) >>7;
        if (v >= 10) {
            lcd->print(v);
        } else {
            lcd->print("0");
            lcd->print(v);
        }
        lcd->print(' ');
        break;
    }
    case DISPLAY_TYPE_STEP_SEQ_BPM:
    	if (newValue > 240) {
    		lcd->print(lfoSeqMidiClock[newValue-241]);
    		break;
    	}
    	goto displaySignedChar;
    case DISPLAY_TYPE_UNSIGNED_CHAR_OR_NONE:
    	if (newValue == 255) {
    		lcd->print("None");
    		break;
    	}
    	// else what follows
	case DISPLAY_TYPE_SIGNED_CHAR:
displaySignedChar:
	case DISPLAY_TYPE_UNSIGNED_CHAR:
		printValueWithSpace(newValue);
		break;
	case DISPLAY_TYPE_OSC_FREQUENCY:
	{
		// Hack... to deal with the special case of the fixe frequency.....
		int oRow = row - ROW_OSC_FIRST;
		OscillatorParams* oParam = (OscillatorParams*)&this->synthState->params.osc1;
		OscFrequencyType ft = (OscFrequencyType)oParam[oRow].frequencyType;

		if (ft == OSC_FT_FIXE) {
			lcd->setCursor(10, 3);
			newValue = ((oParam[oRow].frequencyMul << 7) + oParam[oRow].detune)<<2;
			goto displayFloat53;
		} else {
			// Freq
			if (encoder == ENCODER_OSC_FREQ) goto displayFloat44;
			// Fine tune
			newValue = (char)newValue;
			if (encoder == ENCODER_OSC_FTUNE) goto displaySignedChar;
		}
		break;
	}
	case DISPLAY_TYPE_NONE:
		lcd->print("    ");
		break;
    case DISPLAY_TYPE_VOICES:
        printValueWithSpace(newValue);
        if (newValue <= 2) {
            // if voices = 1 or 0 let's refresh the glide info
            updateEncoderValue(ENCODER_ENGINE_GLIDE+1);
            updateEncoderName(row, ENCODER_ENGINE_GLIDE);
        }
        break;
    case DISPLAY_TYPE_STEP_SEQ1:
    {
			int whichStepSeq = row - ROW_LFO5;
			StepSequencerSteps * seqSteps = &((StepSequencerSteps * )(&this->synthState->params.steps5))[whichStepSeq];

    		int decal = (this->synthState->stepSelect[whichStepSeq] >> 3 ) * 8;
    		lcd->setCursor(12, 3);
			for (int k=0; k<4; k++) {
				lcd->print(stepChars[seqSteps->steps[k + decal] ] );
			}

			lcd->setCursor(10, 2);
			lcd->print("  ");
			lcd->setCursor(10, 3);
			lcd->print(' ');
			if (decal == 0) {
				lcd->print((char)5);
			} else {
				lcd->print((char)6);
			}

			lcd->setCursor(12 + this->synthState->stepSelect[whichStepSeq] - decal, 2);
			lcd->print((char)3);
    	}
    	break;
    case DISPLAY_TYPE_STEP_SEQ2:
    {
    		int whichStepSeq = row - ROW_LFO5;
			StepSequencerSteps * seqSteps = &((StepSequencerSteps * )(&this->synthState->params.steps5))[whichStepSeq];
			int decal = (this->synthState->stepSelect[whichStepSeq] >> 3 ) * 8;

			lcd->setCursor(16, 3);
			for (int k=4; k<8; k++) {
				lcd->print(stepChars[seqSteps->steps[k + decal]] );
			}
			lcd->setCursor(12, 2);
			lcd->print((char)4);
			lcd->setCursor(16, 2);
			lcd->print((char)4);
    }
    break;
	}
}

void FMDisplay::updateEncoderName(int row, int encoder) {
    lcd->setCursor(encoder*5, 2);
    if (!shouldThisValueShowUp(row, encoder)) {
        lcd->print("    ");
        return;
    }
	const struct ParameterRowDisplay* paramRow = allParameterRows.row[row];
	lcd->print(paramRow->paramName[encoder]);
}

void FMDisplay::refreshAllScreenByStep() {
    switch (refreshStatus) {
    case 12:
		lcd->setCursor(3,1);
		lcd->print("               ");
		break;
    case 11:
		// erase the caracters between
		for (int k=0; k<4; k++) {
			lcd->setCursor(4+k*5,2);
			lcd->print(' ');
		}
		break;
    case 10:
		// erase the caracters between
		for (int k=0; k<4; k++) {
			lcd->setCursor(4+k*5,3);
			lcd->print(' ');
		}
		break;
    case 9:
    {
		int row = this->synthState->getCurrentRow();
		lcd->setCursor(0,1);
		lcd->print(allParameterRows.row[row]->rowName);
		if (row> ROW_ENGINE_LAST) {
            lcd->print(' ');
            lcd->print(getRowNumberRelative(row));
		}
        break;
    }
    case 5:
    case 6:
    case 7:
    case 8:
		updateEncoderName(this->synthState->getCurrentRow(), refreshStatus -5);
        break;
    default :
	    updateEncoderValue(refreshStatus);

	}
    refreshStatus --;
}

 void FMDisplay::updateEncoderValue(int refreshStatus) {
    int row = this->synthState->getCurrentRow();
    struct ParameterDisplay param = allParameterRows.row[row]->params[refreshStatus -1];
    int newValue;
    if (row < ROW_LFO5) {
        if (param.displayType == DISPLAY_TYPE_SIGNED_CHAR) {
            newValue = ((char*)&this->synthState->params)[row*NUMBER_OF_ENCODERS+refreshStatus -1];
        } else {
            newValue = ((unsigned char*)&this->synthState->params)[row*NUMBER_OF_ENCODERS+refreshStatus -1];
        }
    } else if (row == ROW_LFO5) {
        newValue = ((unsigned char*)&this->synthState->params.lfo5)[refreshStatus -1];
    } else if (row == ROW_LFO6) {
        newValue = ((unsigned char*)&this->synthState->params.lfo6)[refreshStatus -1];
    }

    updateEncoderValue(this->synthState->getCurrentRow(), refreshStatus -1, &param, newValue);
}


void FMDisplay::displayPreset() {
    FullState* fullState = &this->synthState->fullState;
	int length = getLength(this->synthState->params.presetName);
	lcd->setCursor(19-length,0);
	lcd->print(this->synthState->params.presetName);
	if (fullState->presetModified) {
	    lcd->print((char)2);
	}
}

void FMDisplay::checkPresetModified() {
    if (!presetModifed && this->synthState->fullState.presetModified && this->synthState->fullState.synthMode == SYNTH_MODE_EDIT) {
        presetModifed = true;
        lcd->setCursor(19,0);
        lcd->print((char)2);
    }
}



// Update FMDisplay regarding the callbacks from SynthState


void FMDisplay::newParamValueFromExternal(SynthParamType type, int currentRow, int encoder, ParameterDisplay* param, int oldValue, int newValue) {
    checkPresetModified();
	if (this->synthState->getSynthMode() == SYNTH_MODE_EDIT && currentRow == this->displayedRow) {
		if (currentRow >= ROW_LFO5 && encoder>1) {
			updateStepSequencer(currentRow, encoder, oldValue, newValue);
			return;
		}
		updateEncoderValue(currentRow, encoder, param, newValue);
	}
}


void FMDisplay::updateStepSequencer(int currentRow, int encoder, int oldValue, int newValue) {
	int whichStepSeq = currentRow - ROW_LFO5 ;
	int decal = (this->synthState->stepSelect[whichStepSeq] >> 3 ) * 8;

	if (encoder == 3) {
		// new value to display
		lcd->setCursor(12 + this->synthState->stepSelect[whichStepSeq] - decal, 3);
		lcd->print(stepChars[newValue]);
	} else if (encoder == 2) {
		int oldDecal = (oldValue >> 3 ) * 8;
		// Change cursor
		lcd->setCursor(12 + oldValue - oldDecal, 2);
		if ((oldValue & 3) == 0) {
	    	lcd->print((char)4);
		} else {
			lcd->print(' ');
		}
		// if new part 0-7 or 8-15 let's redraw all value
		if (oldDecal != decal) {
			refreshStatus = 4;
			return;
		}

		lcd->setCursor(12 + newValue - decal, 2);
		lcd->print((char)3);
	}
}

void FMDisplay::newParamValue(SynthParamType type, int currentRow, int encoder, ParameterDisplay* param,  int oldValue, int newValue) {
    checkPresetModified();
	if (this->synthState->getSynthMode() == SYNTH_MODE_EDIT) {
		if (currentRow != this->displayedRow) {
			newcurrentRow(currentRow);
			return;
		}
		// if we change ROW_LFO5 it's a bit special
		if (currentRow >= ROW_LFO5 && encoder>1) {
			updateStepSequencer(currentRow, encoder, oldValue, newValue);
			return;
		}

		// If we change frequency type of OScillator rows, it's a bit special too....
		if (SynthState::getListenerType(currentRow)==SYNTH_PARAM_TYPE_OSC && encoder == ENCODER_OSC_FTYPE) {
			refreshStatus = 4;
			return;
		}

		updateEncoderValue(currentRow, encoder, param, newValue);
	}
}

void FMDisplay::newcurrentRow(int newcurrentRow) {
	refreshStatus = 12;
	this->displayedRow = newcurrentRow;
}


/*
 * Menu Listener
 */

void FMDisplay::newSynthMode(FullState* fullState)  {
	lcd->clear();
	if (fullState->synthMode == SYNTH_MODE_EDIT) {
		displayPreset();
		refreshStatus = 12;
	} else {
		refreshStatus = 0;
		menuRow = 0;
		newMenuState(fullState);
	}
}

void FMDisplay::newMenuState(FullState* fullState) {
	menuRow++;
	if (fullState->currentMenuItem->hasSubMenu) {
	    int pos = 0;
		for (int k=0; k<fullState->currentMenuItem->maxValue; k++) {
		    fullState->menuPosition[k] = pos;
			lcd->setCursor(pos +1, menuRow-1);
			const char* name = MenuItemUtil::getMenuItem(fullState->currentMenuItem->subMenu[k])->name;
			pos = pos + (getLength(name) + 1);
            lcd->print(name);
		}
	}

	switch (fullState->currentMenuItem->menuState) {
		case MENU_SAVE_BANK:
			menuRow = 2;
			lcd->setCursor(1, menuRow-2);
			lcd->print("Store new bank in");
			// Then what follow : no break
		case MENU_LOAD_USER_SELECT_BANK:
		case MENU_SAVE_SELECT_USER_BANK:
		case MENU_MIDI_BANK_SELECT_DUMP:
			lcd->setCursor(1, menuRow-1);
			lcd->print("Bnk1 Bnk2 Bnk3 Bnk4");
            fullState->menuPosition[0] = 0;
            fullState->menuPosition[1] = 5;
            fullState->menuPosition[2] = 10;
            fullState->menuPosition[3] = 15;
			break;
		case MENU_SAVE_ENTER_NAME:
			// -2 because must erase preset name....
			lcd->setCursor(6, menuRow-2);
			for (int k=0;k<12; k++) {
				lcd->print(allChars[(int)fullState->name[k]]);
			}
			break;
		case MENU_SAVE_BANK_CONFIRM:
			lcd->setCursor(1, menuRow-1);
			lcd->print("Confirm replace ?");
			break;
		case MENU_FORMAT_BANK:
			lcd->setCursor(1, menuRow-1);
			lcd->print("Y to format:");
			fullState->menuSelect = 14;
			lcd->setCursor(14, menuRow-1);
			lcd->print(allChars[fullState->menuSelect]);
			break;
		case MENU_SAVE_DEFAULT:
			lcd->setCursor(1, menuRow-1);
			lcd->print("Save to default ?");
			break;
		case MENU_MIDI_SYSEX_GET:
			lcd->setCursor(1, menuRow-1);
			lcd->print("Waiting SysEx...");
			break;
		case MENU_CONFIG_SETTINGS:
			lcd->setCursor(1, menuRow-1);
			lcd->print(midiConfig[fullState->menuSelect].title);
			break;
		case MENU_CONFIG_SETTINGS_SAVE:
			lcd->setCursor(1, menuRow-1);
			lcd->print("Save to EEPROM ?");
			break;
		case MENU_MIDI_PATCH_DUMP:
			lcd->setCursor(1, menuRow-1);
			lcd->print("Send Patch ?");
			break;
	}

	newMenuSelect(fullState);
}

void FMDisplay::newMenuSelect(FullState* fullState) {
	lcd->noCursor();

	switch(fullState->currentMenuItem->menuState) {
	case MAIN_MENU:
	case MENU_LOAD:
	case MENU_SAVE:
    case MENU_CONFIG:
    case MENU_MIDI_SYS_EX:
	case MENU_MIDI_BANK:
	case MENU_MIDI_PATCH:
	case MENU_LOAD_USER_SELECT_BANK:
	case MENU_SAVE_SELECT_USER_BANK:
	case MENU_MIDI_BANK_SELECT_DUMP:
	case MENU_MIDI_SYSEX_DUMP:
	case MENU_SAVE_BANK:
		for (int k=0; k<fullState->currentMenuItem->maxValue; k++) {
			lcd->setCursor(fullState->menuPosition[k], menuRow-1);
			lcd->print(' ');
		}
		lcd->setCursor(fullState->menuPosition[fullState->menuSelect], menuRow-1);
		lcd->print(">");
		break;
	case MENU_LOAD_USER_SELECT_PRESET:
	case MENU_LOAD_INTERNAL:
		eraseRow(menuRow-1);
		lcd->setCursor(2, menuRow-1);
		lcd->print(fullState->menuSelect + 1);
		lcd->print(" - ");
		lcd->print(this->synthState->params.presetName);
		break;
	case MENU_DONE:
		lcd->clear();
		lcd->setCursor(8,1);
		lcd->print("DONE");
		break;
	case MENU_IN_PROGRESS:
		lcd->clear();
		lcd->setCursor(3,1);
		lcd->print("In Progress...");
		break;
	case MENU_SAVE_SELECT_PRESET:
		eraseRow(menuRow-1);
		lcd->setCursor(2, menuRow-1);
		lcd->print(fullState->menuSelect + 1);
        lcd->print(" - ");
        lcd->print(PresetUtil::readPresetNameFromEEPROM(fullState->bankNumber, fullState->menuSelect));
		break;
	case MENU_SAVE_ENTER_NAME:
		lcd->setCursor(6+fullState->menuSelect, menuRow-2);
		lcd->print(allChars[(int)fullState->name[fullState->menuSelect]]);
		lcd->setCursor(6+fullState->menuSelect, menuRow-2);
		lcd->cursor();
		break;
	case MENU_FORMAT_BANK:
		lcd->setCursor(14, menuRow-1);
		lcd->print(allChars[fullState->menuSelect]);
		break;
	case MENU_CONFIG_SETTINGS:
		eraseRow(menuRow-1);
		lcd->setCursor(1, menuRow-1);
		lcd->print(midiConfig[fullState->menuSelect].title);
		if (midiConfig[fullState->menuSelect].valueName != NULL) {
			lcd->print(midiConfig[fullState->menuSelect].valueName[fullState->midiConfigValue[fullState->menuSelect]]);
		} else {
			lcd->print((int)fullState->midiConfigValue[fullState->menuSelect]);
			lcd->print(' ');
		}
		break;
	default:
		break;
	}
}

void FMDisplay::eraseRow(int row) {
	lcd->setCursor(0, row);
	lcd->print("                    ");
}

void FMDisplay::menuBack(FullState* fullState) {
	if (fullState->currentMenuItem->menuState == MENU_DONE) {
		return;
	}
	menuRow --;
	eraseRow(menuRow);
	// -2 because new menu will add 1...
	menuRow--;
}


void FMDisplay::midiClock(boolean show) {
	if (this->synthState->fullState.synthMode  == SYNTH_MODE_EDIT) {
		lcd->setCursor(3,0);
		if (show) {
			lcd->print((char)2);
		} else {
			lcd->print(' ');
		}
	}
}

void FMDisplay::midiIn(boolean show) {
	if (this->synthState->fullState.synthMode  == SYNTH_MODE_EDIT) {
		lcd->setCursor(0,0);
		if (show) {
			lcd->print((char)0);
		} else {
			lcd->print(' ');
		}
	}
}

void FMDisplay::midiOut(boolean show) {
	if (this->synthState->fullState.synthMode  == SYNTH_MODE_EDIT) {
		lcd->setCursor(1,0);
		if (show) {
			lcd->print((char)1);
		} else {
			lcd->print(' ');
		}
	}
}
