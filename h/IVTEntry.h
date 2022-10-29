/*
 * IVTEntry.h
 *
 *  Created on: Aug 17, 2021
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_
#include "kernelEv.h"



class IVTEntry {
public:
	static IVTEntry* ivtArray[256];
	IVTEntry(IVTNo ivtNo, pInterrupt inter);
	IVTNo ivtNo;
	pInterrupt callOld;
	void callOldRoutine(){
		(*callOld)();
	}
	~IVTEntry();
	void signal();
	KernelEv* myKernelEv;


};



#endif /* IVTENTRY_H_ */
