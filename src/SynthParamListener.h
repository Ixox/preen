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


#ifndef SYNTHPARAMLISTENER_H_
#define SYNTHPARAMLISTENER_H_

enum SynthParamType {
    SYNTH_PARAM_TYPE_ENGINE = 0,
    SYNTH_PARAM_TYPE_OSC,
    SYNTH_PARAM_TYPE_ENV,
    SYNTH_PARAM_TYPE_MATRIX,
    SYNTH_PARAM_TYPE_LFO,
    SYNTH_PARAM_TYPE_INVALID
};

enum ParameterDisplayType {
    DISPLAY_TYPE_NONE = 0,
    DISPLAY_TYPE_SIGNED_CHAR,
    DISPLAY_TYPE_UNSIGNED_CHAR,
    DISPLAY_TYPE_STRINGS,
    DISPLAY_TYPE_OSC_FREQUENCY,
    DISPLAY_TYPE_FLOAT_5_3,
    DISPLAY_TYPE_FLOAT_4_4,
    DISPLAY_TYPE_FLOAT_1_7,
    DISPLAY_TYPE_VOICES
};

struct ParameterDisplay {
    char minValue;
    unsigned char maxValue;
    ParameterDisplayType displayType;
    const char** valueName;
};


class SynthParamListener {
public:
    virtual void newParamValue(SynthParamType type, int currentrow, int encoder, ParameterDisplay* param, int oldValue, int newValue) = 0;
    virtual void newParamValueFromExternal(SynthParamType type, int currentrow, int encoder, ParameterDisplay* param, int oldValue, int newValue) = 0;
    virtual void newcurrentRow(int newcurrentRow) = 0;

    virtual void beforeNewParamsLoad() = 0;

    SynthParamListener* nextListener;
};

#endif /* SYNTHPARAMLISTENER_H_ */
