/*
 * RingBuffer.h
 *
 *  Created on: Feb 13, 2011
 *      Author: xhosxe
 */

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#ifdef linux
#include "stdlib.h"
typedef unsigned short uint16;
#else
#include "libmaple.h"
#endif

template <typename T, int size>
class RingBuffer {
public:
	RingBuffer() {
	    this->head = 0;
	    this->tail = 0;
	}
	~RingBuffer() {
	};

	void insert(T element) {
	    this->buf[this->tail] = element;
	    this->tail = (this->tail == size-1) ? 0 : this->tail + 1;
	}

	T remove() {
	    T element = this->buf[this->head];
	    this->head = (this->head == size-1) ? 0 : this->head + 1;
	    return element;
	}

	T getOneAfter() {
		if (getCount()<=1) {
			return NULL;
		} else {
			return this->buf[(this->head == size-1) ? 0 : this->head + 1];
		}
	}

	bool isFull() {
	   return (this->tail + 1 == this->head) ||
	        (this->tail == (size-1) && this->head == 0);
	}

	int getCount() {
	    int count = this->tail - this->head;
	    if (this->tail < this->head) {
	    	count += size;
	    }
	    return count;
	}


private:
    volatile int head;
    volatile int tail;
    volatile T buf[size+1];

};

#endif /* RINGBUFFER_H_ */
