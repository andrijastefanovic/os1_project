/*
 * mainThread.cpp
 *
 *  Created on: Aug 4, 2021
 *      Author: OS1
 */

#include "mainThr.h"
#include "kernel.h"


extern void userMain(int argc, char* argv[]);
MainThread::MainThread(StackSize stackSize, Time timeSlice, int argc, char* argv[]):Thread(stackSize, timeSlice){

	MainThread::argc=argc;
	MainThread::argv=argv;
	dispatch();


}
void MainThread::run(){

userMain(argc,argv);

}

