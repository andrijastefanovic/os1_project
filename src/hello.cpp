/*
 * hello.cpp
 *
 *  Created on: Apr 1, 2015
 *      Author: OS1
 */





#include <iostream.h>
#include "kernel.h"
#include "ksem.h"
#include "IVTEntry.h"





extern int userMain(int argc, char** argv);

void main(int argc, char* argv[])
{

	Kernel k=Kernel(argc, argv);



	k.mainThr->start();


	k.mainThr->waitToComplete();


}


