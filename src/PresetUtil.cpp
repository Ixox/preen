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

#include "PresetUtil.h"


char PresetUtil::readName[13];

PresetUtil::PresetUtil() {
}

PresetUtil::~PresetUtil() {
}


void PresetUtil::dumpLine(int a, int b, int c, int d) {
    SerialUSB.print("{ ");
    SerialUSB.print(a);
    SerialUSB.print(", ");
    SerialUSB.print(b);
    SerialUSB.print(", ");
    SerialUSB.print(c);
    SerialUSB.print(", ");
    SerialUSB.print(d);
    SerialUSB.print("} ");
    SerialUSB.println(", ");
}


void PresetUtil::dumpPatch() {
    dumpLine(synthState.params.engine1.algo, synthState.params.engine1.velocity, synthState.params.engine1.numberOfVoice, synthState.params.engine1.glide );
    dumpLine(synthState.params.engine2.modulationIndex1, synthState.params.engine2.modulationIndex2, synthState.params.engine2.modulationIndex3, synthState.params.engine2.modulationIndex4 );
    dumpLine(synthState.params.engine3.mixOsc1, synthState.params.engine3.mixOsc2, synthState.params.engine3.mixOsc3, synthState.params.engine3.mixOsc4 );
    OscillatorParams * o = (OscillatorParams *)(&(synthState.params.osc1));
    for (int k=0; k<6; k++) {
        dumpLine(o[k].shape, o[k].frequencyType, o[k].frequencyMul, o[k].detune);
    }
    EnvelopeParams * e = (EnvelopeParams*)(&(synthState.params.env1));
    for (int k=0; k<6; k++) {
        dumpLine(e[k].attack, e[k].decay, e[k].sustain, e[k].release);
    }
    MatrixRowParams* m = (MatrixRowParams*)(&(synthState.params.matrixRowState1));
    for (int k=0; k<8; k++) {
        dumpLine(m[k].source, m[k].mul, m[k].destination, 0);
    }
    LfoParams* l = (LfoParams*)(&(synthState.params.lfo1));
    for (int k=0; k<4; k++) {
        dumpLine(l[k].shape, l[k].freq, l[k].bias, l[k].keybRamp);
    }
    SerialUSB.println(synthState.params.presetName);
}


void PresetUtil::readFromEEPROM(int bankNumber, int preset) {

    uint8 deviceaddress = 0b1010000;
    int address = preset*128 +  bankNumber * 128*128;
    uint8 bufReadAddress[2];
    i2c_msg msgsRead[2];
    int block1Size = 64;

    bufReadAddress[0] = (uint8)((int)address >> 8);
    bufReadAddress[1] = (uint8)((int)address & 0xff);

    msgsRead[0].addr = deviceaddress;
    msgsRead[0].flags = 0;
    msgsRead[0].length = 2;
    msgsRead[0].data = bufReadAddress;

    msgsRead[1].addr = deviceaddress;
    msgsRead[1].flags = I2C_MSG_READ;
    msgsRead[1].length = block1Size;
    msgsRead[1].data = (uint8*)&synthState.params;

    i2c_master_xfer(I2C1, msgsRead, 2, 500);

    delay(20);

    int block2Size = sizeof(struct AllSynthParams) - block1Size;

    address = address + block1Size;
    bufReadAddress[0] = (uint8)((int)address >> 8);
    bufReadAddress[1] = (uint8)((int)address & 0xff);

    msgsRead[0].addr = deviceaddress;
    msgsRead[0].flags = 0;
    msgsRead[0].length = 2;
    msgsRead[0].data = bufReadAddress;

    msgsRead[1].addr = deviceaddress;
    msgsRead[1].flags = I2C_MSG_READ;
    msgsRead[1].length = block2Size;
    msgsRead[1].data = &((uint8*)&synthState.params)[block1Size];
    i2c_master_xfer(I2C1, msgsRead, 2, 500);

    delay(20);
}


char* PresetUtil::readPresetNameFromEEPROM(int bankNumber, int preset) {

    uint8 deviceaddress = 0b1010000;
    int address = preset*128 +  bankNumber * 128*128 + 108;
    uint8 bufReadAddress[2];
    i2c_msg msgsRead[2];

    bufReadAddress[0] = (uint8)((int)address >> 8);
    bufReadAddress[1] = (uint8)((int)address & 0xff);

    msgsRead[0].addr = deviceaddress;
    msgsRead[0].flags = 0;
    msgsRead[0].length = 2;
    msgsRead[0].data = bufReadAddress;

    msgsRead[1].addr = deviceaddress;
    msgsRead[1].flags = I2C_MSG_READ;
    msgsRead[1].length = 13;
    msgsRead[1].data = (uint8*)readName;

    i2c_master_xfer(I2C1, msgsRead, 2, 500);

    delay(20);

    return readName;
}



