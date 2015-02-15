
#include "AdvBlobDetector.h"

#include <highgui.h>
//---------------------------------------------------------------------------------------------
// AdvBlobDetector class implementation.
//---------------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------
	// Constructor
	//-----------------------------------------------------------------------------------------
	AdvBlobDetector::AdvBlobDetector(VideoParameters *video_p, ImageParameters* image_p, BlobsInfoDao* blobsInfo_p)
	{
		printf("\nConstructing AdvBlobDetector ...");

		videoParams = video_p;
		imageParams = image_p;
		blobsInfo = blobsInfo_p;

		rgbRed = CV_RGB(250, 0, 0);
		rgbGreen = CV_RGB(0, 250, 0);
		rgbBlue = CV_RGB(0, 0, 250);
	}

	//-----------------------------------------------------------------------------------------
	// Destructor
	//-----------------------------------------------------------------------------------------
	AdvBlobDetector::~AdvBlobDetector()
	{
		printf("\nDestroying AdvBlobDetector ...");
	}

	/**------------------------------------------------------------------------------------
	 * Starts blob detection in HSV color space in FPS test mode.
	 ------------------------------------------------------------------------------------*/
	int AdvBlobDetector::startHsv(bool videoOn, bool fpsOn)
	{
	    VideoCapture videoCap(videoParams->cameraId);
	    if (!videoCap.isOpened())
	    	return -1;

	    videoCap.set(CV_CAP_PROP_FRAME_WIDTH, videoParams->captureWidth);
	    videoCap.set(CV_CAP_PROP_FRAME_HEIGHT, videoParams->captureHeight);

	    Mat edges;
	    namedWindow("edges1", 1);
	    namedWindow("edges2", 1);

		Mat sourceFrame;
		Mat targetFrame;
		Mat smoothedFrame;
		Mat hsvFrame;
		Mat segmentedFrame;

        vector<KeyPoint> keyPoints;
        vector< vector <Point> > contours;
        vector< vector <Point> > approxContours;

    	// -------------------------------------------------------------------------
        // Initialize blob parameters
        // -------------------------------------------------------------------------
    	SimpleBlobDetector::Params params;

    	params.filterByArea = true;
//    	params.filterByConvexity = true;
    	params.filterByInertia = true;
    	params.filterByCircularity = false;
    	params.filterByColor = true;

    	params.minArea = 100;
//    	params.minConvexity = 0.3;
//    	params.minDistBetweenBlobs = 0;
//    	params.minInertiaRatio = 0.01;
    	params.blobColor = 200;

    	params.maxArea = 10000;
//    	params.maxConvexity = 10;

    	SimpleBlobDetector blobDetector(params);
    	blobDetector.create("BlobDetector");
    	// ----------------------------------------------

		// retrieve source frame to determine frame size
		videoCap.retrieve(sourceFrame);
//		MatSize frameSize = sourceFrame.size;

	    bool quit = false;
	    while (!quit)
	    {
	    	// get frame from camera
	    	videoCap >> sourceFrame;

	    	// prepare image for analysis (smooth, convert to HSV, filter by color, improve)
	    	GaussianBlur(sourceFrame, smoothedFrame, Size(7,7), 1.5, 1.5);
	    	cvtColor(smoothedFrame, hsvFrame, COLOR_BGR2HSV);
	    	inRange(hsvFrame, imageParams->getLowerLimit(), imageParams->getUpperLimit(), segmentedFrame);
	    	morphologyEx(segmentedFrame, segmentedFrame, CV_MOP_OPEN, 1);
	    	morphologyEx(segmentedFrame, segmentedFrame, CV_MOP_CLOSE, 2);

	    	blobDetector.detect(segmentedFrame, keyPoints, contours);
	    	drawKeypoints(segmentedFrame, keyPoints, targetFrame, CV_RGB(0,255,0), DrawMatchesFlags::DEFAULT);
	    	approxContours.resize(contours.size());

            for( int i = 0; i < contours.size(); ++i )
            {
				approxPolyDP( Mat(contours[i]), approxContours[i], 4, 1 );
				drawContours(targetFrame, contours, i, CV_RGB(rand()&255, rand()&255, rand()&255) );
				drawContours(targetFrame, approxContours, i, CV_RGB(rand()&255, rand()&255, rand()&255) );
            }
            cout << "Keypoints " << keyPoints.size() << endl;

//	    	Canny(segmentedFrame, segmentedFrame, 0, 30, 3);
	    	imshow("edges1", segmentedFrame);
	    	imshow("edges2", targetFrame);

//	    	Mat blobImg;
//	    	cv::drawKeypoints(sourceFrame, keyPoints, blobImg);
//	    	cv::imshow("Blobs", blobImg);

	    	if (waitKey(30) >= 0)
	    		quit = true;
	    }

	    sourceFrame.release();
	    edges.release();
	    edges.release();
	    videoCap.release();
	    return 0;
	}


