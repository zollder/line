
#include <stdio.h>

// dependencies (headers)
#include"VideoParameters.h"
#include"ImageParameters.h"
#include"BlobDetector.h"

int main()
{
	VideoParameters* videoParams = new VideoParameters(1, 800, 600);
	ImageParameters* imageParams = new ImageParameters();
	BlobsInfoDao* blobsInfoDao = new BlobsInfoDao();

	imageParams->setLowerHsv(128, 124, 89);
	imageParams->setUpperHsv(179, 255, 255);

	BlobDetector* blobDetector = new BlobDetector(videoParams, imageParams, blobsInfoDao);
//	blobDetector->startHsvCalibration(false);
	blobDetector->startHsv(false, true);

	// cleanup
	delete blobDetector;
	delete videoParams;
	delete imageParams;
	delete blobsInfoDao;

	printf("\nMain thread done!\n");
}
