/*
 * kernelEv.h
 *
 *  Created on: Aug 17, 2021
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_
#include "event.h"
#include "kernel.h"
class IVTEntry;

class KernelEv {
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();
	void wait();
	void signal();
	int value;
	Event* myEvent;
	int block;
	PCB* myPCB;


};


#endif /* KERNELEV_H_ */
