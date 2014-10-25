
#include "BlobDetector.h"

//---------------------------------------------------------------------------------------------
// BlobDetector class implementation.
//---------------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------
	// Constructor
	//-----------------------------------------------------------------------------------------
	BlobDetector::BlobDetector(VideoParameters *video_p, ImageParameters* image_p, BlobsInfoDao* blobsInfo_p)
	{
		printf("\nConstructing BlobDetector ...");

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
	BlobDetector::~BlobDetector()
	{
		printf("\nDestroying BlobDetector ...");
	}

	/**------------------------------------------------------------------------------------
	 * Starts HSV color space calibrator with the tracker bars for accurate color filtering.
	 ------------------------------------------------------------------------------------*/
	int BlobDetector::startHsvCalibration(bool logsOn)
	{
		CvCapture* capture;
		CvSize imageSize;
		IplImage *sourceFrame, *targetFrame, *smoothedFrame, *hsvFrame, *segmentedFrame, *labelFrame;

		CvPoint pt1, pt2, pt3, pt4, pt5, pt6;
		int picWidth, picHeight, thickness;

		CvTracks tracks;
		CvBlobs blobs;
		CvBlob* blob;

		cvNamedWindow("Webcam Preview", CV_WINDOW_AUTOSIZE);
		cvMoveWindow("Webcam Preview", 100, 100);

	    capture = cvCaptureFromCAM(videoParams->cameraId);
	    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, videoParams->captureWidth);
	    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, videoParams->captureHeight);

	    sourceFrame = cvRetrieveFrame(capture);
	    if (sourceFrame == NULL) {
	        return -1;
	    }

	    imageSize = cvGetSize(sourceFrame);
	    printf("\nFrame width (pixels): %d | Frame height (pixels): %d", imageSize.width, imageSize.height);

	    targetFrame = cvCreateImage(imageSize, sourceFrame->depth, sourceFrame->nChannels);
		smoothedFrame = cvCreateImage(imageSize, targetFrame->depth, targetFrame->nChannels);
		hsvFrame = cvCreateImage(imageSize, targetFrame->depth, targetFrame->nChannels);
		segmentedFrame = cvCreateImage(imageSize, targetFrame->depth, 1);
		labelFrame = cvCreateImage(cvGetSize(targetFrame), IPL_DEPTH_LABEL, 1);

		printf("\nFrame depth: %d | Frame channels: %d", sourceFrame->depth, sourceFrame->nChannels);

	    // ===========================================color calibrator start
	    namedWindow("Control",CV_WINDOW_AUTOSIZE);

		int iLowH = 0; int iHighH = 179;
		int iLowS = 0; int iHighS = 255;
		int iLowV = 0; int iHighV = 255;

		cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
		cvCreateTrackbar("HighH", "Control", &iHighH, 179);

		cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
		cvCreateTrackbar("HighS", "Control", &iHighS, 255);

		cvCreateTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
		cvCreateTrackbar("HighV", "Control", &iHighV, 255);
		// ===========================================color calibrator end

		while (cvGrabFrame(capture))
		{
			sourceFrame = cvRetrieveFrame(capture);
			if(!sourceFrame)
			{
				printf("\nFrame fault.");
				break;
			}

			cvFlip(targetFrame, targetFrame, 1);
			cvConvertScale(sourceFrame, targetFrame, 1, 0);
			cvSmooth(targetFrame, smoothedFrame, CV_GAUSSIAN_5x5, 5, 5);
			cvCvtColor(smoothedFrame, hsvFrame, COLOR_BGR2HSV);
			cvInRangeS(hsvFrame, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), segmentedFrame);

			cvMorphologyEx(segmentedFrame, segmentedFrame, 0, 0, CV_MOP_OPEN, 2);
			cvMorphologyEx(segmentedFrame, segmentedFrame, 0, 0, CV_MOP_CLOSE, 2);

			cvLabel(segmentedFrame, labelFrame, blobs);
			cvFilterByArea(blobs, 500, 1000000);
			cvRenderBlobs(labelFrame, blobs, targetFrame, targetFrame, CV_BLOB_RENDER_COLOR, 0.5);
			cvUpdateTracks(blobs, tracks, 200., 5);
			cvRenderTracks(tracks, targetFrame, targetFrame, CV_TRACK_RENDER_BOUNDING_BOX, NULL);

			//-----------------------------------------------------------------------------------------
			// Visualize image processing
			//-----------------------------------------------------------------------------------------

			if (logsOn)
			{
				thickness = 1;
				picWidth = targetFrame->width;
				picHeight = targetFrame->height;

				pt3 = cvPoint(0, picHeight/2);
				pt4 = cvPoint(picWidth, picHeight/2);
				cvLine(targetFrame, pt3, pt4, rgbRed, thickness);

				pt1 = cvPoint(picWidth/2, 0);
				pt2 = cvPoint(picWidth/2, picHeight);
				cvLine(targetFrame, pt1, pt2, rgbRed, thickness);

				int largestBlobIndex = cvGreaterBlob(blobs);
				if (largestBlobIndex)
				{
					blob = blobs[largestBlobIndex];
					pt5 = cvPoint(picWidth/2, picHeight/2);
					pt6 = cvPoint(blob->centroid.x, blob->centroid.y);
					cvLine(targetFrame, pt5, pt6, rgbGreen, thickness);
					cvCircle(targetFrame, pt6, 3, rgbGreen, 2, CV_FILLED, 0);

					BlobData blobData = packageBlobData(blob, picWidth, picHeight);
					this->blobsInfo->setLargestBlob(blobData);

					printf("\nBlobs found: %lu", blobs.size());
					printf("\ncenter-x: %.2f center-y: %.2f", blobData.centroidX, blobData.centroidY);
					printf("\noffset-x: %.2f offset-y: %.2f", blobData.offsetX, blobData.offsetY);
					printf("\nradius: %.2f", blobData.radius);
					printf("\nwidth: %.2f", blobData.width);
					printf("\nheight: %.2f", blobData.height);
					printf("\nBLOB LABEL: %d", blobData.blobId);
				}
			}

	        printf("\nLower threshold: H:%d, S:%d, V:%d", iLowH, iLowS, iLowV);
			printf("\nUpper threshold: H:%d, S:%d, V:%d", iHighH, iHighS, iHighV);

	        cvShowImage("Thresholded Image", segmentedFrame);
			cvShowImage("Webcam Preview", targetFrame);

			// escape sequence
			if (waitKey(10) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
				break;

		}

		// cleanup
		cvReleaseBlobs(blobs);
		cvReleaseImage(&hsvFrame);
		cvReleaseImage(&labelFrame);
		cvReleaseImage(&segmentedFrame);
		cvReleaseImage(&smoothedFrame);
		cvReleaseImage(&targetFrame);
		cvReleaseImage(&sourceFrame);

		cvDestroyAllWindows();
		cvReleaseCapture(&capture);

		return 0;
	}

	/**------------------------------------------------------------------------------------
	 * Starts blob detection in HSV color space in FPS test mode.
	 ------------------------------------------------------------------------------------*/
	int BlobDetector::startHsv(bool videoOn, bool fpsOn)
	{
		//-----------------------------fps test variables
	    time_t start, end;
	    double fps, sec;
	    int counter = 0;
	    //-----------------------------

		CvCapture* capture;				// webcam feed structure holder
		CvSize imageSize;				// current frame holder (from the camera feed)

		IplImage *sourceFrame;			// source frame
		IplImage *targetFrame;			// target frame
		IplImage *smoothedFrame;		// blurred frame
		IplImage *hsvFrame;				// frame in the HSV color space
		IplImage *segmentedFrame;		// thresholded frame holder
		IplImage *labelFrame;			// labeled blobs holder

		CvTracks tracks;	// structure to hold tracks
		CvBlobs blobs;		// structure to hold blobs
		CvBlob* blob;		// pointer to the largest blob in the list of blobs

		CvPoint pt1, pt2, pt3, pt4, pt5, pt6;
		int picWidth, picHeight, thickness;

		if (videoOn)
		{
			cvNamedWindow("Webcam Preview", CV_WINDOW_AUTOSIZE);
			cvMoveWindow("Webcam Preview", 100, 100);
		}

	    capture = cvCaptureFromCAM(videoParams->cameraId);
	    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, videoParams->captureWidth);
	    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, videoParams->captureHeight);

	    sourceFrame = cvRetrieveFrame(capture);
	    if (sourceFrame == NULL) {
	        return -1;
	    }

	    imageSize = cvGetSize(sourceFrame);
	    printf("\nFrame width (pixels): %d | Frame height (pixels): %d", imageSize.width, imageSize.height);

		targetFrame = cvCreateImage(imageSize, sourceFrame->depth, sourceFrame->nChannels);
		smoothedFrame = cvCreateImage(imageSize, targetFrame->depth, targetFrame->nChannels);
		hsvFrame = cvCreateImage(imageSize, targetFrame->depth, targetFrame->nChannels);
		segmentedFrame = cvCreateImage(imageSize, targetFrame->depth, 1);
		labelFrame = cvCreateImage(imageSize, IPL_DEPTH_LABEL, 1);

		printf("\nFrame depth: %d | Frame channels: %d", sourceFrame->depth, sourceFrame->nChannels);

		// fps test
		if (fpsOn)
			time(&start);

		bool quit = false;
		while (!quit && cvGrabFrame(capture))
		{
			sourceFrame = cvRetrieveFrame(capture);
			if(!sourceFrame)
			{
				printf("\nFrame fault.");
				break;
			}

			// fps test
			if (fpsOn)
			{
				time(&end);
				++counter;
				sec = difftime(end, start);
				fps = counter / sec;
				printf("FPS = %.2f\n", fps);
			}

			cvFlip(targetFrame, targetFrame, 1);

			// copies one array to another with optional scaling and optional type conversion
			// cvConvert can be used instead (scale=1, shift=0 no prescaling is done)
			cvConvertScale(sourceFrame, targetFrame, 1, 0);

			// smoothes the frame with Gaussian filter with 5x5 kernel (done in-place)
			// good results, but slower than other filters (see details on  pp. 111-114 of the "Learning OpenCV")
			cvSmooth(targetFrame, smoothedFrame, CV_GAUSSIAN_5x5, 5, 5);

			// converts the frame from RGB to HSV color-space
			cvCvtColor(smoothedFrame, hsvFrame, COLOR_BGR2HSV);

			// TODO: try adaptive thresholding
			// threshold the frame according to predefined lower and upper HSV limits
			cvInRangeS(hsvFrame, imageParams->getLowerLimit(), imageParams->getUpperLimit(), segmentedFrame);

			//morphological opening (removes small objects from the foreground)
			cvMorphologyEx(segmentedFrame, segmentedFrame, 0, 0, CV_MOP_OPEN, 2);

			//morphological closing (removes small holes from the foreground)
			cvMorphologyEx(segmentedFrame, segmentedFrame, 0, 0, CV_MOP_CLOSE, 2);

			// find, filter (by area) and render blobs
			cvLabel(segmentedFrame, labelFrame, blobs);
			cvFilterByArea(blobs, 500, 1000000);
			cvRenderBlobs(labelFrame, blobs, targetFrame, targetFrame, CV_BLOB_RENDER_COLOR, 0.5);

			// update and render blob tracks
			cvUpdateTracks(blobs, tracks, 200., 5);
			cvRenderTracks(tracks, targetFrame, targetFrame, CV_TRACK_RENDER_BOUNDING_BOX, NULL);

			//-----------------------------------------------------------------------------------------
			// Visualize image processing
			//-----------------------------------------------------------------------------------------

			thickness = 1;
			picWidth = targetFrame->width;
			picHeight = targetFrame->height;

			if (videoOn)
			{
				// draw x-axes
		        pt3 = cvPoint(0, picHeight / 2);
		        pt4 = cvPoint(picWidth, picHeight / 2);
		        cvLine(targetFrame, pt3, pt4, rgbRed, thickness);

		        // draw y-axes
		        pt1 = cvPoint(picWidth/2, 0);
		        pt2 = cvPoint(picWidth/2, picHeight);
		        cvLine(targetFrame, pt1, pt2, rgbRed, thickness);
			}

	        // find largest blob (biggest area).
	        int largestBlobIndex = cvGreaterBlob(blobs);
	        if (largestBlobIndex)
	        {
	            blob = blobs[largestBlobIndex];
	            pt5 = cvPoint(picWidth/2, picHeight/2);
	            pt6 = cvPoint(blob->centroid.x, blob->centroid.y);
	            cvLine(targetFrame, pt5, pt6, rgbGreen, thickness);
	            cvCircle(targetFrame, pt6, 3, rgbGreen, 2, CV_FILLED, 0);

	            BlobData blobData = packageBlobData(blob, picWidth, picHeight);
	            this->blobsInfo->setLargestBlob(blobData);

	            printf("\nBlobs found: %lu", blobs.size());
	            printf("\ncenter-x: %.2f center-y: %.2f", blobData.centroidX, blobData.centroidY);
	            printf("\noffset-x: %.2f offset-y: %.2f", blobData.offsetX, blobData.offsetY);
	            printf("\nradius: %.2f", blobData.radius);
	            printf("\nwidth: %.2f", blobData.width);
	            printf("\nheight: %.2f", blobData.height);
	            printf("\nBLOB LABEL: %d", blobData.blobId);
	        }

            if (videoOn)
            	cvShowImage("Webcam Preview", targetFrame);

			// escape sequence (5ms delay)
			if (waitKey(5) == 27)
			{
				quit = true;
				break;
			}
		}

		// cleanup
		cvReleaseBlobs(blobs);
		cvReleaseImage(&hsvFrame);
		cvReleaseImage(&labelFrame);
		cvReleaseImage(&segmentedFrame);
		cvReleaseImage(&smoothedFrame);
		cvReleaseImage(&targetFrame);
		cvReleaseImage(&sourceFrame);

		cvDestroyAllWindows();
		cvReleaseCapture(&capture);

		return 0;
	}


	/**------------------------------------------------------------------------------------
	 * Helper method.
	 * Fills and returns BlobData holder with calculated and retrieved data.
	 --------------------------------------------------------------------------------------*/
	BlobData BlobDetector::packageBlobData(CvBlob* blob_p, int width, int height)
	{
		CvPoint origin = cvPoint(width/2, height/2);
		CvPoint blobCentroid = cvPoint(blob_p->centroid.x, blob_p->centroid.y);

		BlobData blobDataHolder;

		// TODO: implement another mechanism for blob tracking, as the current one is not reliable
		blobDataHolder.blobId = blob_p->label;

		// TODO: determine mean color and map it to color codes
		// for now: 1-red, 2-green, 3-blue, 4-orange
		blobDataHolder.colorCode = 1;

		blobDataHolder.centroidX = blob_p->centroid.x;
		blobDataHolder.centroidY = blob_p->centroid.y;
		blobDataHolder.offsetX = (width/2)-(blob_p->centroid.x);
		blobDataHolder.offsetY = (height/2)-(blob_p->centroid.y);
		blobDataHolder.width = blob_p->maxx - blob_p->minx;
		blobDataHolder.height = blob_p->maxy - blob_p->miny;
		blobDataHolder.radius = cvDistancePointPoint(blobCentroid, origin);
		blobDataHolder.angle = cvAngle(blob_p);

		return blobDataHolder;
	}
