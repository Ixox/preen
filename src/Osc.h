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

#include "Matrix.h"

#define SAMPLE_RATE_x_8 264144


extern short sinTable[];
extern int frequenciesx8[];
extern int randomOsc;

struct OscState {
    int index;
    int frequency;
    int mainFrequency;
    int fromFrequency;
    int nextFrequency;
};

template <int number>
class Osc
{
public:
    Osc();
    ~Osc(void);
    void init(Matrix* matrix);

    void newNote(struct OscState& oscState, int note);
    void glideToNote(struct OscState& oscState, int note);
    void glideStep(struct OscState& oscState, int step);

    /*
	int getSample(struct OscState &oscState) {
		int rest = oscState.index & 0x7f;
		int div128 = oscState.index >> 7;
		int div128plus1 = (div128 + 1) & 0x7ff;
		return (sinTable[div128]* (128-rest) + sinTable[div128plus1]*rest) >> 8;
	}
     */

    int getNextSample(struct OscState *oscState) __attribute__((always_inline)) {
        int oscValue;
        oscState->index += (this->matrix->getDestination(destFreq) >> 4) + oscState->frequency;
        oscState->index &= 0x3ffff;

        /*
        switch(oscillator->shape) {
        case OSC_SHAPE_SIN:
            return  sinTable[index >> 7]; // * ((1024 + this->matrix->getDestination(destAmp)) >> 10) ;
            break;
        case OSC_SHAPE_SIN2:
        {
            int s = sinTable[index >> 7];
            return   (s*s) >> 16;
            break;
        }
        case OSC_SHAPE_SIN3:
            return  sinTable[index >> 7] * (index<0x1ffff); // * ((1024 + this->matrix->getDestination(destAmp)) >> 10) ;
            break;
        case OSC_SHAPE_SIN4:
            index &= 0x1ffff;
            return  sinTable[index >> 7];
            break;
        case OSC_SHAPE_RAND:
            return  (randomOsc * index) & 0x7fff;
            break;
        }
        return index;
*/
        asm volatile(
                "    tbb [pc, %[shape]]\n\t"
                "7:\n\t"
                "    .byte   (1f-7b)/2\n\t"
                "    .byte   (2f-7b)/2\n\t"
                "    .byte   (3f-7b)/2\n\t"
                "    .byte   (4f-7b)/2\n\t"
                "    .byte   (5f-7b)/2\n\t"
                "    .align  1\n\t"

                // OSC_SHAPE_SIN
                "1: \n\t"
                "    lsr %[index], #7\n\t"
                "    ldrsh %[value], [ %[sinTable], %[index], lsl #1]\n\t"
                "    b 6f\n\t"
                // OSC_SHAPE_SIN2
                "2:  \n\t"
                "    lsr %[index], #7\n\t"
                "    ldrsh %[value], [ %[sinTable], %[index], lsl #1]\n\t"
                "    mul %[value], %[value]\n\t"
                "    lsr %[value], #16\n\t"
                "    b 6f\n\t"

                // OSC_SHAPE_SIN3
                "3:  \n\t"
                "    lsr %[index], #7\n\t"
                "    ldrsh %[value], [ %[sinTable], %[index], lsl #1]\n\t"
                // if negativ replace by 0
                "    cmp  %[value], #0\n\t"
                "    it lt\n\t"
                "    movlt %[value], #0\n\t"
                "    b 6f\n\t"

                // OSC_SHAPE_SIN4
                "4:  \n\t"
                "    lsr %[index], #7\n\t"
                "    ldrsh %[value], [ %[sinTable], %[index], lsl #1]\n\t"
                // if negativ take the oposite
                "    cmp  %[value], #0\n\t"
                "    it mi\n\t"
                "    rsbmi %[value], %[value], #0\n\t"
                "    b 6f\n\t"

                // OSC_SHAPE_RAND
                "5:  \n\t"
                "    mov %[value], %[randomOsc]\n\t"

                // BREAK
                "6:\n\t"
                : [value] "=r"(oscValue)
                : [sinTable]"rV"(sinTable), [shape]"r" (oscillator->shape), [randomOsc]"r"(randomOsc), [index]"r" (oscState->index)
        );

        return  oscValue;

    };


    static void updateRandomNumber() __attribute__((always_inline)) {
        randomOsc = random(65535) - 32768;
    }
    OscillatorParams* oscillator;

private:
    DestinationEnum destFreq;
    DestinationEnum destAmp;
    Matrix* matrix;

    // Random number
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
    }
    OscillatorParams * o = (OscillatorParams *)(&(synthState.params.osc1));
    oscillator = &o[number-1];
}

template <int number>
Osc<number>::~Osc()
{
}

template <int number>
void Osc<number>::init(Matrix* matrix) {
    this->matrix = matrix;
}

template <int number>
void Osc<number>::newNote(struct OscState& oscState, int note) {
    oscState.index = 1; // << number;
    switch (oscillator->frequencyType) {
    case OSC_FT_KEYBOARD:
        oscState.mainFrequency = ((frequenciesx8[note] * oscillator->frequencyMul) >> 4) + oscillator->detune;
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
        oscState.nextFrequency = ((frequenciesx8[note] * oscillator->frequencyMul) >> 4) + oscillator->detune;
        break;
    case OSC_FT_FIXE:
        oscState.nextFrequency = ((oscillator->frequencyMul << 7) + oscillator->detune)<<2;
        break;
    }
    oscState.fromFrequency = oscState.mainFrequency;
}


template <int number>
void Osc<number>::glideStep(struct OscState& oscState, int step) {
    oscState.mainFrequency = ((oscState.fromFrequency * ((1<<synthState.params.engine1.glide) - step)) + oscState.nextFrequency * step) >> synthState.params.engine1.glide;
    oscState.frequency = oscState.mainFrequency;
}
