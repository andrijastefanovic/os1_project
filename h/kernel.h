/*
 * kernel.h
 *
 *  Created on: Aug 3, 2021
 *      Author: OS1
 */

#ifndef KERNEL_H_
#define KERNEL_H_


#include "Thread.h"
#include "idle.h"
#include "mainThr.h"
#include "PCB.h"
#include "ksem.h"
#include "SCHEDULE.H"
#include <dos.h>
#include <iostream.h>
typedef void interrupt (*pInterrupt)(...);

class PCB;
class Kernel {
public:
	static PCB* mainPCB;
	static pInterrupt oldInt;
	static int semNumber;
	void start();
	static BI_ListImp<KernelSem*> semList;
	static Idle* idle;
    static MainThread* mainThr;
	static volatile int lockFlag;
	static volatile int dispatchCall;
	static volatile int thread_switch_req;
	static void dispatch();
	Kernel(int argc, char* argv[]);
	virtual ~Kernel();
	static void interrupt timer(...);
	friend class BI_ListImp;


};
void lock();
void unlock();


#endif /* KERNEL_H_ */
