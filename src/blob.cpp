
#include <stdio.h>

// dependencies (headers)
#include"VideoParameters.h"
#include"ImageParameters.h"
#include"BlobDetector.h"
#include"AdvBlobDetector.h"

int main()
{
	VideoParameters* videoParams = new VideoParameters(0, 1366, 768);
	ImageParameters* imageParams = new ImageParameters();
	BlobsInfoDao* blobsInfoDao = new BlobsInfoDao();

	imageParams->setLowerHsv(50, 66, 150);
	imageParams->setUpperHsv(115, 255, 255);

	BlobDetector* blobDetector = new BlobDetector(videoParams, imageParams, blobsInfoDao);
	AdvBlobDetector* detector = new AdvBlobDetector(videoParams, imageParams, blobsInfoDao);
	blobDetector->startHsvCalibration(false);
//	blobDetector->startHsv(true, true);
//	detector->startHsv(true, true);

	// cleanup
	delete detector;
	delete blobDetector;
	delete videoParams;
	delete imageParams;
	delete blobsInfoDao;

	printf("\nMain thread done!\n");
}
