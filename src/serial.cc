#include <serial.h>

#ifdef DEBUG
#define DEBUG 1
#else
#define DEBUG 0
#endif

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <string>

extern "C" {
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
}

class serial::impl {
    public:
        impl(const std::string &dev, int baud, char eol);
        ~impl();
        void connect();
        void disconnect();
        void read(std::string &msg);
        void write(const std::string &msg);
        bool isConnected();

   private:
        void setParameters();
        void clear();
        std::string m_dev;
        int m_baud;
        char m_eol;
        int m_fd;
        bool m_isSet;
};

/**
    Constructor
*/
serial::impl::impl(const std::string &dev, int baud, char eol)
    : m_dev(dev)
    , m_baud(baud)
    , m_eol(eol)
    , m_fd(-1)
    , m_isSet(false)
{
    try {
        connect();
        setParameters();
    } catch (const std::runtime_error &e) {
        std::cout << e.what() << "\n";
    }

    if(DEBUG)
        std::cout << "[serial : serial] Connected to " << dev << " @ " << baud << " kbps\n";
}

/**
    Destructor
*/
serial::impl::~impl() {
    disconnect();
}

/**
    Sets port parameters
*/
void serial::impl::setParameters() {
    struct termios terminalAttributes;
    memset(&terminalAttributes, 0, sizeof(terminalAttributes));

    terminalAttributes.c_cflag = CREAD | CLOCAL | CS8; // Control flags
    terminalAttributes.c_iflag = INPCK; // Input flags - enable parity checking
    terminalAttributes.c_oflag = 0; // Output flags
    terminalAttributes.c_lflag = 0; // Local Modes flags
    terminalAttributes.c_cc[VTIME] = 10; // Read timeout, in tenths of a second
    terminalAttributes.c_cc[VMIN] = 0; // Minimum characters to read

    switch(m_baud) {
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

    if (tcsetattr(m_fd, TCSANOW, &terminalAttributes) == 1)
        throw std::runtime_error("Error, can't set parameters");
}

void serial::impl::connect() {
    m_fd = open(m_dev.c_str(), O_RDWR | O_NOCTTY );

    if(m_fd == -1)
        throw std::runtime_error("Error, could not open serial port");
}

void serial::impl::disconnect() {
    clear();
    close(m_fd);
}

/**
    Sends message to serial port
*/
void serial::impl::write(const std::string &msg) {
    if (!isConnected())
        throw std::logic_error("Error, port not opened");

    ::write(m_fd, ( char*) msg.c_str(), strlen(msg.c_str()));

    if(DEBUG)
        std::cout << "[serial : write] " << msg << "\n";
}

/**
    Reads from serial port
*/
void serial::impl::read(std::string &msg) {
    if (!isConnected())
        throw std::logic_error("Error, port not opened");

    msg.clear();
    unsigned char c=0x0D;
    while (c != m_eol) {
        if (::read( m_fd, &c, sizeof(char)) > 0 && c!= 0x0A && c!= 0x0D && c!= '>')
            msg += c;
    }

    if(DEBUG)
        std::cout << "[serial : read] " << msg << "\n";
}

/**
    Flushes port
*/
void serial::impl::clear() {
    tcflush(m_fd, TCIFLUSH);
    tcflush(m_fd, TCOFLUSH);
}

/**
    Returns connection state
*/
bool serial::impl::isConnected() {
    return (m_fd != -1);
}


/**
    Public api
*/

serial::serial(const std::string &dev, int baud, char eol)
    : m_impl(new serial::impl(dev, baud, eol)) {
}

serial::~serial() {
}

void serial::connect() {
    m_impl->connect();
}

void serial::disconnect() {
    m_impl->disconnect();
}

void serial::write(const std::string &msg) {
    m_impl->write(msg);
}

void serial::read(std::string &msg) {
    m_impl->read(msg);
}

bool serial::isConnected() {
    return m_impl->isConnected();
}
