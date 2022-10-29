/*
 * semaphor.cpp
 *
 *  Created on: Aug 11, 2021
 *      Author: OS1
 */

#include "semaphor.h"
#include "ksem.h"


Semaphore::Semaphore(int init){


	myImpl=new KernelSem(init);


}
int Semaphore::wait(Time maxTimeToWait){


	int i;
	if(maxTimeToWait!=0)   i=myImpl->wait(maxTimeToWait);
	else i= myImpl->wait();

	return i;
}

void Semaphore::signal(){
	myImpl->signal();
}

int Semaphore::val() const{
	return myImpl->value;
}

Semaphore::~Semaphore(){
	delete myImpl;
}
