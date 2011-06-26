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



FMDisplay::FMDisplay() {
	refreshStatus = 10;
	presetModifed = false;
}

FMDisplay::~FMDisplay() {
}

void FMDisplay::init(LiquidCrystal* lcd) {
	this->lcd = lcd;
	lcd->clear();
	displayPreset();
}

void FMDisplay::printValueWithSpace(int value) {
    lcd->print(value);

	if (value>99) {
		lcd->print(" ");
	} else if (value>9) {
		lcd->print("  ");
	} else if (value>-1) {
		lcd->print("   ");
	} else if (value>-10) {
		lcd->print("  ");
	} else if (value>-100) {
		lcd->print(" ");
	}
}


boolean FMDisplay::shouldThisValueShowUp(int row, int encoder) {
    int algo = synthState.params.engine1.algo;
    if (row == ROW_MODULATION && (encoder+1)> showUp[algo].im) {
        return false;
    }
    if (row == ROW_OSC_MIX && (encoder+1)> showUp[algo].mix) {
        return false;
    }
    if (row == ROW_ENGINE && encoder == ENCODER_ENGINE_GLIDE && synthState.params.engine1.numberOfVoice >1) {
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
        lcd->print(" ");
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
        lcd->print(" ");
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
        lcd->print(" ");
        break;
    }
	case DISPLAY_TYPE_SIGNED_CHAR:
displaySignedChar:
	case DISPLAY_TYPE_UNSIGNED_CHAR:
		printValueWithSpace(newValue);
		break;
	case DISPLAY_TYPE_OSC_FREQUENCY:
	{
		// Hack... to deal with the special case of the fixe frequency.....
		int oRow = row - ROW_OSC_FIRST;
		OscillatorParams* oParam = (OscillatorParams*)&synthState.params.osc1;
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
	}
}

void FMDisplay::updateEncoderName(int row, int encoder) {
    lcd->setCursor(encoder*5, 2);
    if (!shouldThisValueShowUp(row, encoder)) {
        lcd->print("    ");
        return;
    }
	struct ParameterRowDisplay* paramRow = allParameterRows.row[row];
	lcd->print(paramRow->paramName[encoder]);
}

void FMDisplay::refreshAllScreenByStep() {
    switch (refreshStatus) {
    case 10:
		lcd->setCursor(3,1);
		lcd->print("               ");
		break;
    case 9:
    {
		int row = synthState.getCurrentRow();
		lcd->setCursor(0,1);
		lcd->print(allParameterRows.row[row]->rowName);
		if (row> ROW_ENGINE_LAST) {
            lcd->print(" ");
            lcd->print(getRowNumberRelative(row));
		}
        break;
    }
    case 5:
    case 6:
    case 7:
    case 8:
		updateEncoderName(synthState.getCurrentRow(), refreshStatus -5);
        break;
    default :
	    updateEncoderValue(refreshStatus);

	}
    refreshStatus --;
}

 void FMDisplay::updateEncoderValue(int refreshStatus) {
    int row = synthState.getCurrentRow();
    struct ParameterDisplay param = allParameterRows.row[row]->params[refreshStatus -1];
    int newValue;
    if (param.displayType == DISPLAY_TYPE_SIGNED_CHAR) {
        newValue = ((char*)&synthState.params)[row*NUMBER_OF_ENCODERS+refreshStatus -1];
    } else {
        newValue = ((unsigned char*)&synthState.params)[row*NUMBER_OF_ENCODERS+refreshStatus -1];
    }
    updateEncoderValue(synthState.getCurrentRow(), refreshStatus -1, &param, newValue);
}


void FMDisplay::displayPreset() {
    FullState* fullState = &synthState.fullState;
	int length = getLength(synthState.params.presetName);
	lcd->setCursor(19-length,0);
	lcd->print(synthState.params.presetName);
	if (fullState->presetModified) {
	    lcd->print((char)2);
	}
}

void FMDisplay::checkPresetModified() {
    if (!presetModifed && synthState.fullState.presetModified && synthState.fullState.synthMode == SYNTH_MODE_EDIT) {
        presetModifed = true;
        lcd->setCursor(19,0);
        lcd->print((char)2);
    }
}



// Update FMDisplay regarding the callbacks from SynthState


void FMDisplay::newParamValueFromExternal(SynthParamType type, int currentRow, int encoder, ParameterDisplay* param, int oldValue, int newValue) {
    checkPresetModified();
	if (currentRow == this->displayedRow) {
		updateEncoderValue(currentRow, encoder, param, newValue);
	}
}

