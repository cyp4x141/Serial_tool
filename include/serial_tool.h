#ifndef SERIAL_TOOL_H
#define SERIAL_TOOL_H

//serail data define
#define MSG_LENGTH                0x08

#include <iostream>
#include <sstream>
#include <regex>
using namespace std;


class SerialTool
{
public:

void ParamCheck(int param_num, char** config)
{
  //param check
  if (param_num < 5)
  {
    printf("Param Error! Please input param : SerialSend ${Port} ${Baudrate} ${Sendmode} ${Message} $[Looptime]\n");
    printf("Port should be set as /dev/tty*\n");
    printf("Sendmode should be set as ascii/hex\n");
    printf("Looptime can set a send loop time(ms), default is send once\n");
    abort();
  }
  //Baudrate Check
  for (size_t i = 0; i < strlen(config[2]); i++)
  {
    int tmp = (int)config[2][i];
    if (tmp >= 48 && tmp <= 57)
    {
      continue;
    }
    else
    {
      printf("Baudrate Error!\n");
      abort();
    }
  }
  //Sendmode Check
  if ((!strcmp(config[3], "ascii") || !strcmp(config[3], "ASCII")) || (!strcmp(config[3], "hex") || !strcmp(config[3], "HEX")));
  else 
  {
    printf("Send mode Error, should set as ascii/hex!\n");
    abort();
  }
  //MessageCheck for Hex
  if (!strcmp(config[3],"hex") || !strcmp(config[3],"HEX"))
  {
    string hex_pattern = "^[0-9a-fA-F]+$";
    regex re(hex_pattern);
    if (!regex_match(config[4],re) || (strlen(config[4])%2 != 0))
    {
      printf("Hex message Error, check send mode or message content!\n");
      abort();
    }
  }
  //Looptime Check
  if (param_num == 6)
  {
    for (size_t i = 0; i < strlen(config[5]); i++)
    {
      int tmp = (int)config[5][i];
      if (tmp >= 48 && tmp <= 57)
      {
        continue;
      }
      else
      {
        printf("Looptime Error!\n");
        abort();
      }
    }
  }
}

int PrintConfig(char** config)
{ 
  //param print
  std::cout << "port is: " << config[1] << std::endl;
  std::cout << "Baudrate is: " << config[2] << std::endl;

  //send mode print
  if (!strcmp(config[3], "ascii") || !strcmp(config[3], "ASCII"))
  {
    printf("Send mode is: ASCII\n");
    printf("Ascii Message length is: %d\n", strlen(config[4]));
    std::cout << "Ascii Message is: " << config[4] << std::endl;
    return 1;
  }
  else
  {
    if (!strcmp(config[3], "hex") || !strcmp(config[3], "HEX"))
    {
      printf("Send mode is: HEX\n");
      printf("Hex Message length is: %d\n", strlen(config[4])/2);
      std::cout << "Hex Message is: ";
      vector<uint8_t> hexprint = CharToHex(config);
      for (size_t i = 0; i < hexprint.size(); i++)
      {
        printf("0x%02x ",hexprint[i]);
      }
      printf("\n");
      return 2;
    }
  }
}

void SendAscii(char** config)
{
  uint8_t asc_payload[strlen(config[4])];
  for (size_t i = 0; i < strlen(config[4]); i++)
  {
    asc_payload[i]= config[4][i];
  }
  cout << config[4] << std::endl;
  serial::Serial serial(config[1], atoi(config[2]), serial::Timeout::simpleTimeout(5000));
  printf("%d\n", atoi(config[2]));
  if (serial.isOpen())
  {
    serial.write(asc_payload, strlen(config[4]));
    serial.close();
  }
  else
  printf("Serial Port Open Failed!");
}

vector<uint8_t> CharToHex(char** config)
{
  vector<uint8_t> hex_payload;
  uint8_t int_payload[strlen(config[4])];
  for (size_t i = 0; i < strlen(config[4]); i++)
  {
    if((config[4][i] >= '0') && (config[4][i] <= '9'))
    {
      int_payload[i] = config[4][i]-48;
    }
    if ((config[4][i] >= 'A') && (config[4][i] <= 'F'))
    {
      int_payload[i] = config[4][i]-55;
    }
    if ((config[4][i] >= 'a') && (config[4][i] <= 'f'))
    {
      int_payload[i] = config[4][i]-87;
    }    
  }
  for (size_t i = 0; i < (strlen(config[4])/2); i++)
  {
    hex_payload.push_back(int(int_payload[i*2]*16) + int(int_payload[i*2+1]));
  }
  return hex_payload;
}

void SendHex(char** config)
{ 
  vector<uint8_t> hextosend = CharToHex(config);
  uint8_t hex_payload[hextosend.size()];
  for (size_t i = 0; i < hextosend.size(); i++)
  {
    printf("0x%02x ", hextosend[i]);
  }
  printf("\n");
  for (size_t i = 0; i < hextosend.size(); i++)
  {
    hex_payload[i] = hextosend[i];
  }
  serial::Serial serial(config[1], atoi(config[2]), serial::Timeout::simpleTimeout(5000));
  if (serial.isOpen())
  {
    serial.write(hex_payload, hextosend.size());
    serial.close();
  }
  else
  printf("Serial Port Open Failed!");
}

string BinaryToHex(const string& binaryStr)
{
    string ret;
    static const char *hex = "0123456789ABCDEF";
    for (auto c:binaryStr)
    {
        ret.push_back(hex[(c >> 4) & 0xf]); //取二进制高四位
        ret.push_back(hex[c & 0xf]);        //取二进制低四位
    }
    return ret;
}

//check if eaqul
bool check_eq(serial::Serial& serial, uint8_t num){
  uint8_t buffer;
  serial.read(&buffer, 1);
  if (buffer == num){
    return true;
  }else{
    return false;
  }
}

uint8_t convert_byte(uint8_t * ptr){
  uint8_t f;
  memcpy(&f, ptr, 1);
  return f;
}

bool checksum(uint8_t * payload){
  uint8_t checksum = 0;
  for (int i=0; i<MSG_LENGTH; i++){
    checksum += payload[i];
  }
  return checksum == payload[MSG_LENGTH];
}

float convert_float(uint8_t * ptr){
  float f;
  memcpy(&f, ptr, 4);
  return f;
}

private:

};

#endif //SERIAL_TOOL_H