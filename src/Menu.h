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


#ifndef MENU_H_
#define MENU_H_

#define INTERNAL_LAST_BANK 11


enum SynthMode {
	SYNTH_MODE_EDIT = 0,
	SYNTH_MODE_MENU
};

enum MenuState {
	MAIN_MENU = 0,
	MENU_LOAD,
	MENU_SAVE_CHOOSE_PRESET,
	MENU_SAVE_CHOOSE_USER_BANK,
	MENU_SAVE_PRESET,
	MENU_LOAD_INTERNAL_BANK,
	MENU_LOAD_USER_SELECT_BANK,
	MENU_LOAD_USER_BANK,
	MENU_LOAD_CHOOSE_USER_BANK,
	MENU_ENTER_NAME,
	MENU_MIDI,
	MENU_MIDI_CHANNEL,
	MENU_MIDI_BANK,
	MENU_MIDI_BANK_GET,
	MENU_MIDI_BANK_DUMP,
	MENU_MIDI_PATCH,
	MENU_MIDI_PATCH_GET,
	MENU_MIDI_PATCH_DUMP,
	MENU_DONE,
	MENU_FORMAT_BANK,
	LAST_MENU
};


struct MenuItem {
	MenuState menuState;
	const char* name;
	bool hasSubMenu;
	short maxValue;
	MenuState subMenu[4];
};

struct FullState {
	SynthMode synthMode;
	int menuSelect;
    int previousMenuSelect;
	int presetNumber;
	int bankNumber;
	MenuItem* currentMenuItem;
	char name[13];
	unsigned char midiChannel;
	bool presetModified;
};

extern struct MenuItem allMenus[];

class MenuItemUtil {
public:
	static MenuItem* getMenuItem(MenuState ms) {
		MenuItem* item = &allMenus[0];
		int cpt = 0;
		while (item->menuState != LAST_MENU) {
			if (item->menuState == ms) {
				return item;
			}
			cpt ++;
			item = &allMenus[cpt];
		}
		return 0;
	}

    static MenuItem* getParentMenuItem(MenuState ms) {
        MenuItem* item = &allMenus[0];
        int cpt = 0;
        while (item->menuState != LAST_MENU) {
            for (int k=0; k<4; k++) {
                if (item->subMenu[k] == ms) {
                    return item;
                }
            }
            cpt ++;
            item = &allMenus[cpt];
        }
        return 0;
    }

    static int getParentMenuSelect(MenuState ms) {
        MenuItem* item = &allMenus[0];
        int cpt = 0;
        while (item->menuState != LAST_MENU) {
            for (int k=0; k<4; k++) {
                if (item->subMenu[k] == ms) {
                    return k;
                }
            }
            cpt ++;
            item = &allMenus[cpt];
        }
        return 0;
    }
};




#endif /* MENU_H_ */
