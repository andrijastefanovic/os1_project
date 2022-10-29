/*
 * mainThread.h
 *
 *  Created on: Aug 4, 2021
 *      Author: OS1
 */

#ifndef MAINTHR_H_
#define MAINTHR_H_

#include "Thread.h"
#include "PCB.h"

class MainThread: public Thread {
public:
	MainThread(StackSize stackSize = defaultStackSize, Time timeSlice =
defaultTimeSlice, int argc, char* argv[]);
	int argc;
	char** argv;
	virtual void run();
	PCB* getPCB(){return myPCB;}
};

#endif /* MAINTHR_H_ */
