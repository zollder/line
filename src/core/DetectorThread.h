/*
 *	DetectorThread.h
 *	Created on: 15.02.2015
 *	Author: zollder
 */

#include "../commons/Config.h"
#include "../sys/BaseThread.h"
#include "../sys/FdTimer.h"

#include "Detector.h"

#ifndef dmuthread_h
#define dmuthread_h

//-----------------------------------------------------------------------------------------
// DetectorThread interface.
//-----------------------------------------------------------------------------------------
class DetectorThread : public BaseThread
{
	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:

		// constructor
		DetectorThread(Detector* detector, int threadId);

		// destructor
		~DetectorThread();

		// overrides BaseThread's run() method
		void* run();

	//-----------------------------------------------------------------------------------------
	// Private members
	//-----------------------------------------------------------------------------------------
	private:

		FdTimer* timer;
		Detector* detector;
};

#endif
