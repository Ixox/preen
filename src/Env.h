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

#include "SynthState.h"


enum EnvState {
    ENV_STATE_ON_A = 0,
    ENV_STATE_ON_D,
    ENV_STATE_ON_S,
    ENV_STATE_ON_R,
    ENV_STATE_QUICK_OFF,
    ENV_STATE_DEAD
};

struct EnvData {
    // life cycle of the env
    unsigned int index;
    // Current sample
    unsigned int currentAmp;
    // State of the env
    unsigned int envState;
    // Release speed and acceleration (dependent on the voice when RELEASE)
    unsigned int currentAmpSpeed;

    unsigned int currentAmpAcc;
};


template <int number>
class Env
{
public:
    Env() {
    }
    ~Env(void) {
    }

    void loadADSR() {
        EnvelopeParams * e = (EnvelopeParams *)&(synthState.params.env1);
        EnvelopeParams* envState = &e[number-1];

        adsr[0] = envState->attack * envState->attack +1;
        adsr[1] = envState->decay * envState->decay+1;
        adsr[2] = envState->sustain << 7;
        adsr[3] = envState->release * envState->release+1;

        /*
        if (adsr[0]>=640000) {
            incA = (32767<<15) / adsr[0];
            incIncA = incA / ((adsr[0]+1)/2);
            incA = incA + ((adsr[0]+1)/2)*incIncA;
        } else {
         */
        incA = (32767<<15) / adsr[0];
        incIncA = 0;
        //        }

        /*
        if (adsr[1]>=640000) {
            incD = ((32768 - adsr[2]) << 15) / adsr[1];
            incIncD = incD / ((adsr[1]+1)/2);
            incD = incD + ((adsr[1]+1)/2)*incIncD;
        } else {
         */
        incD = ((32767 - adsr[2]) << 15) / adsr[1];
        incIncD = 0;
        //        }
    }



    void noteOn(struct EnvData& env) {
        env.currentAmp= 0;
        env.currentAmpSpeed = incA;
        env.currentAmpAcc = incIncA;
        env.index = adsr[0] + 1;
        env.envState = ENV_STATE_ON_A;
    }

    void noteOffQuick(struct EnvData* env) {
        // assembly to update all env value at the same time...
        // Because it happens outside of main synth thread
        asm volatile(
                // r5 : index, r6 : currentAmp, r7 : envState, r8 : currentAmpSpeed
                "    ldm %[env], {r5-r8}\n\t"
                // env->index = 65;
                "    mov r5, #65\n\t"
                // env->envState = ENV_STATE_QUICK_OFF;
                "    mov r7, #4\n\t"
                // env->currentAmpSpeed = env->currentAmp >> 6;
                "    mov r8, r6, lsr #6\n\t"
                // Store all values
                "    stm %[env], {r5-r8}\n\t"
                : [env]"+rV" (env)
                  :
                  : "r5", "r6", "r7", "r8", "cc");
    }


    void noteOff(struct EnvData* env) {
        // assembly to update all env value at the same time...
        // Because it happens outside of main synth thread
        asm volatile(
                // r5 : index, r6 : currentAmp, r7 : envState, r8 : currentAmpSpeed
                "    ldm %[env], {r5-r8}\n\t"
                // env->index = adsr[3] +1;
                "    add r5, %[release], #1\n\t"
                // env->envState = ENV_STATE_ON_R;
                "    mov r7, #3\n\t"
                // env->currentAmpSpeed = (env->currentAmp) / adsr[3];
                "    udiv r8, r6, %[release]\n\t"
                // Store all values
                "    stm %[env], {r5-r8}\n\t"
                : [env]"+rV" (env)
                : [release]"r"(adsr[3])
                : "r5", "r6", "r7", "r8", "cc");

    }

    inline int getNextAmp(struct EnvData* env)  __attribute__((always_inline))  {

        asm volatile(
                // r5 : index, r6 : currentAmp, r7 : envState, r8 : currentAmpSpeed
                "    ldm %[env], {r5-r8}\n\t"
                // index --
                "    sub r5, #1\n\t"
                // switch
                "    tbb [pc, r7]\n\t"
                "7:\n\t"
                "    .byte   (1f-7b)/2\n\t"
                "    .byte   (2f-7b)/2\n\t"
                "    .byte   (3f-7b)/2\n\t"
                "    .byte   (4f-7b)/2\n\t"
                "    .byte   (5f-7b)/2\n\t"
                "    .byte   (6f-7b)/2\n\t"
                "    .align  1\n\t"

                // attack
                "1:  cbz r5, 11f\n\t"
                "    add R6, r8\n\t"
                // store index and currentAmp
                "    stm %[env], {r5, r6}\n\t"
                "    b 6f\n\t"

                "11: ldr r5, [%[adsr], #4]\n\t"
                "    mov r7, #1\n\t"
                "    mov r8, %[incD]\n\t"
                // store index, currentAmp
                "    stm %[env], {r5-r8}\n\t"

                // decay
                "2:  cbz r5, 21f\n\t"
                "    sub r6, r8\n\t"
                // store index and currentAmp
                "    stm %[env], {r5,r6}\n\t"
                "    b 6f\n\t"
                "21: mov r7, #2\n\t"
                "    str r7, [%[env], #8]\n\t"

                // sustain
                "3:  ldr r6, [%[adsr], #8]\n\t"
                "    lsl r6, #15\n\t"
                "    str r6, [%[env], #4]\n\t"
                "    b 6f\n\t"

                // release & Quick Off
                "4: \n\t"
                "5: \n\t"
                "    cbz r5, 41f\n\t"
                "    sub r6, r8\n\t"
                // store index and currentAmp
                "    stm %[env], {r5,r6}\n\t"
                "    b 6f\n\t"
                "41: mov r7, #5\n\t"
                "    mov r6, #0\n\t"
                "    str r7, [%[env], #8]\n\t"
                "    str r6, [%[env], #4]\n\t"
                "    b 6f\n\t"

                // env.envState = ENV_STATE_DEAD;
                "6:\n\t"
                : [env]"+rV" (env)
                : [adsr]"rV" (adsr), [incD]"r"(incD)
                : "r5", "r6", "r7", "r8", "cc");

        return env->currentAmp>>15;
    }

    bool isDead(struct EnvData& env)  __attribute__((always_inline))  {
        return env.envState == ENV_STATE_DEAD;
    }

private:
    // ADSR
    unsigned int adsr[4];
    // Ramp speed and acceleration if attack and Decay
    unsigned int incA, incD, incIncA, incIncD;

};

