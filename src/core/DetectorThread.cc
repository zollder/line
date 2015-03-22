/*
 *	DetectorThread.cpp
 *  Created on: 15.02.2015
 *	Author: zollder
 */

#include "DetectorThread.h"

//---------------------------------------------------------------------------------------------
// DetectorThread thread implementation.
//---------------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------
	// Constructor
	//-----------------------------------------------------------------------------------------
	DetectorThread::DetectorThread(Detector* detector_p, int threadId)
	{
		printf("Constructing DetectorThread ...\n");

		setThreadId(threadId);
		this->detector = detector_p;
	}

	//-----------------------------------------------------------------------------------------
	// Destructor
	//-----------------------------------------------------------------------------------------
	DetectorThread::~DetectorThread()
	{
		printf("Destroying DetectorThread thread ...\n");
	}

	//-----------------------------------------------------------------------------------------
	// Overrides BaseThread's run() method
	//-----------------------------------------------------------------------------------------
	void* DetectorThread::run()
	{
		printf("Detector started.\n");
		//	detector->startHsvCalibration(false);
		detector->startHsv(VIDEO_ON, FPS_ON);

		return NULL;
	}

