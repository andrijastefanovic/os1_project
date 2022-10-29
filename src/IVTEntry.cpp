/*
 * IVTEntry.cpp
 *
 *  Created on: Aug 17, 2021
 *      Author: OS1
 */

#include "IVTEntry.h"

IVTEntry* IVTEntry::ivtArray[256]={0};

IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt inter){
lock();
	this->ivtNo=ivtNo;


	callOld=getvect(ivtNo);
	setvect(ivtNo, inter);
	ivtArray[ivtNo]=this;
unlock();

}
void IVTEntry::signal(){
if(myKernelEv!=0)	myKernelEv->signal();
}
IVTEntry::~IVTEntry(){
//	callOld();
	lock();
	setvect(ivtNo,callOld);
	ivtArray[ivtNo]=0;
	this->callOldRoutine();
unlock();

}
