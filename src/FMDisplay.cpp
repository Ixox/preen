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
}

FMDisplay::~FMDisplay() {
}

void FMDisplay::init(LiquidCrystal* lcd) {
	this->lcd = lcd;
	lcd->clear();
	displayPreset();
}

void FMDisplay::printShortValue(short value) {
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

void FMDisplay::updateEncoderValue(int row, int encoder) {
	struct ParameterDisplay param = allParameterRows.row[row]->params[encoder];
	lcd->setCursor(encoder*5, 3);
	short value;
	switch (param.displayType) {
	case DISPLAY_TYPE_STRINGS :
		lcd->print(param.valueName[(((unsigned char*)&synthState.params)[row*NUMBER_OF_ENCODERS+encoder])]);
		break;
	case DISPLAY_TYPE_FLOAT_4_4:
		value = ((unsigned char*)&synthState.params)[row*NUMBER_OF_ENCODERS+encoder];
		lcd->print(value>>4);
		lcd->print(".");
		lcd->print(((value & 0xf) *10) >>4);
		if (value < 160) {
			lcd->print(" ");
		}
		break;
	case DISPLAY_TYPE_FLOAT_5_3:
		value = ((unsigned char*)&synthState.params)[row*NUMBER_OF_ENCODERS+encoder];
		lcd->print(value>>3);
		lcd->print(".");
		lcd->print(((value & 0x7) *10) >>3);
		if (value < 80) {
			lcd->print(" ");
		}
		break;
	case DISPLAY_TYPE_SIGNED_CHAR:
		printShortValue(((char*)&synthState.params)[row*NUMBER_OF_ENCODERS+encoder]);
		break;
	case DISPLAY_TYPE_UNSIGNED_CHAR:
		printShortValue(((unsigned char*)&synthState.params)[row*NUMBER_OF_ENCODERS+encoder]);
		break;
	case DISPLAY_TYPE_NONE:
		lcd->print("    ");
	}
}

void FMDisplay::updateEncoderName(int row, int encoder) {
	struct ParameterRowDisplay* paramRow = allParameterRows.row[row];
	lcd->setCursor(encoder*5, 2);
	lcd->print(paramRow->paramName[encoder]);
}

void FMDisplay::refreshAllScreenByStep() {
	if (refreshStatus==10 ) {
		lcd->setCursor(3,1);
		lcd->print("               ");
	} else if (refreshStatus==9 ) {
		int row = synthState.getCurrentRow();
		lcd->setCursor(0,1);
		lcd->print(allParameterRows.row[row]->rowName);
		if (row>0) {
			lcd->print(" ");
			lcd->print(getRowNumberRelative(row));
		}
	} else if (refreshStatus>4) {
		updateEncoderName(synthState.getCurrentRow(), refreshStatus -5);
	} else {
		updateEncoderValue(synthState.getCurrentRow(), refreshStatus -1);
	}
	refreshStatus --;
}

void FMDisplay::drawMenu(FullState* fullState) {
	switch(fullState->currentMenuState) {
	case MENU_NONE:
		lcd->clear();
		lcd->setCursor(0,0);
		lcd->print(" LOAD  SAVE ");
		lcd->setCursor(fullState->menuSelect*6+2, 1);
		lcd->print("^^");
		break;
	case MENU_SAVE:
		lcd->clear();
		lcd->setCursor(0,0);
		lcd->print(" LOAD >SAVE<");
		lcd->setCursor(0, 3);
		lcd->print(" User Preset ");
		lcd->print(fullState->menuSelect);
		break;
	case MENU_LOAD:
		lcd->clear();
		lcd->setCursor(0,0);
		lcd->print(">LOAD< SAVE ");
		lcd->setCursor(0, 1);
		lcd->print(" Internal  User");
		lcd->setCursor(fullState->menuSelect*8+4, 2);
		lcd->print("^^");
		break;
	case MENU_LOAD_INTERNAL_BANK:
		lcd->clear();
		lcd->setCursor(0,0);
		lcd->print(">LOAD< SAVE");
		lcd->setCursor(0, 1);
		lcd->print(">Internal< User");
		lcd->setCursor(0, 3);
		lcd->print(fullState->menuSelect);
		lcd->print(" - ");
		lcd->print(presets[fullState->menuSelect].presetName);
		break;
	case MENU_LOAD_USER_BANK:
		lcd->clear();
		lcd->setCursor(0,0);
		lcd->print(">LOAD<");
		lcd->setCursor(0, 1);
		lcd->print(" Internal >User<");
		lcd->setCursor(0, 3);
		lcd->print(" Preset ");
		lcd->print(fullState->menuSelect);
		break;
	default:
		break;
	}
}


void FMDisplay::displayPreset() {
	int length = getLength(synthState.params.presetName);
	lcd->clear();
	lcd->setCursor(19-length,0);
	lcd->print(synthState.params.presetName);
}

// Update FMDisplay regarding the callbacks from SynthState


void FMDisplay::newSynthMode(FullState* fullState)  {
	if (fullState->synthMode == SYNTH_MODE_EDIT) {
		displayPreset();
		refreshStatus = 10;
	} else {
		drawMenu(fullState);
	}
}

void FMDisplay::newMenuState(FullState* fullState) {
	drawMenu(fullState);
}

void FMDisplay::newMenuSelect(FullState* fullState) {
	drawMenu(fullState);
}

void FMDisplay::newParamValue(SynthParamListenerType type, int currentRow, int encoder, int oldValue, int newValue) {
	updateEncoderValue(currentRow, encoder);
}

void FMDisplay::newcurrentRow(int newcurrentRow) {
	refreshStatus = 10;
}


