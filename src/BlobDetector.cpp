
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
		converter = new ColorConverter();

		width = video_p->captureWidth;
		height = video_p->captureHeight;

		lowerThreshold = image_p->getLowerThreshold();
		upperThreshold = image_p->getUpperThreshold();

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
		delete converter;
	}

	/**------------------------------------------------------------------------------------
	 * Starts blob detection procedure.
	 * TODO: should be running in a dedicated thread, with available handle for process control
	 ------------------------------------------------------------------------------------*/
	void BlobDetector::startHsv()
	{
		// TODO: move structure initializations into constructor

		CvCapture* capture;				// webcam feed structure holder
		IplImage *sourceFrame;			// source frame holder
		IplImage *targetFrame;			// target frame holder
		IplImage *hsvFrame;				// HSV color space holder
		IplImage *thresholdedFrame;		// thresholded frame holder
		IplImage *labelImage;			// labeled blobs holder
		CvBlobs blobs;					// structure to hold blobs

		capture = cvCreateCameraCapture(videoParams->cameraId);
		targetFrame = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
		hsvFrame = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
		thresholdedFrame = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
		labelImage = cvCreateImage(cvSize(width, height), IPL_DEPTH_LABEL, 1);

		// screen resolution
		int screenx = 1600;
		int screeny = 900;

		//Windows
		cvNamedWindow("Live",CV_WINDOW_AUTOSIZE);

		while (1)
		{

			sourceFrame = cvQueryFrame(capture);

			// leave the loop if failed to get the frame
			if(!sourceFrame)
			{
				cout << "Frame fault." << endl;
				break;
			}

			// resize the capture (last argument <=> sub-pixel interpolation method)
			cvResize(sourceFrame, targetFrame, CV_INTER_LINEAR);

			// flip the target frame
			cvFlip(targetFrame, targetFrame, 1);

			// convert to HSV color-space
			cvCvtColor(targetFrame, hsvFrame, CV_BGR2HSV);

			// threshold converted frame for yellow color
			cvInRangeS(hsvFrame, lowerThreshold, upperThreshold, thresholdedFrame);

			// smooth the image by applying median filter
			cvSmooth(thresholdedFrame, thresholdedFrame, CV_MEDIAN, 7, 7);

			// find the blob
			cvLabel(thresholdedFrame, labelImage, blobs);

			// render the blobs
			cvRenderBlobs(labelImage, blobs, targetFrame, targetFrame);

			// filter rendered blobs
			cvFilterByArea(blobs, 60, 500);

			CvBlobs::const_iterator blob;
			for (blob = blobs.begin(); blob != blobs.end(); ++blob)
			{
				double moment10 = blob->second->m10;
				double moment01 = blob->second->m01;
				double area = blob->second->area;

				// calculate current blob position
				int x1 = moment10/area;
				int y1 = moment01/area;

				// map blob position to screen coordinates
				int x = (int) (x1*screenx/width);
				int y = (int) (y1*screeny/height);

				// print position info
				cout << "x: " << x << "y: " << y << endl;
			}

			// show the process
			cvShowImage("live", targetFrame);

			// escape sequence
			char click = cvWaitKey(33);
			if (click == 27)
				break;
		}

		// cleanup
		cvReleaseBlobs(blobs);
		cvReleaseCapture(&capture);
		cvDestroyAllWindows();
	}

	/**------------------------------------------------------------------------------------
	 * Starts blob detection in HSV color space, with blob tracking visuals (video).
	 ------------------------------------------------------------------------------------*/
	int BlobDetector::startHsv(bool videoOn)
	{
		//-----------------------------fps test variables
	    time_t start, end;
	    double fps, sec;
	    int counter = 0;
	    //-----------------------------

		CvCapture* capture;				// webcam feed structure holder
		CvSize imageSize;				// current frame holder (from the camera feed)

		IplImage *sourceFrame;			// source frame holder
		IplImage *targetFrame;			// target frame holder
		IplImage *smoothedFrame;		// segmented color space holder
		IplImage *hsvFrame;
		IplImage *segmentedFrame;		// thresholded frame holder
		IplImage *labelFrame;			// labeled blobs holder
		IplImage *colorRange;			// thresholded frame holder

		CvPoint pt1, pt2, pt3, pt4, pt5, pt6;
		int picWidth, picHeight, thickness;

		CvTracks tracks;
		CvBlobs blobs;					// structure to hold blobs
		CvBlob* blob;

		unsigned int result = 0;
		bool quit = false;

		if (videoOn)
		{
			cvNamedWindow("Processed Video Frames", CV_WINDOW_AUTOSIZE);
			cvMoveWindow("Processed Video Frames", 600, 100);
			cvNamedWindow("Webcam Preview", CV_WINDOW_AUTOSIZE);
			cvMoveWindow("Webcam Preview", 100, 100);
			cvNamedWindow("Color Range", CV_WINDOW_AUTOSIZE);
			cvMoveWindow("Color Range", 100, 600);
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
		segmentedFrame = cvCreateImage(imageSize, targetFrame->depth, 1);
		labelFrame = cvCreateImage(cvGetSize(targetFrame), IPL_DEPTH_LABEL, 1);
		colorRange = cvCreateImage(cvSize(200, 100), 8, 3);

		printf("\nFrame depth: %d | Frame channels: %d", sourceFrame->depth, sourceFrame->nChannels);

		time(&start);
		while (!quit && cvGrabFrame(capture))
		{
			// leave the loop if failed to get the frame
			sourceFrame = cvRetrieveFrame(capture);
			if(!sourceFrame)
			{
				printf("\nFrame fault.");
				break;
			}

			// ---------------------------fps test
	        time(&end);
	        ++counter;
	        sec = difftime(end, start);
	        fps = counter / sec;
	        printf("FPS = %.2f\n", fps);
	        // ---------------------------

			// flip the target frame
			cvFlip(targetFrame, targetFrame, 1);

			// copies one array to another with optional scaling and optional type conversion
			// cvConvert can be used instead (scale=1, shift=0 no prescaling is done)
			cvConvertScale(sourceFrame, targetFrame, 1, 0);

			// TODO: try bilateral filter (preserves edges)
			// smoothes the frame with Gaussian filter with 5x5 kernel (done in-place)
			// good results, but slower than other filters (see details on  pp. 111-114 of the "Learning OpenCV")
			cvSmooth(targetFrame, smoothedFrame, CV_BILATERAL, 5, 5);

			// convert to HSV color-space
			hsvFrame = converter->convertRgbToHsv(smoothedFrame);

			// applying opening morphology filter
			cvMorphologyEx(hsvFrame, hsvFrame, 0, 0, CV_MOP_CLOSE, 5);

			// TODO: try adaptive thresholding
			// allocate image structure for thresholded image (params: size, channel data type, # of channels)
			// only one channel is required per color
			cvInRangeS(hsvFrame, lowerThreshold, upperThreshold, segmentedFrame);

			// find blobs (frame-in, frame-out, list of blobs)
			// clean up and render blobs & tracks
			result = cvLabel(segmentedFrame, labelFrame, blobs);
			cvFilterByArea(blobs, 500, 1000000);
			cvRenderBlobs(labelFrame, blobs, targetFrame, targetFrame, CV_BLOB_RENDER_COLOR, 0.5);
			cvUpdateTracks(blobs, tracks, 200., 5);
			cvRenderTracks(tracks, targetFrame, targetFrame, CV_TRACK_RENDER_BOUNDING_BOX, NULL);

			//-----------------------------------------------------------------------------------------
			// Visualize image processing
			//-----------------------------------------------------------------------------------------

			thickness = 1;
	        picWidth = targetFrame->width;
	        picHeight = targetFrame->height;

	        // draw x-axes
	        pt3 = cvPoint(0, picHeight / 2);
	        pt4 = cvPoint(picWidth, picHeight / 2);
	        cvLine(targetFrame, pt3, pt4, rgbRed, thickness);

	        // draw y-axes
	        pt1 = cvPoint(picWidth/2, 0);
	        pt2 = cvPoint(picWidth/2, picHeight);
	        cvLine(targetFrame, pt1, pt2, rgbRed, thickness);

	        if (videoOn)
	        {
				// Show color range values in separate window
				cvZero(colorRange);
				cvRectangle(colorRange, cvPoint(0, 0), cvPoint(100, 100),lowerThreshold, CV_FILLED);
				cvRectangle(colorRange, cvPoint(100, 0), cvPoint(200, 100),upperThreshold, CV_FILLED);
				cvShowImage("Color Range", colorRange);

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


	            if (videoOn)
	            {
	            	cvShowImage("Webcam Preview", targetFrame);
	            	// cvShowImage("Processed Video Frames", colorRange);
	            }

	            BlobData blobData = packageBlobData(blob, picWidth, picHeight);
	            this->blobsInfo->setLargestBlob(blobData);

	            printf("\nBlobs found: %lu", blobs.size());
	            printf("\nPixels labeled: %d", result);
	            printf("\ncenter-x: %.2f center-y: %.2f", blobData.centroidX, blobData.centroidY);
	            printf("\noffset-x: %.2f offset-y: %.2f", blobData.offsetX, blobData.offsetY);
	            printf("\nradius: %.2f", blobData.radius);
	            printf("\nwidth: %.2f", blobData.width);
	            printf("\nheight: %.2f", blobData.height);
	            printf("\nBLOB LABEL: %d", blobData.blobId);
	        }

			// escape sequence
			char click = cvWaitKey(33);
			if (click == 27)
			{
				quit = true;
				break;
			}

			cvReleaseImage(&hsvFrame);
		}

		// cleanup
		cvReleaseBlobs(blobs);
		cvReleaseImage(&colorRange);
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
	 * Starts blob detection in RGB color space, with blob tracking visuals (video).
	 ------------------------------------------------------------------------------------*/
	int BlobDetector::startRgb(bool videoOn)
	{
		CvCapture* capture;				// webcam feed structure holder
		CvSize imageSize;				// current frame holder (from the camera feed)

		IplImage *sourceFrame;			// source frame holder
		IplImage *targetFrame;			// target frame holder
		IplImage *smoothedFrame;		// segmented color space holder
		IplImage *rgbFrame;				// thresholded frame holder
		IplImage *labelFrame;			// labeled blobs holder
		IplImage *colorRange;			// thresholded frame holder

		CvPoint pt1, pt2, pt3, pt4, pt5, pt6;
		int picWidth, picHeight, thickness;

		CvTracks tracks;
		CvBlobs blobs;					// structure to hold blobs
		CvBlob* blob;

		unsigned int result = 0;
		bool quit = false;

	    cvNamedWindow("Processed Video Frames", CV_WINDOW_AUTOSIZE);
	    cvMoveWindow("Processed Video Frames", 600, 100);
	    cvNamedWindow("Webcam Preview", CV_WINDOW_AUTOSIZE);
	    cvMoveWindow("Webcam Preview", 100, 100);
	    cvNamedWindow("Color Range", CV_WINDOW_AUTOSIZE);
	    cvMoveWindow("Color Range", 100, 600);

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
		rgbFrame = cvCreateImage(imageSize, targetFrame->depth, 1);
		labelFrame = cvCreateImage(cvGetSize(targetFrame), IPL_DEPTH_LABEL, 1);
		colorRange = cvCreateImage(cvSize(200, 100), 8, 3);

		printf("\nFrame depth: %d | Frame channels: %d", sourceFrame->depth, sourceFrame->nChannels);

		while (!quit && cvGrabFrame(capture))
		{
			// leave the loop if failed to get the frame
			sourceFrame = cvRetrieveFrame(capture);
			if(!sourceFrame)
			{
				printf("\nFrame fault.");
				break;
			}

			// flip the target frame
			cvFlip(targetFrame, targetFrame, 1);

			// copies one array to another with optional scaling and optional type conversion
			// cvConvert can be used instead (scale=1, shift=0 no prescaling is done)
			cvConvertScale(sourceFrame, targetFrame, 1, 0);

			// TODO: try bilateral filter (preserves edges)
			// smoothes the frame with Gaussian filter with 5x5 kernel (done in-place)
			// good results, but slower than other filters (see details on  pp. 111-114 of the "Learning OpenCV")
			cvSmooth(targetFrame, smoothedFrame, CV_GAUSSIAN, 5, 5);


			// TODO: try adaptive thresholding
			// allocate image structure for thresholded image (params: size, channel data type, # of channels)
			// only one channel is required per color
			cvInRangeS(smoothedFrame, lowerThreshold, upperThreshold, rgbFrame);


			// find blobs (frame-in, frame-out, list of blobs)
			// clean up and render blobs & tracks
			result = cvLabel(rgbFrame, labelFrame, blobs);
			cvFilterByArea(blobs, 500, 1000000);
			cvRenderBlobs(labelFrame, blobs, targetFrame, targetFrame, CV_BLOB_RENDER_COLOR, 0.5);
			cvUpdateTracks(blobs, tracks, 200., 5);
			cvRenderTracks(tracks, targetFrame, targetFrame, CV_TRACK_RENDER_BOUNDING_BOX, NULL);

			//-----------------------------------------------------------------------------------------
			// Visualize image processing
			//-----------------------------------------------------------------------------------------

			thickness = 1;
	        picWidth = targetFrame->width;
	        picHeight = targetFrame->height;

	        // draw x-axes
	        pt3 = cvPoint(0, picHeight / 2);
	        pt4 = cvPoint(picWidth, picHeight / 2);
	        cvLine(targetFrame, pt3, pt4, rgbRed, thickness);

	        // draw y-axes
	        pt1 = cvPoint(picWidth/2, 0);
	        pt2 = cvPoint(picWidth/2, picHeight);
	        cvLine(targetFrame, pt1, pt2, rgbRed, thickness);

	        cvShowImage("Webcam Preview", targetFrame);
	        if (videoOn)
	        {
		        cvShowImage("Processed Video Frames", rgbFrame);

				// Show color range values in separate window
		        cvZero(colorRange);
		        cvRectangle(colorRange, cvPoint(0, 0), cvPoint(100, 100),lowerThreshold, CV_FILLED);
		        cvRectangle(colorRange, cvPoint(100, 0), cvPoint(200, 100),upperThreshold, CV_FILLED);
		        cvShowImage("Color Range", colorRange);
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

	            cvShowImage("Webcam Preview", targetFrame);

	            if (videoOn)
	            	cvShowImage("Processed Video Frames", colorRange);

	            BlobData blobData = packageBlobData(blob, picWidth, picHeight);
	            this->blobsInfo->setLargestBlob(blobData);

	            printf("\nBlobs found: %lu", blobs.size());
	            printf("\nPixels labeled: %d", result);
	            printf("\ncenter-x: %.2f center-y: %.2f", blobData.centroidX, blobData.centroidY);
	            printf("\noffset-x: %.2f offset-y: %.2f", blobData.offsetX, blobData.offsetY);
	            printf("\nradius: %.2f", blobData.radius);
	            printf("\nwidth: %.2f", blobData.width);
	            printf("\nheight: %.2f", blobData.height);
	            printf("\nBLOB LABEL: %d", blobData.blobId);
	        }

			// escape sequence
			char click = cvWaitKey(33);
			if (click == 27)
			{
				quit = true;
				break;
			}
		}

		// cleanup
		cvReleaseBlobs(blobs);
		cvReleaseImage(&labelFrame);
		cvReleaseImage(&rgbFrame);
		cvReleaseImage(&colorRange);
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
