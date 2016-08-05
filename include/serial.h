#ifndef SERIALPORT_H_
#define SERIALPORT_H_

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <stdexcept>
#include <iostream>
#include <string>

#define TIMEOUT_US 400000

class serial
{
    public:
        serial(std::string dev, int baud, char endOfLine);
        ~serial();
        int connect ();
        bool setParameters();
        void disconnect(void);
        int sendString(std::string &message);
        int readString (std::string &message);
        bool isConnected();

   private:
        int fd;
        std::string device;
        int baudrate;
        char eol;
        void clear();
};

#endif /* SERIALPORT_H_ */
