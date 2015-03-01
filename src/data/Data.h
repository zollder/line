
#include "Mutexx.h"

#ifndef data_h
#define data_h

// Data structure declaration: line info holder
struct Data
{
	// general info
	int objectId;
	int colorCode;

	// line's center coordinates(pixels)
	float centroidX;
	float centroidY;

	// line's offset from the axes
	float offsetX;
	float offsetY;

	// dimensions (pixels)
	float width;
	float height;
	float radius;

	// line's angle orientation
	float angle;

	// line tracking parameters
	int lifetime;	// # of frames the blob exists
	int active;		// # of frames the blob is active from the last inactive period
	int inactive;	// # of frames the blob was missing

	// read/write protection mechanism
	Mutexx mutex;
};


#endif
