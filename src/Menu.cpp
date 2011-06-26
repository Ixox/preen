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


#include "Menu.h"

struct MenuItem allMenus[] __attribute__ ((section (".USER_FLASH"))) = {
	{
		MAIN_MENU,
		"",
		true,
		4,
		{MENU_LOAD, MENU_SAVE_SELECT_USER_BANK, MENU_MIDI, MENU_FORMAT_BANK }
	},
	// === LOAD
	{
		MENU_LOAD,
		"Load",
		true,
		2,
		{MENU_LOAD_INTERNAL, MENU_LOAD_USER_SELECT_BANK}
	},
	{
		MENU_LOAD_INTERNAL,
		"Pres",
		false,
		INTERNAL_LAST_BANK,
		{MENU_DONE}
	},
	{
		MENU_LOAD_USER_SELECT_BANK,
		"User",
		false,
		3,
		{MENU_LOAD_USER_SELECT_PRESET}
	},
	{
		MENU_LOAD_USER_SELECT_PRESET,
		"",
		false,
		127,
		{MENU_DONE}
	},
	// === SAVE
	{
		MENU_SAVE_SELECT_USER_BANK,
		"Save",
		false,
		3,
		{MENU_SAVE_SELECT_PRESET}
	},
	{
		MENU_SAVE_SELECT_PRESET,
		"",
		false,
		127,
		{MENU_SAVE_ENTER_NAME}
	},
	{
		MENU_SAVE_ENTER_NAME,
		"Enter name",
		false,
		12,
		{MENU_DONE}
	},
	// === MIDI
	{
		MENU_MIDI,
		"Midi",
		true,
		3,
		{MENU_MIDI_CHANNEL, MENU_MIDI_BANK, MENU_MIDI_PATCH }
	},
	{
		MENU_MIDI_CHANNEL,
		"Chan",
		false,
		16,
		{MENU_DONE}
	},
	{
		MENU_MIDI_BANK,
		"Bank",
		true,
		2,
		{MENU_MIDI_BANK_GET, MENU_MIDI_BANK_DUMP}
	},
	{
		MENU_MIDI_BANK_GET,
		"Get",
		false,
		0,
		{MENU_DONE}
	},
	{
		MENU_MIDI_BANK_DUMP,
		"Dump",
		false,
		0,
		{MENU_DONE}
	},
	{
		MENU_MIDI_PATCH,
		"Patc",
		true,
		2,
		{MENU_MIDI_PATCH_GET, MENU_MIDI_PATCH_DUMP}
	},
	{
		MENU_MIDI_PATCH_GET,
		"Get",
		false,
		0,
		{MENU_DONE}
	},
	{
		MENU_MIDI_PATCH_DUMP,
		"Dump",
		false,
		0,
		{MENU_DONE}
	},
	// === DONE
	{
		MENU_DONE,
		"",
		false,
		0,
		{MENU_DONE}
	},
	// ==  FORMAT BANK
	{
		MENU_FORMAT_BANK,
		"Frmt",
		true,
		1,
		{MENU_DONE}
	}
};

