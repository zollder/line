/*
 * Logger.h
 *
 *  Created on: Feb 5, 2015
 */

#ifndef SRC_SYS_LOGGER_H_
#define SRC_SYS_LOGGER_H_

#include <syslog.h>

class Logger {

public:

	Logger( char * );
	~Logger();

	void error( char *);
	void info( char * );
	void notice( char * );

private:


};



#endif /* SRC_SYS_LOGGER_H_ */
