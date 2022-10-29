#include "Thread.h"
#include "PCB.h"
#include "kernel.h"

extern int syncPrintf(const char *format, ...);

Thread::Thread(StackSize stackSize, Time timeSlice){
	myPCB=new PCB(stackSize, timeSlice);
	myPCB->myThread=this;
}
void Thread::waitToComplete(){
	if(this->myPCB->done==0){
	Kernel::lockFlag++;
	PCB::running->waitingList.add(this->myPCB);
	this->myPCB->notifyList.add(PCB::running);
	this->myPCB->notifySize++;
	PCB::running->waitingSize++;
	PCB::running->suspended=1;
	Kernel::lockFlag--;
	dispatch();}
}
Thread * Thread::getThreadById(ID id){
	BI_ListIteratorImp<PCB*> iter(PCB::allPCB);

	for(int i=0;i<PCB::allPCBNum;i++){
		if(iter.current()->id==id) return iter.current()->myThread;
		else iter++;
	}
	return 0;
};
ID Thread::getId(){
	return myPCB->id;
}
ID Thread::getRunningId(){
	return PCB::running->id;
}
void dispatch(){
	Kernel::dispatch();
}
void Thread::start(){
	Scheduler::put(myPCB);
}
Thread::~Thread(){

}
