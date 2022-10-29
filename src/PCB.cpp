/*
 * PCB.cpp
 *
 *  Created on: Aug 1, 2021
 *      Author: OS1
 */

#include "PCB.h"
#include "kernel.h"
int PCB::allPCBNum=0;
BI_ListImp<PCB*> PCB::allPCB;
ID PCB::IDG=0;
PCB* PCB::running=0;
extern int syncPrintf(const char *format, ...);

PCB::PCB(StackSize stackSize, Time timeSlice) {
	lock();
	blockTime=0;
	waitingSize=0;
	semReturn=1;
	notifySize=0;
	id=IDG;
	IDG++;
	done=0;
	suspended=0;
	stack=new unsigned[stackSize/sizeof(unsigned)];
		stack[stackSize/sizeof(unsigned)-1]=0x200;
		stack[stackSize/sizeof(unsigned)-2]=FP_SEG(PCB::wrapper);
		stack[stackSize/sizeof(unsigned)-3]=FP_OFF(PCB::wrapper);
		this->ss = FP_SEG(stack + stackSize/sizeof(unsigned) - 12);
		this->sp = this->bp = FP_OFF(stack + stackSize/sizeof(unsigned) - 12);

		this->timeSlice=timeSlice;
		PCB::allPCB.add(this);
		PCB::allPCBNum++;

		unlock();
	//	Scheduler::put(this);


}

PCB::~PCB() {

}
void PCB::wrapper(){
	PCB::running->myThread->run();

	PCB::running->done=1;

	if(!PCB::running->notifyList.isEmpty()) PCB::running->notify();
	dispatch();
}
void PCB::notify(){


	for(int i=0; i<PCB::running->notifySize;i++){
		PCB::running->notifyList.peekHead()->notified(PCB::running);
		PCB::running->notifyList.detach(PCB::running->notifyList.peekHead(),1);

	}
	PCB::running->notifySize=0;
	PCB::running->notifyList.flush();


}
void PCB::notified(PCB* notifier){
	/*asm cli;
	cout<<"NOTIFIED "<<this->id <<endl;
	asm cli; */
	BI_ListIteratorImp<PCB*> iter(this->waitingList);
for(int i=0; i<this->waitingSize;i++){
	if(iter.current()->id==notifier->id) {this->waitingList.detach(iter.current(),1); break;}
iter++;
}
waitingSize--;

if(waitingSize==0){this->suspended=0; Scheduler::put(this);}

}

