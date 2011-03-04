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
#include "SynthStatus.h"


FMDisplay::FMDisplay() {
	refreshStatus = 8;
}

FMDisplay::~FMDisplay() {
}

void FMDisplay::init(Encoders* encoders, LiquidCrystal* lcd) {
	this->encoders = encoders;
	this->lcd = lcd;
}

void FMDisplay::update() {
	if (encoders->isMenuMode()) {
		drawMenu();
	} else {
		if (encoders->menuModeChanged()) {
			int length = getLength(currentSynthState->presetName);
			lcd->setCursor(0,0);
			lcd->print("                    ");
			lcd->setCursor(19-length,0);
			lcd->print(currentSynthState->presetName);
			refreshStatus = 10;
		}
		if (encoders->rowChanged()) {
			refreshStatus = 10;
		} else if (this->encoders->valueHasChanged()>=0) {
			int row = this->encoders->valueHasChanged() / NUMBER_OF_ENCODERS;
			int encoder =  this->encoders->valueHasChanged() % NUMBER_OF_ENCODERS;
			updateEncoderValue(row, encoder);
		}
	}
	encoders->resetChanged();
}

void FMDisplay::updateEncoderValue(int row, int encoder) {
	struct Parameter param = allParameterRows.row[row]->params[encoder];
	lcd->setCursor(encoder*5, 3);
	if (param.valueName[0] == 0) {
		short value;
		if (param.minValue<0) {
			value = ((char*)(currentSynthState))[row*NUMBER_OF_ENCODERS+encoder];
		} else {
			value = ((unsigned char*)currentSynthState)[row*NUMBER_OF_ENCODERS+encoder];
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
		lcd->print(param.valueName[(((unsigned char*)currentSynthState)[row*NUMBER_OF_ENCODERS+encoder])]);
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
		int row = encoders->getCurrentRow();
		lcd->setCursor(0,1);
		lcd->print(allParameterRows.row[row]->rowName);
		if (row>0) {
			lcd->print(" ");
			lcd->print(encoders->getRowNumberRelative());
		}
	} else if (refreshStatus>4) {
		updateEncoderName(encoders->getCurrentRow(), refreshStatus -5);
	} else {
		updateEncoderValue(encoders->getCurrentRow(), refreshStatus -1);
	}
	refreshStatus --;
}

void FMDisplay::drawMenu() {
	switch(encoders->getMenuState()) {
	case MENU_NONE:
		lcd->clear();
		lcd->setCursor(0,0);
		lcd->print(" LOAD  SAVE ");
		lcd->setCursor(encoders->getMenuSelect()*6+2, 1);
		lcd->print("^^");
		break;
	case MENU_SAVE:
		lcd->clear();
		lcd->setCursor(0,0);
		lcd->print(" LOAD >SAVE<");
		lcd->setCursor(0, 3);
		lcd->print(" User Preset ");
		lcd->print(encoders->getMenuSelect());
		break;
	case MENU_LOAD:
		lcd->clear();
		lcd->setCursor(0,0);
		lcd->print(">LOAD< SAVE ");
		lcd->setCursor(0, 1);
		lcd->print(" Internal  User");
		lcd->setCursor(encoders->getMenuSelect()*8+4, 2);
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
		lcd->print(encoders->getMenuSelect());
		break;
	case MENU_LOAD_USER_BANK:
		lcd->clear();
		lcd->setCursor(0,0);
		lcd->print(">LOAD<");
		lcd->setCursor(0, 1);
		lcd->print(" Internal >User<");
		lcd->setCursor(0, 3);
		lcd->print(" Preset ");
		lcd->print(encoders->getMenuSelect());
		break;
	default:
		break;

	}

}

