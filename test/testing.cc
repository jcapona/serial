#include <serial.h>
#include <iostream>
#include <string>

static void echo(serial &s)
{
    std::string msg;
    while (s.isConnected()) {
        s.read(msg);
        std::cout << msg << "\n";
    }
}

int main(int argc, char **argv)
{
    if (argc < 3) {
        std::cout << "Usage:\n" << argv[0] << " [port] [baudrate]\n\n";
        return 0;
    }

    try {
        serial s(argv[1], atoi(argv[2]), '\n');
        echo(s);
    }
    catch (const std::exception &e) {
        std::cout << e.what() << " " << argv[1] << "\n";
        return -1;
    }

    return 0;
}
