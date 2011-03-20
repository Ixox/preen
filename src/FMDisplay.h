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

#ifndef FMDISPLAY_H_
#define FMDISPLAY_H_


#include "SynthState.h"
#include "LiquidCrystal.h"

class FMDisplay : public SynthParamListener, public SynthMenuListener {
public:
	FMDisplay();
	~FMDisplay();
	void init(LiquidCrystal* lcd);

	void drawMenu(FullState* fullState);
	inline void updateEncoderValue(int row, int encoder, ParameterDisplay* param, int newValue);
	inline void updateEncoderName(int row, int encoder);
	inline void printValueWithSpace(int value);

	int getLength(const char *str) {
		int length = 0;
		for (const char *c = str; *c != '\0'; c++) {
			length++;
		}
		return length;
	}
	int getRowNumberRelative(int row) {
		if (row<5) {
			return row;
		} else if (row<9) {
			return row - 4;
		} else if (row<15) {
			return row- 8;
		} else {
			return row - 14;
		}
	}

	boolean needRefresh() { return refreshStatus != 0; }
	void refreshAllScreenByStep();
	void displayPreset();


    void newSynthMode(FullState* fullState) ;
    void newMenuState(FullState* fullState) ;
    void newMenuSelect(FullState* fullState);

    void newParamValueFromExternal(SynthParamListenerType type, int currentRow, int encoder, ParameterDisplay* param, int oldValue, int newValue);
    void newParamValue(SynthParamListenerType type, int currentRow, int encoder, ParameterDisplay* param, int oldValue, int newValue);
    void newcurrentRow(int newcurrentRow);



private:
	LiquidCrystal* lcd;
	int refreshStatus;
	int displayedRow;
};

#endif /* FMDISPLAY_H_ */
