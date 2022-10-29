/*
 * ksem.cpp
 *
 *  Created on: Aug 11, 2021
 *      Author: OS1
 */

#include "ksem.h"
#define lock() { Kernel::lockFlag++;}
#define unlock() {Kernel::lockFlag--;}
int KernelSem::ID=0;

KernelSem::KernelSem(int init){
    first=1;
	value=init;

	blockedLimitedNumber=0;
	id=KernelSem::ID++;


}

int KernelSem::wait(Time maxTimeToWait){
	lock();





	if(first){ Kernel::semList.add(this); Kernel::semNumber++;first=0;}



	value--;
	if(value>=0){PCB::running->semReturn=1;unlock();}
	else{



		PCB::running->blockTime=maxTimeToWait;
		PCB::running->suspended=1;
		blockedLimited.add(PCB::running);
		blockedLimitedNumber++;



		unlock();

		dispatch();

	}

	return PCB::running->semReturn;

}
int KernelSem::wait(){
	lock();
	if(first){Kernel::semList.add(this); Kernel::semNumber++;first=0;}

//	cout<<"WAITING"<<endl;
	value--;


	if(value>=0){PCB::running->semReturn=1;unlock();}
	else{


		PCB::running->suspended=1;
		blockedUnlimited.add(PCB::running);

		unlock();
		dispatch();
	}


	return PCB::running->semReturn;


}

void KernelSem::signal(){
lock();
if (PCB::running->semReturn==1){

value++;


}

if(first){ Kernel::semList.add(this); Kernel::semNumber++;first=0;}



if(!blockedUnlimited.isEmpty()){



blockedUnlimited.peekHead()->suspended=0;
blockedUnlimited.peekHead()->semReturn=1;
if(blockedUnlimited.peekHead()->waitingSize==0){Scheduler::put(blockedUnlimited.peekHead());}
PCB* toDetach=blockedUnlimited.peekHead();
blockedUnlimited.detach(toDetach,1);

}
else{
	if(!blockedLimited.isEmpty()){





		blockedLimited.peekHead()->suspended=0;
		blockedLimited.peekHead()->semReturn=1;
		if(blockedLimited.peekHead()->waitingSize==0){Scheduler::put(blockedLimited.peekHead());}


        PCB* toDetach=blockedLimited.peekHead();


        blockedLimited.peekHead()->blockTime=0;
		blockedLimited.detach(toDetach,1);
		blockedLimitedNumber--;


	}
}

unlock();
}

