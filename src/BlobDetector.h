/*
 * BlobDetector.h
 *
 *  Created on: Oct 15, 2014
 *      Author: zollder
 */

// dependencies (headers)
#include"VideoParameters.h"
#include"ImageParameters.h"
#include "BlobsInfoDao.h"
#include "ColorConverter.h"

// openCV and cvBlob headers
#include <cv.h>
#include <highgui.h>
#include<opencv2/opencv.hpp>
#include<cvblob.h>

#include <time.h>
#include<iostream>
#include<cmath>

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
		BlobDetector(VideoParameters *video_p, ImageParameters* image_p, BlobsInfoDao* blobsInfo_p);

		// destructor
		~BlobDetector();

		// start blob detection in HSV color-space
		void startHsv();

		// start blob detection in RGB color-space with video
		int startRgb(bool videoOn);

		// start blob detection in HSV color-space with video
		int startHsv(bool videoOn);

	//-----------------------------------------------------------------------------------------
	// Private members
	//-----------------------------------------------------------------------------------------
	private:

		// object holders
		VideoParameters* videoParams;
		ImageParameters* imageParams;
		BlobsInfoDao* blobsInfo;
		ColorConverter* converter;

		// local capture size holders
		int width;
		int height;

		// OpenCV threshold holders
		CvScalar lowerThreshold;
		CvScalar upperThreshold;

		// pure RGB color holders
		CvScalar rgbRed, rgbGreen, rgbBlue;

		/**------------------------------------------------------------------------------------
		 * Helper method.
		 * Fills and returns BlobData holder with calculated and retrieved data.
		 --------------------------------------------------------------------------------------*/
		BlobData packageBlobData(CvBlob* blob_p, int width, int height);
};

#endif
