/*
 * Config.h
 * Global shared configuration holder.
 */

#ifndef config_h
#define config_h

/* ----------------------------------------------------
 * Socket server/client parameters
 ------------------------------------------------------*/
#define PORT 5000
#define CONNECTIONS 2

/* ----------------------------------------------------
 * Thread parameters
 ------------------------------------------------------*/
#define LINE_DETECTOR_THREAD_ID 1
#define SOCKET_SERVER_THREAD_ID 2

/* ----------------------------------------------------
 * Timer parameters
 ------------------------------------------------------*/
#define DETECTOR_INTERVAL 0.50
#define SOCKET_SERVER_INTERVAL 0.50

#endif
