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


void FMDisplay::updateEncoderValue(int row, int encoder) {
	struct Parameter param = allParameterRows.row[row]->params[encoder];
	lcd->setCursor(encoder*5, 3);
	if (param.valueName[0] == 0) {
		short value;
		if (param.minValue<0) {
			value = ((char*)synthStatus.state)[row*NUMBER_OF_ENCODERS+encoder];
		} else {
			value = ((unsigned char*)synthStatus.state)[row*NUMBER_OF_ENCODERS+encoder];
		}
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
	} else {
		lcd->print(param.valueName[(((unsigned char*)synthStatus.state)[row*NUMBER_OF_ENCODERS+encoder])]);
	}
}

void FMDisplay::updateEncoderName(int row, int encoder) {
	struct ParameterRow* paramRow = allParameterRows.row[row];
	lcd->setCursor(encoder*5, 2);
	lcd->print(paramRow->paramName[encoder]);
}

void FMDisplay::refreshAllScreenByStep() {
	if (refreshStatus==10 ) {
		lcd->setCursor(3,1);
		lcd->print("               ");
	} else if (refreshStatus==9 ) {
		int row = synthStatus.getCurrentRow();
		lcd->setCursor(0,1);
		lcd->print(allParameterRows.row[row]->rowName);
		if (row>0) {
			lcd->print(" ");
			lcd->print(getRowNumberRelative(row));
		}
	} else if (refreshStatus>4) {
		updateEncoderName(synthStatus.getCurrentRow(), refreshStatus -5);
	} else {
		updateEncoderValue(synthStatus.getCurrentRow(), refreshStatus -1);
	}
	refreshStatus --;
}

void FMDisplay::drawMenu() {
	switch(synthStatus.getMenuState()) {
	case MENU_NONE:
		lcd->clear();
		lcd->setCursor(0,0);
		lcd->print(" LOAD  SAVE ");
		lcd->setCursor(synthStatus.getMenuSelect()*6+2, 1);
		lcd->print("^^");
		break;
	case MENU_SAVE:
		lcd->clear();
		lcd->setCursor(0,0);
		lcd->print(" LOAD >SAVE<");
		lcd->setCursor(0, 3);
		lcd->print(" User Preset ");
		lcd->print(synthStatus.getMenuSelect());
		break;
	case MENU_LOAD:
		lcd->clear();
		lcd->setCursor(0,0);
		lcd->print(">LOAD< SAVE ");
		lcd->setCursor(0, 1);
		lcd->print(" Internal  User");
		lcd->setCursor(synthStatus.getMenuSelect()*8+4, 2);
		lcd->print("^^");
		break;
	case MENU_LOAD_INTERNAL_BANK:
		lcd->clear();
		lcd->setCursor(0,0);
		lcd->print(">LOAD< SAVE");
		lcd->setCursor(0, 1);
		lcd->print(">Internal< User");
		lcd->setCursor(0, 3);
		lcd->print(" Preset ");
		lcd->print(synthStatus.getMenuSelect());
		break;
	case MENU_LOAD_USER_BANK:
		lcd->clear();
		lcd->setCursor(0,0);
		lcd->print(">LOAD<");
		lcd->setCursor(0, 1);
		lcd->print(" Internal >User<");
		lcd->setCursor(0, 3);
		lcd->print(" Preset ");
		lcd->print(synthStatus.getMenuSelect());
		break;
	default:
		break;
	}
}


void FMDisplay::incParameter(int encoder) {
	if (synthStatus.getEditMode() == MODE_EDIT) {
		updateEncoderValue(synthStatus.getCurrentRow(), encoder);
	} else {
		drawMenu();
	}
}
void FMDisplay::decParameter(int encoder) {
	if (synthStatus.getEditMode() == MODE_EDIT) {
		updateEncoderValue(synthStatus.getCurrentRow(), encoder);
	} else {
		drawMenu();
	}
}
void FMDisplay::displayPreset() {
	int length = getLength(synthStatus.state->presetName);
	lcd->setCursor(0,0);
	lcd->print("                    ");
	lcd->setCursor(19-length,0);
	lcd->print(synthStatus.state->presetName);
}

void FMDisplay::buttonPressed(int button) {

	if (synthStatus.getEditMode() == MODE_EDIT) {
		if (button == BUTTON_MENU) {
			// Just push the button menu
			displayPreset();
		}
		refreshStatus = 10;
	} else {
		drawMenu();
	}

}

