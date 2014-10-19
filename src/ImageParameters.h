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

		double lowerRed;
		double lowerGreen;
		double lowerBlue;

		double upperRed;
		double upperGreen;
		double upperBlue;

		// constructor
		ImageParameters()
		{
			lowerRed = 0;
			lowerGreen = 0;
			lowerBlue = 0;

			upperRed = 255;
			upperGreen = 255;
			upperBlue = 255;
		}

		// destructor
		~ImageParameters()
		{
			printf("\n\nDestroying ImageParameters ... \n");
		}

		// sets lower RGB threshold values
		void setLowerRgb(double R, double G, double B)
		{
			lowerRed = R;
			lowerGreen = G;
			lowerBlue = B;
		}

		// sets upper RGB threshold values
		void setUpperRgb(double R, double G, double B)
		{
			upperRed = R;
			upperGreen = G;
			upperBlue = B;
		}

};

#endif
