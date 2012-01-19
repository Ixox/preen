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

#pragma once

#include "SynthStateAware.h"
#include "Matrix.h"



extern short sinTable[];

#define RANDOM sinTable[2048]

extern const short waveforms[];
extern const int frequenciesX8[];

struct OscState {
    int index;
    int frequency;
    int mainFrequencyPlusMatrix;
    int mainFrequency;
    int fromFrequency;
    int nextFrequency;
};

template <int number>
class Osc : public SynthStateAware
{
public:
    Osc();
    ~Osc(void);
    void init(Matrix* matrix);

    void newNote(struct OscState& oscState, int note);
    void glideToNote(struct OscState& oscState, int note);
    void glideStep(struct OscState& oscState, int step);

    void calculateFrequencyWithMatrix(struct OscState *oscState) {
		oscState->mainFrequencyPlusMatrix = oscState->mainFrequency + ((oscState->mainFrequency   * (this->matrix->getDestination(destFreq) + this->matrix->getDestination(ALL_OSC_FREQ))) >> 14);
    }

    int getNextSample(struct OscState *oscState) __attribute__((always_inline)) {
        int oscValue;

        /*
        oscState->index +=  oscState->frequency;
        oscState->index &= 0x3ffff;

        switch(oscillator->shape) {
        case OSC_SHAPE_SIN:
            return  waveforms[index >> 7]; // * ((1024 + this->matrix->getDestination(destAmp)) >> 10) ;
            break;
        case OSC_SHAPE_SIN2:
        {
            int s = waveforms[index >> 7];
            return   (s*s) >> 16;
            break;
        }
        case OSC_SHAPE_SIN3:
            return  waveforms[index >> 7] * (index<0x1ffff); // * ((1024 + this->matrix->getDestination(destAmp)) >> 10) ;
            break;
        case OSC_SHAPE_SIN4:
            index &= 0x1ffff;
            return  waveforms[index >> 7];
            break;
        case OSC_SHAPE_RAND:
            return  (randomOsc * index) & 0x7fff;
            break;
        }
        return index;
*/


        asm volatile(

        		// Frequency is prepared in Voice.cpp

        		// oscState->index += oscState->frequency;
                // oscState->index &= 0x3ffff;

                // r5 : index
        		// r6 : frequency,

        		"    ldm %[osc], {r5-r6}\n\t"
                "    add r5, r5, r6\n\t"
                "    mvn r6, #0\n\t"
                "    lsr r6, r6, #14\n\t"
                "    and r5, r5, r6\n\t"
                "    str r5, [%[osc], #0]\n\t"

                // Switch
                "    tbb [pc, %[shape]]\n\t"
                "7:\n\t"
                "    .byte   (1f-7b)/2\n\t"
                "    .byte   (2f-7b)/2\n\t"
                "    .byte   (3f-7b)/2\n\t"
                "    .byte   (4f-7b)/2\n\t"
                "    .byte   (5f-7b)/2\n\t"
                "    .byte   (9f-7b)/2\n\t"
                "    .byte   (10f-7b)/2\n\t"
                "    .byte   (8f-7b)/2\n\t"
                "    .align  1\n\t"

                // OSC_SHAPE_SIN
                "1: \n\t"
                "    lsr r5, r5, #7\n\t"
                "    ldrsh %[value], [ %[sinTable], r5, lsl #1]\n\t"
                "    b 6f\n\t"
                // OSC_SHAPE_SIN2
                "2:  \n\t"
                "    lsr r5, r5, #7\n\t"
                "    ldrsh %[value], [ %[sinTable], r5, lsl #1]\n\t"
                "    mul %[value], %[value], %[value]\n\t"
                "    lsr %[value], #15\n\t"
                "    b 6f\n\t"

                // OSC_SHAPE_SIN3
                "3:  \n\t"
                "    lsr r5, r5, #7\n\t"
                "    ldrsh %[value], [ %[sinTable], r5, lsl #1]\n\t"
                // if negativ replace by 0
                "    cmp  %[value], #0\n\t"
                "    it lt\n\t"
                "    movlt %[value], #0\n\t"
                "    b 6f\n\t"

                // OSC_SHAPE_SIN4
                "4:  \n\t"
                "    lsr r5, r5, #7\n\t"
                "    ldrsh %[value], [ %[sinTable], r5, lsl #1]\n\t"
                // if negativ take the oposite
                "    cmp  %[value], #0\n\t"
                "    it mi\n\t"
                "    rsbmi %[value], %[value], #0\n\t"
                "    b 6f\n\t"

        		// OSC_SHAPE_SQUARE
                "9:  \n\t"
                "    lsr r5, r5, #7\n\t"
                "    ldrsh %[value], [ %[waveforms], r5, lsl #1]\n\t"
                "    b 6f\n\t"

        		// OSC_SHAPE_SAW
                "10:  \n\t"
                "    lsr r5, r5, #7\n\t"
                "    add r5, #2048\n\t"
                "    ldrsh %[value], [ %[waveforms], r5, lsl #1]\n\t"
                "    b 6f\n\t"


                // OSC_SHAPE_RAND
                "8:  \n\t"
                "    mov %[value], #0\n\t"
                "    b 6f\n\t"

                // OSC_SHAPE_RAND
                "5:  \n\t"
                "    mov r5, #2048\n\t"
                "    ldrsh %[value], [ %[sinTable], r5, lsl #1]\n\t"

                // BREAK
                "6:\n\t"
                : [value] "=r"(oscValue), [osc]"+rV" (oscState)
                : [sinTable]"rV"(sinTable), [waveforms]"rV"(waveforms), [shape]"r" (oscillator->shape)
                : "cc", "r5", "r6", "r7"
        );

        return  oscValue;

    };


