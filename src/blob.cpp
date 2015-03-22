
#include <stdio.h>

// dependencies (headers)
#include"data/Data.h"
#include"data/DataService.h"
#include"core/Detector.h"
#include"core/DetectorThread.h"
#include"core/ThresholdCalibrator.h"
#include"com/SocketServer.h"
#include"com/ServerThread.h"

#include "commons/Parameters.h"
#include "commons/Config.h"

int main()
{
	Data* data = new Data();
	DataService* service = new DataService(data);
	SocketServer* socketServer = new SocketServer(PORT, CONNECTIONS, service);

	Parameters params;
	params.setCameraId(1);
	params.setFrameSize(1024, 576);
	params.setLowerHsv(55, 97, 63);
	params.setUpperHsv(179, 185, 255);

	Detector* detector = new Detector(params, service);
	DetectorThread* detectorThread = new DetectorThread(detector, LINE_DETECTOR_THREAD_ID);
	ServerThread* serverThread = new ServerThread(socketServer, SOCKET_SERVER_THREAD_ID);

//	ThresholdCalibrator* calibrator = new ThresholdCalibrator(params);
//	calibrator->startHsvCalibration();

//	serverThread->start();
	detectorThread->start();
//
//	serverThread->join();
	detectorThread->join();

	// cleanup
	delete socketServer;
	delete serverThread;
	delete detectorThread;
	delete detector;
	delete service;
	delete data;
//	delete calibrator;

	printf("\nMain thread done!\n");
}
