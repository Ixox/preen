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

#ifndef PRESETUTIL_H_
#define PRESETUTIL_H_

#define EEPROM_CONFIG_CHECK 39

#include "libmaple_types.h"
#include "wirish.h"
#include "i2c.h"

#define EEPROM1_ID 0b1010000;
#define EEPROM2_ID 0b1010001;



class SynthState;

// is included by SynthState so cannot be SynthStateAware...
// have to implement its own get/set synthState

class PresetUtil  {
public:
    PresetUtil();
    ~PresetUtil();

    static void setSynthState(SynthState* synthState);


    static void dumpPatch();
    static void dumpLine(int a, int b, int c, int d);

    static void readFromEEPROM(uint8 bankNumber, uint8 preset, char* params);
    static char* readPresetNameFromEEPROM(int bankNumber, int preset);
    static void savePatchToEEPROM(uint8* params, int bankNumber, int preset);
    static void saveCurrentPatchToEEPROM(int bankNumber, int preset);
    static void formatEEPROM();
    static void saveConfigToEEPROM();
    static void loadConfigFromEEPROM();

    static void sendBankToSysex(int bankNumber);
    static void sendCurrentPatchToSysex();
    static void sendParamsToSysex(char* params, int size);
    static int  readSysex(bool patchAllowed, bool bankAllowed);
    static int  readSysexPatch(char* params);
    static int  readSysexBank();

    static void copyBank(int source, int dest);

    static int  getNextMidiByte();
    static void copyPatch(char* source, char* dest);

    static void loadDefaultPatchIfAny();
    static void saveCurrentPatchAsDefault();

private:
    static char readName[13];
    static SynthState * synthState;

    static uint8 getDeviceId(int bankNumber);
    static int getAddress(int bankNumber, int preset);



};

#endif /* PRESETUTIL_H_ */
