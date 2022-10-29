/*
 * PCB.h
 *
 *  Created on: Aug 1, 2021
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_
#include "Thread.h"
#include "listimp.h"

class Thread;
class PCB {
public:
	static ID IDG;
	PCB(StackSize stackSize, Time timeSlice);
	virtual ~PCB();
	Thread* myThread;
	int blockTime;
	int semReturn;
	static PCB* running;
	volatile unsigned ss;
	volatile unsigned sp;
	volatile unsigned bp;
	BI_ListImp<PCB*> notifyList;
	BI_ListImp<PCB*> waitingList;
	static BI_ListImp<PCB*> allPCB;
	static int allPCBNum;
	int waitingSize;
	int notifySize;
//	int blocked;
	friend class Idle;
	unsigned* stack;
	unsigned done;
	unsigned suspended;
	Time timeSlice;
	Time timeRemaining;
	ID id;
	static void notify();
	static void wrapper();
	void notified(PCB* notifier);
	friend class Thread;
};

#endif /* PCB_H_ */
