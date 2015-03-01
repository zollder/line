/*
 * Config.h
 * Global shared configuration holder.
 */

#include <opencv2/opencv.hpp>
#include <stdio.h>

#ifndef parameters_h
#define parameters_h

using namespace cv;

class Parameters
{
	public:

		/*  Default camera parameters */
		int camId = 0;
		int frameWidth = 1280;
		int frameHeight = 720;

		/*  HSV color-space default threshold values */
		int lowerH = 86;
		int lowerS = 121;
		int lowerV = 131;

		int upperH = 117;
		int upperS = 200;
		int upperV = 255;

		// sets camera ID
		void setCameraId(int id)
		{
			camId = id;
		}

		// sets frame size
		void setFrameSize(int w, int h)
		{
			frameWidth = w;
			frameHeight = h;
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
		Scalar getLowerHsvLimit()
		{
			return Scalar(lowerH, lowerS, lowerV);
		}

		// returns upper HSV limit as an OpenCV scalar value
		Scalar getUpperHsvLimit()
		{
			return Scalar(upperH, upperS, upperV);
		}
};

#endif
