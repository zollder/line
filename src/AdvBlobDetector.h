/*
 * AdvBlobDetector.h
 *
 *  Created on: 04/02/2015
 *  Author: zollder
 *  Inspired by: https://code.google.com/p/my-masters-thesis/source/browse/trunk/MastersThesis/Sample/SimpleBlobDetecter_demo.cpp?r=66
 */

// dependencies (headers)
#include "VideoParameters.h"
#include "ImageParameters.h"
#include "BlobsInfoDao.h"

// openCV and cvBlob headers
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <cvblob.h>

#include <time.h>
#include <iostream>
#include <cmath>

// name spaces
using namespace cv;
using namespace cvb;
using namespace std;

#ifndef advblobdetector_h
#define advblobdetector_h

//-----------------------------------------------------------------------------------------
// AdvBlobDetector interface.
//-----------------------------------------------------------------------------------------
class AdvBlobDetector
{
	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:

		// constructor
		AdvBlobDetector(VideoParameters *video_p, ImageParameters* image_p, BlobsInfoDao* blobsInfo_p);

		// destructor
		~AdvBlobDetector();

		/**------------------------------------------------------------------------------------
		 * Starts blob detection in HSV color space.
		 * Enables/disables corresponding options with true/false values respectively
		 ------------------------------------------------------------------------------------*/
		int startHsv(bool videoOn, bool fpsOn);

		/**------------------------------------------------------------------------------------
		 * Starts blob detection in calibration mode.
		 * Allows to determine color threshold values with high precision.
		 * Enables/disables detailed console logs with true/false param value.
		 ------------------------------------------------------------------------------------*/
		int startHsvCalibration(bool logsOn);

	//-----------------------------------------------------------------------------------------
	// Private members
	//-----------------------------------------------------------------------------------------
	private:

		// object holders
		VideoParameters* videoParams;
		ImageParameters* imageParams;
		BlobsInfoDao* blobsInfo;

		// pure RGB color holders
		cv::Scalar rgbRed, rgbGreen, rgbBlue;

		/**------------------------------------------------------------------------------------
		 * Helper method.
		 * Fills and returns BlobData holder with calculated and retrieved data.
		 --------------------------------------------------------------------------------------*/
		BlobData packageBlobData(CvBlob* blob_p, int width, int height);
};

#endif
