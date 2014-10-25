#include <cv.h>
#include <stdio.h>

#ifndef imageparameters_h
#define imageparameters_h

/*
 * ImageParameters class.
 * Encapsulates RGB threshold values.
 */
class ImageParameters
{
	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:

		int lowerH;
		int lowerS;
		int lowerV;

		int upperH;
		int upperS;
		int upperV;

		// constructor
		ImageParameters()
		{
			printf("\nInitializing ImageParameters ...");

			lowerH = 0;
			lowerS = 0;
			lowerV = 0;

			upperH = 255;
			upperS = 255;
			upperV = 255;
		}

		// destructor
		~ImageParameters()
		{
			printf("\nDestroying ImageParameters ...");
		}

		// sets lower HSV threshold values
		void setLowerHsv(int h, int s, int v)
		{
			lowerH = h;
			lowerS = s;
			lowerV = v;
		}

		// sets upper HSV threshold values
		void setUpperHsv(int h, int s, int v)
		{
			upperH = h;
			upperS = s;
			upperV = v;
		}

		// returns lower HSV limit as an OpenCV scalar value
		CvScalar getLowerLimit()
		{
			return cvScalar(lowerH, lowerS, lowerV);
		}

		// returns upper HSV limit as an OpenCV scalar value
		CvScalar getUpperLimit()
		{
			return cvScalar(upperH, upperS, upperV);
		}
};

#endif
