#include "serial.h"

/**
    Constructor
*/
serial ::serial(std::string dev, int baud, char endOfLine) {
    device = dev;
    baudrate = baud;
    eol = endOfLine;

    if (connect() == -1)
        throw std::runtime_error("[Serial] Error opening port\n");

    setParameters();
}

/**
    Destructor
*/
serial::~serial() {
    disconnect();
}

/**
    Sets port parameters
*/
bool serial::setParameters() {
    struct termios terminalAttributes;
    memset(&terminalAttributes, 0, sizeof(terminalAttributes));

    terminalAttributes.c_cflag = CREAD | CLOCAL | CS8;// Control flags
    terminalAttributes.c_iflag = INPCK;// Input flags - enable parity checking
    terminalAttributes.c_oflag = 0;// Output flags
    terminalAttributes.c_lflag = 0;// Local Modes flags

    terminalAttributes.c_cc[VTIME] = 10;// Read timeout, in tenths of a second
    terminalAttributes.c_cc[VMIN] = 0;// Minimum characters to read

    switch( baudrate )
    {
        case 50:
            terminalAttributes.c_cflag |= B50;
            break;
        case 75:
            terminalAttributes.c_cflag |= B75;
            break;
        case 110:
            terminalAttributes.c_cflag |= B110;
            break;
        case 134:
            terminalAttributes.c_cflag |= B134;
            break;
        case 150:
            terminalAttributes.c_cflag |= B150;
            break;
        case 200:
            terminalAttributes.c_cflag |= B200;
            break;
        case 300:
            terminalAttributes.c_cflag |= B300;
            break;
        case 600:
            terminalAttributes.c_cflag |= B600;
            break;
        case 1200:
            terminalAttributes.c_cflag |= B1200;
            break;
        case 1800:
            terminalAttributes.c_cflag |= B1800;
            break;
        case 2400:
            terminalAttributes.c_cflag |= B2400;
            break;
        case 4800:
            terminalAttributes.c_cflag |= B4800;
            break;
        case 9600:
            terminalAttributes.c_cflag |= B9600;
            break;
        case 19200:
            terminalAttributes.c_cflag |= B19200;
            break;
        case 38400:
            terminalAttributes.c_cflag |= B38400;
            break;
        case 57600:
            terminalAttributes.c_cflag |= B57600;
            break;
        case 115200:
            terminalAttributes.c_cflag |= B115200;
            break;
        case 230400:
            terminalAttributes.c_cflag |= B230400;
            break;
        default:
            terminalAttributes.c_cflag |= B115200;
    }

    return !tcsetattr(fd, TCSANOW, &terminalAttributes);  // Set options
}

int serial::connect() {
    fd = open(device.c_str(), O_RDWR | O_NOCTTY );

    if(fd == -1)
        return -1;

    return 0;
}

void serial::disconnect()
{
    clear();
    close(fd);
}

/**
    Sends message to serial port
*/
int serial::sendString(std::string &message) {
    if (!isConnected()) {
        //std::cerr << "serial::sendString: Device not connected\n";
        return -1;
    } else {
        int n = write(fd, ( char*) message.c_str(), strlen(message.c_str()));
        //std::cout << "serial::sendString: Message: " << message <<"\n";
        //clear();
        return n;
    }
}
 
/**
    Reads from serial port
*/
int serial::readString (std::string &msg) {
    if (!isConnected()) {
        //std::cerr << "serial::readString: Device not connected\n";
        return -1;
    }

    unsigned char c=0x0D;
    while (c!=eol) {
        if (read(fd, &c, sizeof(char)) > 0 && c!= 0x0A && c!= 0x0D && c!= '>') {
            msg += c;
            //std::cout << "serial::readString: Received: " << c << "\n"; // Debug
        }
    }

    return msg.size();
}

/**
    Flushes port
*/
void serial::clear() {
    tcflush(fd, TCIFLUSH);
    tcflush(fd, TCOFLUSH);
}

/**
    Returns connection state
*/
bool serial::isConnected() {
 return (fd != -1);
}
