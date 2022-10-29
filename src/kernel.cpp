/*
 * kernel.cpp
 *
 *  Created on: Aug 3, 2021
 *      Author: OS1
 */
#include "kernel.h"


extern void tick();
Idle* Kernel::idle=new Idle(1024, 1);
PCB* Kernel::mainPCB=0;
volatile int Kernel::lockFlag=0;
volatile int Kernel::dispatchCall=0;
volatile int Kernel::thread_switch_req=0;
pInterrupt Kernel::oldInt=0;
MainThread* Kernel::mainThr=0;
BI_ListImp<KernelSem*> Kernel::semList;
int Kernel::semNumber=0;
void lock(){Kernel::lockFlag++;}
void unlock(){
	if(Kernel::lockFlag>0) Kernel::lockFlag--;
	if(Kernel::thread_switch_req==1&&Kernel::lockFlag==0) dispatch();
}
void callOldRoutine(){
	(*(Kernel::oldInt))();
}

Kernel::Kernel(int argc, char* argv[]) {


this->start();
PCB* p=new PCB(4096,2);
PCB::running=mainPCB=p;
mainThr=new MainThread(4096,2,argc,argv);


}


void setVect(int IVTNo, pInterrupt newInt){
	setvect(IVTNo, newInt);
}
void Kernel::start(){
#ifndef BCC_BLOCK_IGNORE
	oldInt= getvect(0x8);
	setVect(0x8, (pInterrupt)Kernel::timer);
#endif

}


Kernel::~Kernel() {
setVect(0x8, oldInt);
delete Kernel::idle;
delete Kernel::mainThr;
delete Kernel::mainPCB;
}
void ksemTimer(){

	if(!Kernel::semList.isEmpty()){
		BI_ListIteratorImp<KernelSem*> iter1(Kernel::semList);

		int limit1=Kernel::semNumber;



		for(int i=0;i<limit1;i++){
			if(!iter1.current()->blockedLimited.isEmpty()){
			BI_ListIteratorImp<PCB*> iter2(iter1.current()->blockedLimited);
			int limit2=iter1.current()->blockedLimitedNumber;
			for(int j=0;j<limit2;j++){
				iter2.current()->blockTime--;
				if(iter2.current()->blockTime==0){
					if(iter2.current()->waitingSize==0) Scheduler::put(iter2.current());
					else cout<<"ERROR"<<endl;
					PCB* toDetach=iter2.current();
					iter2.current()->suspended=0;
					iter2++;
					iter1.current()->blockedLimited.detach(toDetach,1);
					toDetach->semReturn=0;
					iter1.current()->blockedLimitedNumber--;
					iter1.current()->value++;
				}
				else iter2++;}
			}
			iter1++;
		}
		}

return;}




void interrupt Kernel::timer(...){
static volatile unsigned tsp;
static volatile unsigned tss;
static volatile unsigned tbp;

if(!Kernel::dispatchCall){
	tick();
    ksemTimer();
	callOldRoutine();
}
if(Kernel::dispatchCall==0 && PCB::running->timeSlice!=0) PCB::running->timeRemaining--;
if(PCB::running->done) PCB::running->timeRemaining=0;

if((PCB::running->timeRemaining <= 0 && PCB::running->timeSlice!=0) || Kernel::thread_switch_req|| PCB::running->done) {
	if (Kernel::lockFlag==0){
    	lock();
    	Kernel::thread_switch_req = 0;
#ifndef BCC_BLOCK_IGNORE
    	asm {
    	    	mov tsp, sp
    			mov tss, ss
    			mov tbp, bp
    	    }
#endif
       PCB::running->sp=tsp;
       PCB::running->ss=tss;
       PCB::running->bp=tbp;
       if(Thread::getRunningId()!=Kernel::idle->getId() && PCB::running->suspended==0 && PCB::running->done==0){ Scheduler::put(PCB::running);}
       PCB::running=Scheduler::get();
       if(PCB::running==0&&Kernel::mainThr->getPCB()->done==0) PCB::running=Kernel::idle->getMyPCB();
       if(PCB::running==0&&Kernel::mainThr->getPCB()->done==1) PCB::running=Kernel::mainPCB;
       tsp=PCB::running->sp;
       tss=PCB::running->ss;
       tbp=PCB::running->bp;
       PCB::running->timeRemaining = PCB::running->timeSlice;
#ifndef BCC_BLOCK_IGNORE
    asm {
    	mov sp, tsp
		mov ss, tss
		mov bp, tbp
    }
#endif
       Kernel::dispatchCall=0;
       unlock();
	}
    else{ Kernel::thread_switch_req = 1; Kernel::dispatchCall=0; }
}

}
void Kernel::dispatch() {
	Kernel::dispatchCall=1;
	Kernel::thread_switch_req=1;
    asm int 0x8;
}
