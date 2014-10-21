#include"BlobData.h"

#include<cvblob.h>

#ifndef blobsinfodao_h
#define blobsinfodao_h

using namespace cvb;
using namespace std;


/*
 * BlobsInfoDao class definition.
 * Contains information about all discovered blobs.
 * Implements methods for simple blob info manipulations.
 */
class BlobsInfoDao
{
	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:

		// constructor
		BlobsInfoDao();

		// destructor
		~BlobsInfoDao();

		/**------------------------------------------------------------------------------------
		 * TODO: revise, once the database is ready.
		 * For each blob:
		 * 1. calculates required values from available blob and track info
		 * 2. converts blob and track info into BlobInfo format.
		 * 3. adds converted info to the list for persistence.
		 --------------------------------------------------------------------------------------*/
		void addBlobsInfo(CvBlobs blobs, CvTracks tracks);

		/**------------------------------------------------------------------------------------
		 * Returns a list of discovered BlobData holders.
		 --------------------------------------------------------------------------------------*/
		list<BlobData>* getBlobsInfo();

		/**------------------------------------------------------------------------------------
		 * Sets the largest blob data holder.
		 --------------------------------------------------------------------------------------*/
		void setLargestBlob(BlobData blobData);

		/**------------------------------------------------------------------------------------
		 * Returns the largest blob data holder.
		 --------------------------------------------------------------------------------------*/
		BlobData getLargestBlob();


	//-----------------------------------------------------------------------------------------
	// Private members
	//-----------------------------------------------------------------------------------------
	private:

		list<BlobData>* blobList;
		BlobData largestBlob;
};

#endif
