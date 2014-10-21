
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
	imageParams->setLowerRgb(170, 0, 0);
	imageParams->setUpperRgb(255, 100, 100);

	BlobDetector* blobDetector = new BlobDetector(videoParams, imageParams, blobsInfoDao);
	blobDetector->startVideoRgb();

	// cleanup
	delete blobDetector;
	delete videoParams;
	delete imageParams;
	delete blobsInfoDao;

	printf("\nMain thread done!\n");
}
