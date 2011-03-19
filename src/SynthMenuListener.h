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


#ifndef SYNTHMENULISTENER_H_
#define SYNTHMENULISTENER_H_


enum SynthMode {
	SYNTH_MODE_EDIT = 0,
	SYNTH_MODE_MENU
};

enum MenuState {
	MENU_NONE = 0,
	MENU_LOAD,
	MENU_SAVE,
	MENU_LOAD_INTERNAL_BANK,
	MENU_LOAD_USER_BANK,
	MENU_SAVE_PRESET,
	MENU_DONE,
	MENU_ENTER_NAME
};

struct FullState {
	SynthMode synthMode;
	MenuState currentMenuState;
	int menuSelect;
	int presetNumber;
	char name[13];
};


class SynthMenuListener {
public:

    virtual void newSynthMode(FullState* fullState) = 0;
    virtual void newMenuState(FullState* fullState) = 0;
    virtual void newMenuSelect(FullState* fullState) = 0;


    SynthMenuListener* nextListener;
};

#endif /* SYNTHMENULISTENER_H_ */
