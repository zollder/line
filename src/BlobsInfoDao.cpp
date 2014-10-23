
#include "BlobsInfoDao.h"

//---------------------------------------------------------------------------------------------
// BlobsInfo class implementation.
//---------------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------
	// Constructor
	//-----------------------------------------------------------------------------------------
	BlobsInfoDao::BlobsInfoDao()
	{
		printf("\nConstructing BlobsInfoDao ...");
		blobList = new list<BlobData>();
	}

	//-----------------------------------------------------------------------------------------
	// Destructor
	//-----------------------------------------------------------------------------------------
	BlobsInfoDao::~BlobsInfoDao()
	{
		printf("\nDestroying BlobsInfoDao ...");

		blobList->clear();
		delete blobList;
	}

	/**
	 * TODO: revise, once the database is ready.
	 * For each blob:
	 * 1. calculates required values from available blob and track info
	 * 2. converts blob and track info into BlobInfo format.
	 * 3. adds converted info to the list for persistence.
	 */
	void BlobsInfoDao::addBlobsInfo(CvBlobs blobs, CvTracks tracks)
	{
		// TODO: design & implement
		blobList->clear();
	}

	/** Returns a pointer to a list BlobData holders. */
	list<BlobData>* BlobsInfoDao::getBlobsInfo()
	{
		return blobList;
	}

	/** Stores a pointer to a largest blob in a local BlobData instance. */
	void BlobsInfoDao::setLargestBlob(BlobData data)
	{
		this->largestBlob = data;
	}

	/** Returns a pointer to largest blob instance. */
	BlobData BlobsInfoDao::getLargestBlob()
	{
		return largestBlob;
	}
