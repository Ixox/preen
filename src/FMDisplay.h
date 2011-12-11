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


#include "SynthStateAware.h"
#include "LiquidCrystal.h"
#include "Menu.h"




class FMDisplay : public SynthParamListener, public SynthMenuListener, public SynthStateAware {
public:
	FMDisplay();
	~FMDisplay();
	void init(LiquidCrystal* lcd);

	inline void updateEncoderValue(int row, int encoder, ParameterDisplay* param, int newValue);
	inline void updateEncoderName(int row, int encoder);
	inline void printValueWithSpace(int value);
	inline boolean shouldThisValueShowUp(int row, int encoder);
	inline void updateEncoderValue(int refreshStatus);


	int getLength(const char *str) {
		int length = 0;
		for (const char *c = str; *c != '\0'; c++) {
			length++;
		}
		return length;
	}
	int getRowNumberRelative(int row) {
		if (row <= ROW_ENGINE_LAST) {
			return row - ROW_ENGINE_FIRST +1;
		} else if (row <= ROW_OSC_LAST) {
            return row - ROW_OSC_FIRST +1;
        } else if (row <= ROW_ENV_LAST) {
            return row - ROW_ENV_FIRST +1;
        } else if (row <= ROW_MATRIX_LAST) {
            return row - ROW_MATRIX_FIRST +1;
        } else if (row <= ROW_LFO_LAST) {
            return row - ROW_LFO_FIRST +1;
        }
		return 0;
	}

	boolean needRefresh() { return refreshStatus != 0; }

	void refreshAllScreenByStep();
	void displayPreset();
    void checkPresetModified();


    void newSynthMode(FullState* fullState) ;
    void newMenuState(FullState* fullState) ;
    void newMenuSelect(FullState* fullState);
	void menuBack(FullState* fullState);
	void eraseRow(int row);

    void newParamValueFromExternal(SynthParamType type, int currentRow, int encoder, ParameterDisplay* param, int oldValue, int newValue);
    void newParamValue(SynthParamType type, int currentRow, int encoder, ParameterDisplay* param, int oldValue, int newValue);
    void newcurrentRow(int newcurrentRow);
    void updateStepSequencer(int currentRow, int encoder, int oldValue, int newValue);

    void beforeNewParamsLoad() { };
    void afterNewParamsLoad() {
        presetModifed = false;
    }

    // Overide SynthParamListener
    void playNote(char note, char velocity) {
    	lcd->setCursor(3,0);
    	lcd->print((char)7);
    }
    void stopNote(char note) {
    	lcd->setCursor(3,0);
    	lcd->print(' ');
    }


private:
	LiquidCrystal* lcd;
	int refreshStatus;
	int displayedRow;

	int menuRow;
	// Local value preset modified to know whether it's currently showing up
	boolean presetModifed;

};

#endif /* FMDISPLAY_H_ */
