/*
 * BlobDetector.h
 *
 *  Created on: Oct 15, 2014
 *      Author: zollder
 */

// dependencies (headers)
#include"VideoParameters.h"
#include"ImageParameters.h"

// openCV and cvBlob headers
#include <cv.h>
#include <highgui.h>
#include<opencv2/opencv.hpp>
#include<cvblob.h>

// input/output headers
#include<iostream>

// name spaces
using namespace cv;
using namespace cvb;
using namespace std;

#ifndef blobdetector_h
#define blobdetector_h

//-----------------------------------------------------------------------------------------
// BlobDetector interface.
//-----------------------------------------------------------------------------------------
class BlobDetector
{
	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:

		// constructor
		BlobDetector(VideoParameters *videoParams, ImageParameters* imageParams);

		// destructor
		~BlobDetector();

		// start blob detection in HSV color-space
		void startHsv();

		// start blob detection in RGB color-space
		int startRgb();

		void stop();

	//-----------------------------------------------------------------------------------------
	// Private members
	//-----------------------------------------------------------------------------------------
	private:
		VideoParameters* videoParams;
		ImageParameters* imageParams;

		// local capture size holders
		int width;
		int height;

		// OpenCV threshold holders
		CvScalar lowerThreshold;
		CvScalar upperThreshold;

		// pure RGB colors
		CvScalar rgbRed, rgbGreen, rgbBlue;
};

#endif
