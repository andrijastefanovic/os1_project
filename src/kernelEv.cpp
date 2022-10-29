/*
 * kernelEv.cpp
 *
 *  Created on: Aug 17, 2021
 *      Author: OS1
 */

#include "kernelEv.h"
#include "IVTEntry.h"
/*#define lock { asm{pushf; cli;}}
#define unlock {asm{popf;}}
*/


KernelEv::KernelEv(IVTNo ivtNo){
	lock();
	block=0;
	myPCB=PCB::running;
	IVTEntry::ivtArray[ivtNo]->myKernelEv=this;
	value=0;
	unlock();



}
void KernelEv::wait(){
	lock();


	if(PCB::running!=myPCB){unlock();return;}
	if(value==1){value=0;unlock(); return;}
	if(value==0){PCB::running->suspended=1; block=1; unlock(); dispatch();}

}
void KernelEv::signal(){

	lock();

	if(!block){unlock(); value=1;}
	else{
		value=0;

		block=0;

		if(myPCB->waitingSize==0) {myPCB->suspended=0;Scheduler::put(myPCB);unlock();dispatch();}
		else unlock();

	}



}

KernelEv::~KernelEv(){
	this->myPCB=0;
	this->myEvent=0;
}