void FMDisplay::newParamValue(SynthParamType type, int currentRow, int encoder, ParameterDisplay* param,  int oldValue, int newValue) {
    checkPresetModified();
	if (synthState.getSynthMode() == SYNTH_MODE_EDIT) {
		if (currentRow != this->displayedRow) {
			newcurrentRow(currentRow);
			return;
		}
		// If we change frequency type of OScillator rows, it's a bit special....
		if (SynthState::getListenerType(currentRow)==SYNTH_PARAM_TYPE_OSC && encoder == ENCODER_OSC_FTYPE) {
			refreshStatus = 4;
			return;
		}

		updateEncoderValue(currentRow, encoder, param, newValue);
	}
}

void FMDisplay::newcurrentRow(int newcurrentRow) {
	refreshStatus = 10;
	this->displayedRow = newcurrentRow;
}


/*
 * Menu Listener
 */

void FMDisplay::newSynthMode(FullState* fullState)  {
	lcd->clear();
	if (fullState->synthMode == SYNTH_MODE_EDIT) {
		displayPreset();
		refreshStatus = 10;
	} else {
		menuRow = 0;
		newMenuState(fullState);
	}
}

void FMDisplay::newMenuState(FullState* fullState) {
	menuRow++;
	if (fullState->currentMenuItem->hasSubMenu) {
		for (int k=0; k<fullState->currentMenuItem->maxValue; k++) {
			lcd->setCursor(k*5+1, menuRow-1);
			lcd->print(MenuItemUtil::getMenuItem(fullState->currentMenuItem->subMenu[k])->name);
		}
	}

	switch (fullState->currentMenuItem->menuState) {
		case MENU_LOAD_USER_SELECT_BANK:
		case MENU_SAVE_SELECT_USER_BANK:
			lcd->setCursor(1, menuRow-1);
			lcd->print("Bnk1 Bnk2 Bnk3");
			break;
		case MENU_SAVE_ENTER_NAME:
			lcd->setCursor(1, menuRow-1);
			for (int k=0;k<12; k++) {
				lcd->print(allChars[(int)fullState->name[k]]);
			}
			break;
		case MENU_MIDI_CHANNEL:
			lcd->setCursor(1, menuRow-1);
			lcd->print("Midi channel:");
			break;
		case MENU_FORMAT_BANK:
			lcd->setCursor(1, menuRow-1);
			lcd->print("Confirm Format ?");
			break;
	}

	newMenuSelect(fullState);

}

void FMDisplay::newMenuSelect(FullState* fullState) {
	lcd->noCursor();

	switch(fullState->currentMenuItem->menuState) {
	case MAIN_MENU:
	case MENU_LOAD:
	case MENU_MIDI:
	case MENU_MIDI_BANK:
	case MENU_MIDI_PATCH:
	case MENU_LOAD_USER_SELECT_BANK:
	case MENU_SAVE_SELECT_USER_BANK:
		for (int k=0; k<4; k++) {
			lcd->setCursor(k*5, menuRow-1);
			lcd->print(" ");
		}
		lcd->setCursor(fullState->menuSelect*5, menuRow-1);
		lcd->print(">");
		break;
	case MENU_LOAD_USER_SELECT_PRESET:
	case MENU_LOAD_INTERNAL:
		eraseRow(menuRow-1);
		lcd->setCursor(2, menuRow-1);
		lcd->print(fullState->menuSelect);
		lcd->print(" - ");
		lcd->print(synthState.params.presetName);
		break;
	case MENU_DONE:
		lcd->clear();
		lcd->setCursor(8,1);
		lcd->print("DONE");
		break;
	case MENU_SAVE_SELECT_PRESET:
		eraseRow(menuRow-1);
		lcd->setCursor(4, menuRow-1);
		lcd->print(fullState->menuSelect);
        lcd->print(" - ");
        lcd->print(PresetUtil::readPresetNameFromEEPROM(fullState->bankNumber, fullState->menuSelect));
		break;
	case MENU_SAVE_ENTER_NAME:
		/*
		lcd->setCursor(1, menuRow-1);
		for (int k=0;k<12; k++) {
			lcd->print(allChars[(int)fullState->name[k]]);
		}*/
		lcd->setCursor(1+fullState->menuSelect, menuRow-1);
		lcd->print(allChars[(int)fullState->name[fullState->menuSelect]]);
		lcd->setCursor(1+fullState->menuSelect, menuRow-1);
		lcd->cursor();
		break;
	case MENU_MIDI_CHANNEL:
		lcd->setCursor(15, menuRow-1);
		lcd->print(fullState->menuSelect+1);
		lcd->print(" ");
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
	menuRow --;
	eraseRow(menuRow);
	// -2 because new menu will add 1...
	menuRow--;
}