//		CvCapture* capture;				// webcam feed structure holder
//		CvSize imageSize;				// current frame holder (from the camera feed)
//
//		IplImage *sourceFrame;			// source frame
//		IplImage *targetFrame;			// target frame
//		IplImage *smoothedFrame;		// blurred frame
//		IplImage *hsvFrame;				// frame in the HSV color space
//		IplImage *segmentedFrame;		// thresholded frame holder
//		IplImage *labelFrame;			// labeled blobs holder
//
//		CvTracks tracks;	// structure to hold tracks
//		CvBlobs blobs;		// structure to hold blobs
//		CvBlob* blob;		// pointer to the largest blob in the list of blobs
//
//		CvPoint pt1, pt2, pt3, pt4, pt5, pt6;
//		int picWidth, picHeight, thickness;
//
//		if (videoOn)
//		{
//			cvNamedWindow("Webcam Preview", CV_WINDOW_AUTOSIZE);
//			cvMoveWindow("Webcam Preview", 100, 100);
//		}
//
//	    capture = cvCaptureFromCAM(videoParams->cameraId);
//	    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, videoParams->captureWidth);
//	    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, videoParams->captureHeight);
//
//	    sourceFrame = cvRetrieveFrame(capture);
//	    if (sourceFrame == NULL) {
//	        return -1;
//	    }
//
//	    imageSize = cvGetSize(sourceFrame);
//	    printf("\nFrame width (pixels): %d | Frame height (pixels): %d", imageSize.width, imageSize.height);
//
//		targetFrame = cvCreateImage(imageSize, sourceFrame->depth, sourceFrame->nChannels);
//		smoothedFrame = cvCreateImage(imageSize, targetFrame->depth, targetFrame->nChannels);
//		hsvFrame = cvCreateImage(imageSize, targetFrame->depth, targetFrame->nChannels);
//		segmentedFrame = cvCreateImage(imageSize, targetFrame->depth, 1);
//		labelFrame = cvCreateImage(imageSize, IPL_DEPTH_LABEL, 1);
//
//		printf("\nFrame depth: %d | Frame channels: %d", sourceFrame->depth, sourceFrame->nChannels);
//
//		// fps test
//		if (fpsOn)
//			time(&start);
//
//		bool quit = false;
//		while (!quit && cvGrabFrame(capture))
//		{
//			sourceFrame = cvRetrieveFrame(capture);
//			if(!sourceFrame)
//			{
//				printf("\nFrame fault.");
//				break;
//			}
//
//			// fps test
//			if (fpsOn)
//			{
//				time(&end);
//				++counter;
//				sec = difftime(end, start);
//				fps = counter / sec;
//				printf("\nFPS = %.2f\n", fps);
//			}
//
////			cvFlip(targetFrame, targetFrame, 1);
//
//			// copies one array to another with optional scaling and optional type conversion
//			// cvConvert can be used instead (scale=1, shift=0 no prescaling is done)
//			cvConvertScale(sourceFrame, targetFrame, 1, 0);
//
//			// smoothes the frame with Gaussian filter with 5x5 kernel (done in-place)
//			// good results, but slower than other filters (see details on  pp. 111-114 of the "Learning OpenCV")
//			cvSmooth(targetFrame, smoothedFrame, CV_GAUSSIAN, 3, 3);
//
//			// converts the frame from RGB to HSV color-space
//			cvCvtColor(smoothedFrame, hsvFrame, COLOR_BGR2HSV);
//
//			// TODO: try adaptive thresholding
//			// threshold the frame according to predefined lower and upper HSV limits
//			cvInRangeS(hsvFrame, imageParams->getLowerLimit(), imageParams->getUpperLimit(), segmentedFrame);
//
//			//morphological opening (removes small objects from the foreground)
//			cvMorphologyEx(segmentedFrame, segmentedFrame, 0, 0, CV_MOP_OPEN, 1);
//
//			//morphological closing (removes small holes from the foreground)
//			cvMorphologyEx(segmentedFrame, segmentedFrame, 0, 0, CV_MOP_CLOSE, 1);
//
//			// find, filter (by area) and render blobs
//			cvLabel(segmentedFrame, labelFrame, blobs);
//			cvFilterByArea(blobs, 500, 1000000);
//			cvRenderBlobs(labelFrame, blobs, targetFrame, targetFrame, CV_BLOB_RENDER_COLOR, 0.5);
//
//			// update and render blob tracks
//			cvUpdateTracks(blobs, tracks, 200., 5);
//			cvRenderTracks(tracks, targetFrame, targetFrame, CV_TRACK_RENDER_BOUNDING_BOX, NULL);
//
//			//-----------------------------------------------------------------------------------------
//			// Visualize image processing
//			//-----------------------------------------------------------------------------------------
//
//			thickness = 1;
//			picWidth = targetFrame->width;
//			picHeight = targetFrame->height;
//
//			if (videoOn)
//			{
//				// draw x-axes
//		        pt3 = cvPoint(0, picHeight / 2);
//		        pt4 = cvPoint(picWidth, picHeight / 2);
//		        cvLine(targetFrame, pt3, pt4, rgbRed, thickness);
//
//		        // draw y-axes
//		        pt1 = cvPoint(picWidth/2, 0);
//		        pt2 = cvPoint(picWidth/2, picHeight);
//		        cvLine(targetFrame, pt1, pt2, rgbRed, thickness);
//			}
//
//	        // find largest blob (biggest area).
//	        int largestBlobIndex = cvGreaterBlob(blobs);
//	        if (largestBlobIndex)
//	        {
//	            blob = blobs[largestBlobIndex];
//	            pt5 = cvPoint(picWidth/2, picHeight/2);
//	            pt6 = cvPoint(blob->centroid.x, blob->centroid.y);
//	            cvLine(targetFrame, pt5, pt6, rgbGreen, thickness);
//	            cvCircle(targetFrame, pt6, 3, rgbGreen, 2, CV_FILLED, 0);
//
//	            BlobData blobData = packageBlobData(blob, picWidth, picHeight);
//	            this->blobsInfo->setLargestBlob(blobData);
//
////	            printf("\nBlobs found: %lu", blobs.size());
////	            printf("\ncenter-x: %.2f center-y: %.2f", blobData.centroidX, blobData.centroidY);
////	            printf("\noffset-x: %.2f offset-y: %.2f", blobData.offsetX, blobData.offsetY);
////	            printf("\nradius: %.2f", blobData.radius);
////	            printf("\nwidth: %.2f", blobData.width);
////	            printf("\nheight: %.2f", blobData.height);
////	            printf("\nBLOB LABEL: %d", blobData.blobId);
//	            printf("\nangle: %.2f", blobData.angle);
//	        }
//
//            if (videoOn)
//            	cvShowImage("Webcam Preview", targetFrame);
//
//			// escape sequence (5ms delay)
//			if (waitKey(5) == 27)
//			{
//				quit = true;
//				break;
//			}
//		}
//
//		// cleanup
//		cvReleaseBlobs(blobs);
//		cvReleaseImage(&hsvFrame);
//		cvReleaseImage(&labelFrame);
//		cvReleaseImage(&segmentedFrame);
//		cvReleaseImage(&smoothedFrame);
//		cvReleaseImage(&targetFrame);
//		cvReleaseImage(&sourceFrame);
//
//		cvDestroyAllWindows();
//		cvReleaseCapture(&capture);
//
//		return 0;
//	}
//
//
//	/**------------------------------------------------------------------------------------
//	 * Helper method.
//	 * Fills and returns BlobData holder with calculated and retrieved data.
//	 --------------------------------------------------------------------------------------*/
//	BlobData AdvBlobDetector::packageBlobData(CvBlob* blob_p, int width, int height)
//	{
//		CvPoint origin = cvPoint(width/2, height/2);
//		CvPoint blobCentroid = cvPoint(blob_p->centroid.x, blob_p->centroid.y);
//
//		BlobData blobDataHolder;
//
//		// TODO: implement another mechanism for blob tracking, as the current one is not reliable
//		blobDataHolder.blobId = blob_p->label;
//
//		// TODO: determine mean color and map it to color codes
//		// for now: 1-red, 2-green, 3-blue, 4-orange
//		blobDataHolder.colorCode = 1;
//
//		blobDataHolder.centroidX = blob_p->centroid.x;
//		blobDataHolder.centroidY = blob_p->centroid.y;
//		blobDataHolder.offsetX = (width/2)-(blob_p->centroid.x);
//		blobDataHolder.offsetY = (height/2)-(blob_p->centroid.y);
//		blobDataHolder.width = blob_p->maxx - blob_p->minx;
//		blobDataHolder.height = blob_p->maxy - blob_p->miny;
//		blobDataHolder.radius = cvDistancePointPoint(blobCentroid, origin);
//		blobDataHolder.angle = cvAngle(blob_p)*57.2957795;
//
//		return blobDataHolder;
//	}
