
#include "Mutexx.h"

#ifndef data_h
#define data_h

// Data structure declaration: line info holder
struct Data
{
	// line's center coordinates(pixels)
	float centroidX;
	float centroidY;

	// line's offset from the axes
	float offsetX;
	float offsetY;

	// dimensions (pixels)
	float width;
	float height;

	// line's angle orientation
	float angle;

	// line tracking parameters
	int lifetime;	// # of frames the blob exists
	int active;		// # of frames the blob is active from the last inactive period
	int inactive;	// # of frames the blob was missing

	// read/write protection mechanism
	Mutexx mutex;

	void reset()
	{
		centroidX = 0.0;
		centroidY = 0.0;
		offsetX = 0.0;
		offsetY = 0.0;
		width = 0.0;
		height = 0.0;
		angle = 0.0;
	}
};


#endif
