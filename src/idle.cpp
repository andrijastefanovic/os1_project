/*
 * idle.cpp
 *
 *  Created on: Aug 4, 2021
 *      Author: OS1
 */

#include "idle.h"
#include "PCB.h"
#include "kernel.h"
#include <iostream.h>

void Idle::run(){

	while(1){

	}
}


Idle::Idle(StackSize stackSize, Time timeSlice): Thread(stackSize, timeSlice){

}
PCB* Idle::getMyPCB(){return myPCB;}