void PresetUtil::pruneToEEPROM(int bankNumber, int preset) {
    uint8 deviceaddress = 0b1010000;
    i2c_msg msgWrite1, msgWrite2;
    int block1Size = 64;
    int address = preset*128 +  bankNumber * 128*128;
    uint8 bufWrite1[block1Size + 2];

    bufWrite1[0] = (uint8)((int)address >> 8);
    bufWrite1[1] = (uint8)((int)address & 0xff);
    for (int k=0; k<block1Size; k++) {
        bufWrite1[k+2] = ((uint8*)&synthState.params)[k];
    }
    /* Write test pattern  */
    msgWrite1.addr = deviceaddress;
    msgWrite1.flags = 0;
    msgWrite1.length = block1Size +2;
    msgWrite1.data = bufWrite1;
    i2c_master_xfer(I2C1, &msgWrite1, 1, 500);
    delay(5);

    int block2Size = sizeof(struct AllSynthParams) - block1Size;
    uint8 bufWrite2[block2Size + 2];
    address = address + block1Size;
    bufWrite2[0] = (uint8)((int)address >> 8);
    bufWrite2[1] = (uint8)((int)address & 0xff);
    for (int k=0; k<block2Size; k++) {
        bufWrite2[k+2] = ((uint8*)&synthState.params)[k+block1Size];
    }
    msgWrite2.addr = deviceaddress;
    msgWrite2.flags = 0;
    msgWrite2.length = block2Size + 2;
    msgWrite2.data = bufWrite2;
    i2c_master_xfer(I2C1, &msgWrite2, 1, 500);
    delay(5);

}




void PresetUtil::midiPatchDump() {
    uint8 newPatch[] = {0xf0, 0x7d, 0x01};

    for (int k=0; k<=2; k++) {
        Serial3.print(newPatch[k]);
    }

    int checksum = 0;
    int total = sizeof(struct AllSynthParams);

    for (int k=0; k<total; k++) {
        uint8 byte = ((unsigned char*)&synthState.params)[k];
        checksum+= byte;
        while (byte >= 127) {
            Serial3.print((uint8)127);
            byte -= 127;
        }
        Serial3.print(byte);
    }

    Serial3.print((uint8)(checksum % 128));
    Serial3.print((uint8)0xf7);
}

void PresetUtil::formatEEPROM() {
    uint8 deviceaddress = 0b1010000;
    for (int bankNumber=0; bankNumber<3; bankNumber++) {
        for (int preset =0; preset<128; preset++) {

            i2c_msg msgWrite1, msgWrite2;
            int block1Size = 64;
            int address = preset*128 +  bankNumber * 128*128;
            uint8 bufWrite1[block1Size + 2];

            bufWrite1[0] = (uint8)((int)address >> 8);
            bufWrite1[1] = (uint8)((int)address & 0xff);
            for (int k=0; k<block1Size; k++) {
                bufWrite1[k+2] = ((uint8*)&presets[bankNumber])[k];
            }
            /* Write test pattern  */
            msgWrite1.addr = deviceaddress;
            msgWrite1.flags = 0;
            msgWrite1.length = block1Size +2;
            msgWrite1.data = bufWrite1;
            i2c_master_xfer(I2C1, &msgWrite1, 1, 500);
            delay(5);

            int block2Size = sizeof(struct AllSynthParams) - block1Size;
            uint8 bufWrite2[block2Size + 2];
            address = address + block1Size;
            bufWrite2[0] = (uint8)((int)address >> 8);
            bufWrite2[1] = (uint8)((int)address & 0xff);
            for (int k=0; k<block2Size; k++) {
                bufWrite2[k+2] = ((uint8*)&presets[bankNumber])[k+block1Size];
            }
            msgWrite2.addr = deviceaddress;
            msgWrite2.flags = 0;
            msgWrite2.length = block2Size + 2;
            msgWrite2.data = bufWrite2;
            i2c_master_xfer(I2C1, &msgWrite2, 1, 500);
            delay(5);
        }
    }
}

