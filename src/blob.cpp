
#include <stdio.h>

// dependencies (headers)
#include"VideoParameters.h"
#include"ImageParameters.h"
#include"BlobDetector.h"

int main()
{
	VideoParameters* videoParams = new VideoParameters(1, 1600, 900);
	ImageParameters* imageParams = new ImageParameters();

	// color ranges for orange
	imageParams->setLowerRgb(170, 0, 0);
	imageParams->setUpperRgb(255, 100, 100);

	BlobDetector* blobDetector = new BlobDetector(videoParams, imageParams);
	blobDetector->startRgb();

	// cleanup
	delete blobDetector;
	delete videoParams;
	delete imageParams;

	printf("\nMain thread done!\n");
}
