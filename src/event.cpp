/*
 * event.cpp
 *
 *  Created on: Aug 17, 2021
 *      Author: OS1
 */

#include "event.h"
#include "kernelEv.h"


Event::Event(IVTNo ivtNo){
	myImpl=new KernelEv(ivtNo);
	myImpl->myEvent=this;
}
void Event::wait(){

	myImpl->wait();

}
Event::~Event(){
	delete myImpl;
myImpl=0;

}
void Event::signal(){



}
