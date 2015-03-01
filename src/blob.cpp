
#include <stdio.h>

// dependencies (headers)
#include"data/Data.h"
#include"data/DataService.h"
#include"core/Detector.h"
#include"core/DetectorThread.h"
#include"core/ThresholdCalibrator.h"

#include "commons/Parameters.h"
#include "commons/Config.h"

int main()
{
	Data* data = new Data();
	DataService* service = new DataService(data);

//	lower: 70 30 116
//	upper: 116 255 255

	Parameters params;
	params.setCameraId(1);
	params.setFrameSize(1280, 720);
	params.setLowerHsv(90, 36, 131);
	params.setUpperHsv(116, 255, 255);

	Detector* detector = new Detector(params, service);
	DetectorThread* detectorThread = new DetectorThread(detector, LINE_DETECTOR_THREAD_ID);

//	ThresholdCalibrator* calibrator = new ThresholdCalibrator(params);
//	calibrator->startHsvCalibration();

	detectorThread->start();
	detectorThread->join();

	// cleanup
	delete detectorThread;
	delete detector;
	delete service;
	delete data;

	printf("\nMain thread done!\n");
}
