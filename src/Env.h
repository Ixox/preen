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
    // Release speed (dependent on the voice when RELEASE)
    unsigned int currentAmpSpeed;
};


template <int number>
class Env : public SynthStateAware
{
public:
    Env() {
    }
    ~Env(void) {
    }

	void setSynthState(SynthState* sState) {
		SynthStateAware::setSynthState(sState);
	    EnvelopeParams * e = (EnvelopeParams *)&(this->synthState->params.env1);
	    envParams = &e[number-1];
	    reloadADSR();
	}


    void reloadADSR() {

        adsr[0] = envParams->attack * envParams->attack +1;
        adsr[1] = envParams->decay * envParams->decay+1;
        adsr[2] = envParams->sustain << 7;
        adsr[3] = envParams->release * envParams->release+1;

        incA = (32767<<15) / adsr[0];
        incD = ((32767 - adsr[2]) << 15) / adsr[1];
    }



    void noteOn(struct EnvData& env) {
        env.currentAmp= 0;
        env.currentAmpSpeed = incA;
        // index is decremented
        env.index = adsr[0] + 1;
        env.envState = ENV_STATE_ON_A;
    }

    void noteOffQuick(struct EnvData* env) {
        // assembly to update all env value at the same time...
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
        		// Every other time : return - go directly to 6 (end)
//        		"    tst r5, #1\n\t"
 //       		"    itt ne"
   //     		"    cbz r5, 6f\n\t"

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
    // Ramp speed Of attack and Decay
    unsigned int incA, incD;
    EnvelopeParams* envParams;
};

