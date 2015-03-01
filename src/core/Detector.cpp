
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
	int Detector::startHsv()
	{
	    VideoCapture videoCap(config.camId);
	    if (!videoCap.isOpened())
	    	return -1;

	    videoCap.set(CV_CAP_PROP_FRAME_WIDTH, config.frameWidth);
	    videoCap.set(CV_CAP_PROP_FRAME_HEIGHT, config.frameHeight);

	    namedWindow("edges1", 1);
	    namedWindow("edges2", 1);

		Mat sourceFrame;
		Mat smoothedFrame;
		Mat hsvFrame;
		Mat segmentedFrame;
		Mat targetFrame;

        vector< vector<Point> > contours;
        vector<Vec4i> hierarchy;

        // random number generator
        RNG generator(12345);

		// retrieve source frame to determine frame size
		videoCap.retrieve(sourceFrame);

	    bool quit = false;
	    while (!quit)
	    {
	    	// get frame from camera
	    	videoCap >> sourceFrame;
	    	imshow("edges1", sourceFrame);

	    	// prepare image for analysis (smooth, convert to HSV, filter by color, improve)
	    	GaussianBlur(sourceFrame, smoothedFrame, Size(7,7), 1.5, 1.5);
	    	cvtColor(smoothedFrame, hsvFrame, COLOR_BGR2HSV);
	    	inRange(hsvFrame, config.getLowerHsvLimit(), config.getUpperHsvLimit(), segmentedFrame);
	    	morphologyEx(segmentedFrame, segmentedFrame, CV_MOP_OPEN, 1);
	    	morphologyEx(segmentedFrame, segmentedFrame, CV_MOP_CLOSE, 2);

	    	findContours(segmentedFrame, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));

			vector<RotatedRect> minAreaRectangle(contours.size());
	    	vector< vector<Point> > polygonContours(contours.size());
	    	vector<Point2f> center(contours.size());

	    	// Obtain minimum area (rotated) bounding rectangles and filter by max area
			RotatedRect maxAreaRectangle = minAreaRect(Mat(contours[0]));
			for (unsigned int i = 0; i < contours.size(); i++)
			{
				minAreaRectangle[i] = minAreaRect(Mat(contours[i]));
				if (minAreaRectangle[i].size.area() > maxAreaRectangle.size.area())
					maxAreaRectangle = minAreaRectangle[i];
			}

			// draw contours and rectangles on the pre-processed frame
			targetFrame = Mat::zeros(segmentedFrame.size(), CV_8UC3);
			for (unsigned int i = 0; i < contours.size(); i++)
			{
				Scalar randomColor = Scalar(generator.uniform(0,255), generator.uniform(0,255), generator.uniform(0,255));
				drawContours(targetFrame, polygonContours, i, randomColor, 1, 8, vector<Vec4i>(), 0, Point());

				Point2f vertices[4];
				minAreaRectangle[i].points(vertices);
				for (int j = 0; j < 4; j++)
					line(targetFrame, vertices[j], vertices[(j+1)%4], randomColor, 1, 8);
			}

			// TODO: create a finction: draw x and y axes on the source frame
			line(sourceFrame, pt1, pt2, rgbRed, 1);
			line(sourceFrame, pt3, pt4, rgbRed, 1);

			// TODO: create a finction:  draw contour and rectangle for the largest detected object on the source frame
			Point2f vertices[4];
			maxAreaRectangle.points(vertices);
			for (int j = 0; j < 4; j++)
				line(sourceFrame, vertices[j], vertices[(j+1)%4], rgbGreen, 1, 8);

			Point pt6 = Point(maxAreaRectangle.center.x, maxAreaRectangle.center.y);
			line(sourceFrame, pt5, pt6, rgbGreen, 1);
			circle(sourceFrame, pt6, 2, rgbGreen, 2, CV_FILLED, 0);

	    	imshow("edges1", sourceFrame);
	    	imshow("edges2", targetFrame);

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

	void drawAxes(Mat frame)
	{

	}


