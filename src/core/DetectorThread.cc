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
		timer = new FdTimer(getThreadId(), DETECTOR_INTERVAL);
		this->detector = detector_p;
	}

	//-----------------------------------------------------------------------------------------
	// Destructor
	//-----------------------------------------------------------------------------------------
	DetectorThread::~DetectorThread()
	{
		printf("Destroying DetectorThread thread ...\n");
		delete timer;
	}

	//-----------------------------------------------------------------------------------------
	// Overrides BaseThread's run() method
	//-----------------------------------------------------------------------------------------
	void* DetectorThread::run()
	{
		//	detector->startHsvCalibration(false);
		detector->startHsv();

		return NULL;
	}

