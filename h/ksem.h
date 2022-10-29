/*
 * ksem.h
 *
 *  Created on: Aug 11, 2021
 *      Author: OS1
 */

#ifndef KSEM_H_
#define KSEM_H_

#include "semaphor.h"
#include "kernel.h"

class KernelSem {
public:
	static int ID;
	int first;
	int id;
	BI_ListImp<PCB*> blockedUnlimited;
	BI_ListImp<PCB*> blockedLimited;
	KernelSem(int init);
	volatile int blockedLimitedNumber;
	int wait(Time maxTimeToWait);
	int wait();


	void signal();
	volatile int value;
	friend class Semaphore;
};

#endif /* KSEM_H_ */
