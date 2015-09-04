#ifndef SERIALPORT_H_
#define SERIALPORT_H_

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h> // File control definitions
#include <errno.h> // Error number definitions
#include <termios.h> // POSIX terminal control definitions
#include <sys/ioctl.h>
#include <stdexcept>
#include <iostream>
#include <string>


#define TIMEOUT_US 400000

class serial
{
 private:
  int fileDescriptor;
  std::string device;
  int baudrate;
  char eol;

 public:
 	serial(std::string dev, int baud, char endOfLine);
 	serial();
 	~serial();
  int connect ();
  bool setParameters();
 	void disconnect(void);

 	int sendString(std::string &message);
 	int readString (std::string &message);
 	bool isConnected(); 
	 void clear();
};
 
#endif /* SERIALPORT_H_ */

