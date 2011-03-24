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


#ifndef MATRIX_H_
#define MATRIX_H_

#define MATRIX_SIZE 6

#include "SynthState.h"
#include "SynthParamListener.h"




class Matrix  {
public:
	Matrix();
	~Matrix();
	void reinitUsage();
	void reinitUsage(int k, int oldValue, int value);

	void resetAllDestination() {
		for (int k=0; k< DESTINATION_MAX; k++) {
			currentDestinations[k] = 0;
		}
	}

	void resetUsedDestination() {
		for (int k=0; k< MATRIX_SIZE; k++) {
			currentDestinations[(int)rows[k].destination] = 0;
		}
	}

	void computeFutureDestintation(int k) {
		futurDestinations[(int)rows[k].destination] += sources[(int)rows[k].source] * rows[k].mul;
	}

	void setRowSource(int index, SourceEnum source);
	void setRowDestination(int index, DestinationEnum destination) ;
	void setRowMul(int index, short mul);
	void checkRow(int index);

	void setSource(SourceEnum source, int value) {
		this->sources[source] = value;
	}
	int getDestination(DestinationEnum destination) {
		return this->currentDestinations[destination];
	}

    void newParamValue(int param, int oldValue, int newValue);;

    void useNewValues() {
    	if (currentDestinations == destinations1) {
    		currentDestinations = destinations2;
    		futurDestinations = destinations1;
    	} else {
    		currentDestinations = destinations1;
    		futurDestinations = destinations2;
    	}
    }

private:
	bool sourceUsed[SOURCE_MAX];
	bool destinationUsed[DESTINATION_MAX];
	int sources[SOURCE_MAX];
	int destinations1[DESTINATION_MAX];
	int destinations2[DESTINATION_MAX];
	int *currentDestinations;
	int *futurDestinations;
	MatrixRowParams* rows;
};

#endif /* MATRIX_H_ */
