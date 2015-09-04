º//Test /dev/cu.usbserial
#include <serial.h>

int main(int argc, char *argv[])
{
 if(argc != 3 )
 {
  std::cerr << "Error: enter parameters.\n";
  return 1;
 }

 serial a(argv[1], 115200,'>');
 a.clear();
 std::string msg = "ATE0\r\n";
 a.sendString(msg);

 std::string rcv;
 a.readString(rcv);
 std::cout << "rx ... " << rcv << "\n";

 return 0;
}

