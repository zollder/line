/*
 *	DataService.h
 *  Created on: 15.02.2015
 *	Author: zollder
 */

#include "Data.h"

#ifndef blobdataservice_h
#define blobdataservice_h

//-----------------------------------------------------------------------------------------
// DataService interface.
//-----------------------------------------------------------------------------------------
class DataService
{
	//-----------------------------------------------------------------------------------------
	// Public members
	//-----------------------------------------------------------------------------------------
	public:

		// constructor
		DataService(Data* data_p)
		{ data = data_p; }

		// destructor
		~DataService() {}

		float* getData()
		{
			// copy data into array
			this->copyData();
			return dataHolder;
		}

		int getSize()
		{
			return sizeof(dataHolder);
		}

		Data* data;

	private:

		float dataHolder[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

		void copyData()
		{
			data->mutex.lock();
				dataHolder[0] = 1.0;
				dataHolder[1] = (float) data->objectId;
				dataHolder[2] = (float) data->colorCode;
				dataHolder[3] = data->offsetX;
				dataHolder[4] = data->offsetY;
				dataHolder[5] = data->angle;
				dataHolder[6] = data->radius;
				dataHolder[7] = data->width;
				dataHolder[8] = data->height;
			data->mutex.unlock();
		}
};

#endif
