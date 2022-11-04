#include <unistd.h>
#include "serial.h"
#include "serial_tool.h"

using namespace std;

/* argv define 
  argv[1] = serial port
  argv[2] = baudrate
  argv[3] = send mode
  argv[4] = message
  argv[5] = loop time
*/
/* work mode define 
  workmode = 1, send ascii
  workmode = 2, send hex
*/

int main(int argc, char **argv)
{
  SerialTool serialtool;
  //param check & serial port config
  serialtool.ParamCheck(argc, argv);
  int sendmode = serialtool.PrintConfig(argv);

  if (argc == 6)
  {
    int looptime = atoi(argv[5]);
    while (1)
    {
      if (sendmode == 1)
      {
        serialtool.SendAscii(argv);
      }
      if (sendmode == 2)
      serialtool.SendHex(argv);
      usleep(looptime*1000);
    }
  }
  else
 {
    if (sendmode == 1)
      {
        serialtool.SendAscii(argv);
      }
      if (sendmode == 2)
      {
        serialtool.SendHex(argv);
      }
 }
  return 0;
}