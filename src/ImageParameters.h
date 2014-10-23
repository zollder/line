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

		double lower1;
		double lower2;
		double lower3;

		double upper1;
		double upper2;
		double upper3;

		// constructor
		ImageParameters()
		{
			printf("\nInitializing ImageParameters ...");

			lower1 = 0;
			lower2 = 0;
			lower3 = 0;

			upper1 = 255;
			upper2 = 255;
			upper3 = 255;
		}

		// destructor
		~ImageParameters()
		{
			printf("\nDestroying ImageParameters ...");
		}

		// sets lower RGB or HSV threshold values
		void setLowerRgb(double first, double second, double third)
		{
			lower1 = first;
			lower2 = second;
			lower3 = third;
		}

		// sets upper RGB or HSV threshold values
		void setUpperRgb(double first, double second, double third)
		{
			upper1 = first;
			upper2 = second;
			upper3 = third;
		}

		// sets lower RGB or HSV threshold values
		void setLowerHsv(double first, double second, double third)
		{
			lower1 = first;
			lower2 = second;
			lower3 = third;
		}

		// sets upper RGB or HSV threshold values
		void setUpperHsv(double first, double second, double third)
		{
			upper1 = first;
			upper2 = second;
			upper3 = third;
		}

		CvScalar getLowerThreshold()
		{
			return cvScalar(lower1, lower2, lower3);
		}

		CvScalar getUpperThreshold()
		{
			return cvScalar(upper1, upper2, upper3);
		}
};

#endif
