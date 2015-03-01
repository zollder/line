/*
 * Detector.h
 *
 *  Created on: 04/02/2015
 *  Author: zollder
 */

#include <time.h>
#include <iostream>
#include <cmath>

// openCV and cvBlob headers
#include <opencv/highgui.h>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

// dependencies (headers)
#include "../commons/Parameters.h"
#include "../data/DataService.h"

// name spaces
using namespace cv;
using namespace std;

#ifndef detector_h
#define detector_h

//-----------------------------------------------------------------------------------------
// Detector interface.
//-----------------------------------------------------------------------------------------
class Detector
{
	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:

		// constructor
		Detector(Parameters params, DataService* service);

		// destructor
		~Detector();

		/**------------------------------------------------------------------------------------
		 * Starts blob detection in HSV color space.
		 * Enables/disables corresponding options with true/false values respectively
		 ------------------------------------------------------------------------------------*/
		int startHsv();

	//-----------------------------------------------------------------------------------------
	// Private members
	//-----------------------------------------------------------------------------------------
	private:

		Parameters config;
		DataService* dataService;

		// pure RGB color holders
		Scalar rgbRed = CV_RGB(250, 0, 0);
		Scalar rgbGreen = CV_RGB(0, 250, 0);
		Scalar rgbBlue = CV_RGB(0, 0, 250);

		Point pt1, pt2, pt3, pt4, pt5;
};

#endif