    static void updateRandomNumber() __attribute__((always_inline)) {
    	RANDOM = random(65535) - 32768;
    }
    OscillatorParams* oscillator;

private:
    DestinationEnum destFreq;
    Matrix* matrix;
};



template <int number>
Osc<number>::Osc()
{
    switch (number-1) {
    case 0:
        this->destFreq = OSC1_FREQ;
        break;
    case 1:
        this->destFreq = OSC2_FREQ;
        break;
    case 2:
        this->destFreq = OSC3_FREQ;
        break;
    case 3:
        this->destFreq = OSC4_FREQ;
        break;
    case 4:
        this->destFreq = OSC5_FREQ;
        break;
    case 5:
        this->destFreq = OSC6_FREQ;
        break;
    }
}

template <int number>
Osc<number>::~Osc()
{
}

template <int number>
void Osc<number>::init(Matrix* matrix) {
    this->matrix = matrix;

    OscillatorParams * o = (OscillatorParams *)(&(this->synthState->params.osc1));
    oscillator = &o[number-1];
}

template <int number>
void Osc<number>::newNote(struct OscState& oscState, int note) {
    oscState.index = 1; // << number;
    switch (oscillator->frequencyType) {
    case OSC_FT_KEYBOARD:
    	// frequency : 15 bits + oscillator 7 - 4 + 10 - 9
    	// MAX : 21 bits !
        oscState.mainFrequency = (((frequenciesX8[note] * oscillator->frequencyMul) >> 4) * (2048 + oscillator->detune)) >> 11;
        break;
    case OSC_FT_FIXE:
        oscState.mainFrequency = ((oscillator->frequencyMul << 7) + oscillator->detune)<<2;
        break;
    }
    oscState.frequency = oscState.mainFrequency;
}


template <int number>
void Osc<number>::glideToNote(struct OscState& oscState, int note) {
    switch (oscillator->frequencyType) {
    case OSC_FT_KEYBOARD:
        oscState.nextFrequency = (((frequenciesX8[note] * oscillator->frequencyMul) >> 4) * (2048 + oscillator->detune)) >> 11;
        break;
    case OSC_FT_FIXE:
        oscState.nextFrequency = ((oscillator->frequencyMul << 7) + oscillator->detune)<<2;
        break;
    }
    oscState.fromFrequency = oscState.mainFrequency;
}


template <int number>
void Osc<number>::glideStep(struct OscState& oscState, int step) {
    oscState.mainFrequency = ((oscState.fromFrequency * ((1<<this->synthState->params.engine1.glide) - step)) + oscState.nextFrequency * step) >> this->synthState->params.engine1.glide;
    oscState.frequency = oscState.mainFrequency;
}
