
#include "Detector.h"

//---------------------------------------------------------------------------------------------
// Detector class implementation.
//---------------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------
	// Constructor
	//-----------------------------------------------------------------------------------------
	Detector::Detector(Parameters params, DataService* service)
	{
		printf("\nConstructing Detector ...");
		config = params;
		dataService = service;

        // x-axes points
        pt1 = Point(0, config.frameHeight/2);
        pt2 = Point(config.frameWidth, config.frameHeight/2);

        // y-axes points
        pt3 = Point(config.frameWidth/2, 0);
        pt4 = Point(config.frameWidth/2, config.frameHeight);

        // origin
        pt5 = Point(config.frameWidth/2, config.frameHeight/2);
	}

	//-----------------------------------------------------------------------------------------
	// Destructor
	//-----------------------------------------------------------------------------------------
	Detector::~Detector()
	{
		printf("\nDestroying Detector ...");
	}

	/**------------------------------------------------------------------------------------
	 * Starts blob detection in HSV color space in FPS test mode.
	 ------------------------------------------------------------------------------------*/
	int Detector::startHsv(bool videoOn, bool fpsOn)
	{
		//-----------------------------fps test variables
	    time_t start, end;
	    double fps, sec;
	    int counter = 0;
	    //-----------------------------

	    VideoCapture videoCap(config.camId);
	    if (!videoCap.isOpened())
	    	return -1;

	    videoCap.set(CV_CAP_PROP_FRAME_WIDTH, config.frameWidth);
	    videoCap.set(CV_CAP_PROP_FRAME_HEIGHT, config.frameHeight);

	    if (videoOn)
	    {
			namedWindow("edges1", 1);
			namedWindow("edges2", 1);
	    }

		Mat sourceFrame;
		Mat smoothedFrame;
		Mat hsvFrame;
		Mat segmentedFrame;
		Mat targetFrame;

        vector< vector<Point> > contours;
        vector<Vec4i> hierarchy;

		// retrieve source frame to determine frame size
		videoCap.retrieve(sourceFrame);

		// fps test
		if (fpsOn)
			time(&start);

	    bool quit = false;
	    while (!quit)
	    {
			// fps test
			if (fpsOn)
			{
				time(&end);
				++counter;
				sec = difftime(end, start);
				fps = counter / sec;
				printf("FPS = %.2f\n", fps);
			}

	    	// get frame from camera
	    	videoCap >> sourceFrame;

	    	// prepare image for analysis (smooth, convert to HSV, filter by color, improve)
	    	GaussianBlur(sourceFrame, smoothedFrame, Size(7,7), 1.5, 1.5);
	    	cvtColor(smoothedFrame, hsvFrame, COLOR_BGR2HSV);
	    	inRange(hsvFrame, config.getLowerHsvLimit(), config.getUpperHsvLimit(), segmentedFrame);
	    	morphologyEx(segmentedFrame, segmentedFrame, CV_MOP_OPEN, 1);
	    	morphologyEx(segmentedFrame, segmentedFrame, CV_MOP_CLOSE, 2);

	    	findContours(segmentedFrame, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));

	    	targetFrame = Mat::zeros(segmentedFrame.size(), CV_8UC3);
			vector<RotatedRect> rotatedRectangle(contours.size());
	    	vector< vector<Point> > polygonContours(contours.size());

	    	if (!contours.empty())
	    	{
				// Obtain minimum area (rotated) bounding rectangles and find the largest contour
				RotatedRect largestRectangle = minAreaRect(Mat(contours[0]));
				for (unsigned int i = 0; i < contours.size(); i++)
				{
					rotatedRectangle[i] = minAreaRect(Mat(contours[i]));
					if (rotatedRectangle[i].size.area() > largestRectangle.size.area())
						largestRectangle = rotatedRectangle[i];
				}

				// validate and draw valid rectangle and accompanying components
				bool valid = isValid(largestRectangle);
				if (valid)
				{
					this->saveData(largestRectangle, valid);
					printf("Area:%5.0f, Ang:%5.0f, W:%5.0f, H:%5.0f\n", largestRectangle.size.area(), largestRectangle.angle,
							largestRectangle.size.width, largestRectangle.size.height);

					// draw contour and rectangle for the largest detected object on the source frame
					Point2f vertices[4];
					largestRectangle.points(vertices);
					for (int j = 0; j < 4; j++)
						line(sourceFrame, vertices[j], vertices[(j+1)%4], rgbBlue, 1, 8);

					Point pt6 = Point(largestRectangle.center.x, largestRectangle.center.y);
					line(sourceFrame, pt5, pt6, rgbGreen, 1);
					circle(sourceFrame, pt6, 2, rgbGreen, 2, CV_FILLED, 0);
				}
				else
					this->saveData(largestRectangle, valid);

				// draw all detected contours and rectangles (for debugging purposes)
				if (videoOn)
				{
					// random number generator
					RNG generator(12345);

					// draw contours and rectangles on the pre-processed frame
					for (unsigned int i = 0; i < contours.size(); i++)
					{
						Scalar randomColor = Scalar(generator.uniform(0,255), generator.uniform(0,255), generator.uniform(0,255));
						drawContours(targetFrame, polygonContours, i, randomColor, 1, 8, vector<Vec4i>(), 0, Point());

						Point2f vertices[4];
						rotatedRectangle[i].points(vertices);
						for (int j = 0; j < 4; j++)
							line(targetFrame, vertices[j], vertices[(j+1)%4], randomColor, 1, 8);
					}
				}
	    	}

			// TODO: make sure the object is rectangular

			if (videoOn)
			{
				// draw x and y axes on the source frame
				line(sourceFrame, pt1, pt2, rgbRed, 1);
				line(sourceFrame, pt3, pt4, rgbRed, 1);

				// show results
				imshow("edges1", sourceFrame);
				imshow("edges2", targetFrame);
			}

	    	if (waitKey(30) >= 0)
	    		quit = true;
	    }

	    sourceFrame.release();
	    targetFrame.release();
	    segmentedFrame.release();
	    hsvFrame.release();
	    smoothedFrame.release();
	    videoCap.release();

	    return 0;
	}

	/**------------------------------------------------------------------------------------
	 * Filters largest contour rectangle according to specified criteria.
	 ------------------------------------------------------------------------------------*/
	bool Detector::isValid(RotatedRect &rectangle)
	{
		// filter by area
		if (rectangle.size.area() < 2400)
			return false;

		if (rectangle.size.area() > 10000)
			return false;

		// filter by width/height ratio
		float width = rectangle.size.width;
		float heigth = rectangle.size.height;

		if ((width >= heigth) and (width/heigth < 2))
			return false;

		if (((width <= heigth) and (heigth/width < 2)))
			return false;

		return true;
	}

	/**------------------------------------------------------------------------------------
	 * Retrieves and saves object data in the shared data structure
	 ------------------------------------------------------------------------------------*/
	void Detector::saveData(RotatedRect &rectangle, bool valid)
	{
		// TODO: interpret angle value correctly
		if (valid)
		{
			dataService->data->mutex.lock();
				dataService->data->centroidX = rectangle.center.x;
				dataService->data->centroidY = rectangle.center.y;
				dataService->data->angle = rectangle.angle;
				dataService->data->offsetX = (float) (config.frameWidth/2)-(rectangle.center.x);
				dataService->data->offsetY = (float) (config.frameHeight/2)-(rectangle.center.y);
				dataService->data->width = rectangle.size.width;
				dataService->data->height = rectangle.size.height;
			dataService->data->mutex.unlock();
		}
		else
		{
			dataService->data->mutex.lock();
				dataService->data->reset();
			dataService->data->mutex.unlock();
		}
	}


