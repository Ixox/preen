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

enum SynthParamListenerType {
	SYNTH_PARAM_ENGINE_LISTENER = 0,
	SYNTH_PARAM_OSCILLATOR_LISTENER,
	SYNTH_PARAM_ENVELOPE_LISTENER,
	SYNTH_PARAM_MATRIX_LISTENER,
	SYNTH_PARAM_LFO_LISTENER,
	SYNTH_PARAM_INVALID_LISTENER
};

enum ParameterDisplayType {
	DISPLAY_TYPE_NONE = 0,
	DISPLAY_TYPE_SIGNED_CHAR,
	DISPLAY_TYPE_UNSIGNED_CHAR,
	DISPLAY_TYPE_STRINGS,
	DISPLAY_TYPE_OSC_FREQUENCY,
	DISPLAY_TYPE_FLOAT_5_3,
	DISPLAY_TYPE_FLOAT_4_4
};

struct ParameterDisplay {
	char minValue;
	unsigned char maxValue;
	ParameterDisplayType displayType;
	const char** valueName;
};


class SynthParamListener {
public:
    virtual void newParamValue(SynthParamListenerType type, int currentrow, int encoder, ParameterDisplay* param, int oldValue, int newValue) = 0;
    virtual void newParamValueFromExternal(SynthParamListenerType type, int currentrow, int encoder, ParameterDisplay* param, int oldValue, int newValue) = 0;
    virtual void newcurrentRow(int newcurrentRow) = 0;

	SynthParamListener* nextListener;
};

#endif /* SYNTHPARAMLISTENER_H_ */
