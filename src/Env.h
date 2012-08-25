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


enum EnvState {
    ENV_STATE_ON_A = 0,
    ENV_STATE_ON_D,
    ENV_STATE_ON_S,
    ENV_STATE_ON_R,
    ENV_STATE_QUICK_OFF_USELESSNOW__,
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


class Env : public SynthStateAware
{
public:
    Env() {
    }

    virtual ~Env(void) {
    }

    void setSynthState(SynthState* sState);
    void init(int number, Matrix* matrix);

    void reloadADSR() {

        adsr[0] = ((envParams->attack * envParams->attack) >> 5);
        adsr[1] = ((envParams->decay * envParams->decay) >> 5 );
        adsr[2] = envParams->sustain << 7;
        adsr[3] = ((envParams->release * envParams->release) >> 5) + 1;

        if (adsr[1] != 0) {
            incD = ((32767 - adsr[2]) << 15) / adsr[1];
        } else {
            incD = ((32767 - adsr[2]) << 15);
        }

    }


    void noteOn(struct EnvData& env) {
        int attack = adsr[0] + ((this->matrix->getDestination(destAttack) + this->matrix->getDestination(ALL_ENV_ATTACK)) >> 5);

        if (attack > 2033) {
            attack = 2033;
        } else if (attack< 0) {
            attack = 0;
        }

        if (attack != 0) {
            incA = (32767 << 15) / attack;
            env.currentAmp= 0;
        } else {
            incA = (32767 << 15);
            env.currentAmp= incA;
        }

        env.currentAmpSpeed = incA;
        // index is decremented in the first call...
        env.index = attack + 1;
        env.envState = ENV_STATE_ON_A;
    }

    void noteOffQuick(struct EnvData* env) {
        env->index = 5;
        env->envState = ENV_STATE_ON_R;
        env->currentAmpSpeed = env->currentAmp  >> 2;
    }

    void noteOff(struct EnvData* env) {
        env->index = adsr[3] + 1;
        env->envState = ENV_STATE_ON_R;
        env->currentAmpSpeed = (env->currentAmp) / adsr[3];

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
                // mv 32767 << 15 in r6 (currentAmp).
                "    mvn r6, #0\n\t"
                "    lsr r6, r6, #2\n\t"
                // change state to DECAY
                "    mov r7, #1\n\t"
                "    mov r8, %[incD]\n\t"
                // store envState, currentAmpSpeed
                "    str r7, [%[env], #8]\n\t"
                "    str r8, [%[env], #12]\n\t"

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
    Matrix* matrix;
    DestinationEnum destAttack;
};

