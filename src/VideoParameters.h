#include <stdio.h>

#ifndef videoparameters_h
#define videoparameters_h

/*
 * VideoParameters class definition and implementation.
 * Encapsulates webcam parameters.
 */
class VideoParameters
{
	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:

		int cameraId;
		int captureWidth;
		int captureHeight;

//		CameraParams* cameraParams;

		// constructor
		VideoParameters(int id, int width, int height)
		{
			printf("\nInitializing VideoParameters ... ");

			cameraId = id;
			captureWidth = width;
			captureHeight = height;
		}

		// destructor
		~VideoParameters()
		{
			printf("\nDestroying VideoParameters ... ");
		}
};

#endif
