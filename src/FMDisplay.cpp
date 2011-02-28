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
	lcd->clear();
}

void FMDisplay::update() {
	if (encoders->rowChanged()) {
		refreshStatus = 8;
	} else if (this->encoders->valueHasChanged()>=0) {
		int row = this->encoders->valueHasChanged() / NUMBER_OF_ENCODERS;
		int encoder =  this->encoders->valueHasChanged() % NUMBER_OF_ENCODERS;
		updateEncoderValue(row, encoder);
	}
	encoders->resetChanged();
}

void FMDisplay::updateEncoderValue(int row, int encoder) {
	struct Parameter param = allParameterRows.row[row]->params[encoder];
	lcd->setCursor(encoder*5, 1);
	if (param.valueName[0] == 0) {
		short value;
		if (param.minValue<0) {
			value = ((char*)(&currentSynthState))[row*NUMBER_OF_ENCODERS+encoder];
		} else {
			value = ((unsigned char*)&currentSynthState)[row*NUMBER_OF_ENCODERS+encoder];
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
		lcd->print(param.valueName[(((unsigned char*)&currentSynthState)[row*NUMBER_OF_ENCODERS+encoder])]);
	}
}

void FMDisplay::updateEncoderName(int row, int encoder) {
	struct ParameterRow* paramRow = allParameterRows.row[row];
	lcd->setCursor(encoder*5, 0);
	lcd->print(paramRow->paramName[encoder]);
}

void FMDisplay::refreshAllScreenByStep() {
	if (refreshStatus>4) {
		updateEncoderName(encoders->getCurrentRow(), refreshStatus -5);
	} else {
		updateEncoderValue(encoders->getCurrentRow(), refreshStatus -1);
	}
	refreshStatus --;
}
