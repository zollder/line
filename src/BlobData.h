
#ifndef blobdata_h
#define blobdata_h

// blob data structure declaration: single blob info holder
struct BlobData
{
	// general info
	int blobId;
	int colorCode;

	// blob's center coordinates(pixels)
	double centroidX;
	double centroidY;

	// blob's offset from the axes
	double offsetX;
	double offsetY;

	// dimensions (pixels)
	double width;
	double height;
	double radius;

	// blob's angle orientation
	double angle;

	// blob tracking parameters
	int lifetime;	// # of frames the blob exists
	int active;		// # of frames the blob is active from the last inactive period
	int inactive;	// # of frames the blob was missing
};

#endif
