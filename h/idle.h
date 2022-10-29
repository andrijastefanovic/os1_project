/*
 * idle.h
 *
 *  Created on: Aug 4, 2021
 *      Author: OS1
 */

#ifndef IDLE_H_
#define IDLE_H_

#include "Thread.h"

class Idle: public Thread {
public:
	Idle(StackSize stackSize = defaultStackSize, Time timeSlice =
			defaultTimeSlice);
	virtual void run();
	PCB* getMyPCB();


};

#endif /* IDLE_H_ */
