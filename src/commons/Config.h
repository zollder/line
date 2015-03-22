/*
 * Config.h
 * Global shared configuration holder.
 */

#ifndef config_h
#define config_h

/* ----------------------------------------------------
 * Debug parameters
 ------------------------------------------------------*/
#define VIDEO_ON true
#define FPS_ON false

/* ----------------------------------------------------
 * Socket server/client parameters
 ------------------------------------------------------*/
#define PORT 5000
#define CONNECTIONS 2

/* ----------------------------------------------------
 * Thread parameters
 ------------------------------------------------------*/
#define BASE_THREAD_ID 1
#define LINE_DETECTOR_THREAD_ID 2
#define SOCKET_SERVER_THREAD_ID 3

/* ----------------------------------------------------
 * Timer parameters
 ------------------------------------------------------*/
#define DETECTOR_INTERVAL 0.50
#define SOCKET_SERVER_INTERVAL 0.50

/* ----------------------------------------------------
 * Object parameters
 ------------------------------------------------------*/
#define MAX_AREA 50000
#define MIN_AREA 2400

#endif
