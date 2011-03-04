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


#include "Encoders.h"

class FMDisplay {
public:
	FMDisplay();
	virtual ~FMDisplay();
	void init(Encoders* encoders, LiquidCrystal* lcd);
	void update();
	void drawMenu();
	inline void updateEncoderValue(int row, int encoder);
	inline void updateEncoderName(int row, int encoder);
	int getLength(const char *str) {
		int length = 0;
		for (const char *c = str; *c != '\0'; c++) {
			length++;
		}
		return length;
	}
	boolean needRefresh() { return refreshStatus != 0; }
	void refreshAllScreenByStep();

private:
	Encoders* encoders;
	LiquidCrystal* lcd;
	int refreshStatus;
};

#endif /* FMDISPLAY_H_ */
