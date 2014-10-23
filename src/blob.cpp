
#include <stdio.h>

// dependencies (headers)
#include"VideoParameters.h"
#include"ImageParameters.h"
#include"BlobDetector.h"

int main()
{
	VideoParameters* videoParams = new VideoParameters(1, 1920, 1080);
	ImageParameters* imageParams = new ImageParameters();
	BlobsInfoDao* blobsInfoDao = new BlobsInfoDao();

	// color ranges for orange
	imageParams->setLowerHsv(0, 100, 150);
	imageParams->setUpperHsv(25, 255, 255);

//	imageParams->setLowerRgb(150, 0, 0);
//	imageParams->setUpperRgb(255, 100, 100);

	BlobDetector* blobDetector = new BlobDetector(videoParams, imageParams, blobsInfoDao);
	blobDetector->startHsv(false);
//	blobDetector->startRgb(true);

	// cleanup
	delete blobDetector;
	delete videoParams;
	delete imageParams;
	delete blobsInfoDao;

	printf("\nMain thread done!\n");
}
