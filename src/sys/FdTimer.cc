/*
 *	FdTimer.cpp
 *  Created on: 11.12.2014
 *	Author: zollder
 *	Inspired by: http://www.2net.co.uk/tutorial/periodic_threads#ref_1
 */

#include "FdTimer.h"

//---------------------------------------------------------------------------------------------
// Periodic FdTimer class implementation.
//---------------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------
	// Constructor
	//-----------------------------------------------------------------------------------------
	FdTimer::FdTimer(int id, double interval)
	{
		printf("Creating and initializing FdTimer %d ...\n", id);

		timerId = id;

		// convert interval to seconds and nanoseconds and initialize corresponding members
		setInterval(interval);

		// initialize notification structure and create timer
		createTimer();
	}

	//-----------------------------------------------------------------------------------------
	// Destructor.
	//-----------------------------------------------------------------------------------------
	FdTimer::~FdTimer()
	{
		printf("Destroying FdTimer %d ...\n", timerId);
	}

	//-----------------------------------------------------------------------------------------
	/** Converts specified time interval into seconds and nanoseconds. */
	//-----------------------------------------------------------------------------------------
	void FdTimer::setInterval(double interval)
	{
		seconds = floor(interval);
		nanoseconds = (interval - seconds)*pow(10,9);
	}

	//-----------------------------------------------------------------------------------------
	/** Creates timer object.
	 *  Returns and saves a timer file descriptor reference (timerRef). */
	//-----------------------------------------------------------------------------------------
	void FdTimer::createTimer()
	{
		// CLOCK_MONOTONIC - not affected by discontinuous changes in the system clock
		// TFD_CLOEXEC - sets the close-on-exec flag (see timerfd doc for details)
		timerRef = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC);
		if (timerRef < 0)
		{
			printf("Timer %d creation error.\n", timerId);
			exit(EXIT_FAILURE);
		}

		// initialize timer info holder
		timerInfo.descriptor = timerRef;
		timerInfo.missedEvents = 0;

		printf("Timer %d created successfully. \n", timerId);
	}

	//-----------------------------------------------------------------------------------------
	/** Starts periodic timer. */
	//-----------------------------------------------------------------------------------------
	int FdTimer::start()
	{
		// (re)initializes timer structure
		reset();

		// start the timer and running status accordingly
		int result = timerfd_settime (timerRef, 0, &timer, NULL);
		if (result != 0)
		{
			printf("Error creating timer %d.\n", timerId);
			exit(EXIT_FAILURE);
		}
		else
			printf("Timer %d started.\n", timerId);

		return result;
	}

	//-----------------------------------------------------------------------------------------
	/** Starts single-shot timer. */
	//-----------------------------------------------------------------------------------------
	int FdTimer::startSingle()
	{
		// (re)initializes timer structure
		resetSingle();

		// start the timer and running status accordingly
		int result = timerfd_settime (timerRef, 0, &timer, NULL);
		if (result != 0)
		{
			printf("Error creating timer %d.\n", timerId);
			exit(EXIT_FAILURE);
		}
		else
			printf("Timer %d started.\n", timerId);

		return result;
	}

	//-----------------------------------------------------------------------------------------
	/** Stops timer by nullifying its timer values and updating active system timer. */
	//-----------------------------------------------------------------------------------------
	int FdTimer::stop()
	{
		timer.it_value.tv_sec = 0;
		timer.it_value.tv_nsec = 0;
		timer.it_interval.tv_sec = 0;
		timer.it_interval.tv_nsec = 0;

		int result = timerfd_settime (timerRef, 0, &timer, NULL);
		if (result != 0)
			printf("Error stopping timer %d.\n", timerId);
		else
			printf("Timer %d stopped.\n", timerId);

		return result;
	}

	//-----------------------------------------------------------------------------------------
	/** Blocks the execution until the next timer event.
	 *  Keeps track of missed timer events. */
	//-----------------------------------------------------------------------------------------
	void FdTimer::waitTimerEvent()
	{
		unsigned long long missedEvent;
		int result = read(timerInfo.descriptor, &missedEvent, sizeof(missedEvent));
		if (result < 0)
		{
			printf("Error reading timer %d.\n", timerId);
			exit(EXIT_FAILURE);
		}

		timerInfo.missedEvents += missedEvent;
	}

	//-----------------------------------------------------------------------------------------
	/** (Re)Initializes periodic timer structure.*/
	//-----------------------------------------------------------------------------------------
	void FdTimer::reset()
	{
		// periodic timer: will go off in 1 sec and then again every sec+nanosec
		timer.it_value.tv_sec = 1;
		timer.it_value.tv_nsec = 0;
		timer.it_interval.tv_sec = getSeconds();
		timer.it_interval.tv_nsec = getNanoseconds();
	}

	//-----------------------------------------------------------------------------------------
	/** (Re)Initializes one-shot timer structure.*/
	//-----------------------------------------------------------------------------------------
	void FdTimer::resetSingle()
	{
		// one-shot timer: will go off only once in sec+nanosec
		timer.it_value.tv_sec = getSeconds();
		timer.it_value.tv_nsec = getNanoseconds();
		timer.it_interval.tv_sec = 0;
		timer.it_interval.tv_nsec = 0;
	}

	//-----------------------------------------------------------------------------------------
	/** Returns seconds portion (converted) of the timer interval.*/
	//-----------------------------------------------------------------------------------------
	long FdTimer::getSeconds()
	{
		return seconds;
	}

	//-----------------------------------------------------------------------------------------
	/** Returns nanoseconds portion (converted) of the timer interval.*/
	//-----------------------------------------------------------------------------------------
	long FdTimer::getNanoseconds()
	{
		return nanoseconds;
	}

	//-----------------------------------------------------------------------------------------
	/** Returns timer ID.*/
	//-----------------------------------------------------------------------------------------
	int FdTimer::getTimerId()
	{
		return timerId;
	}
