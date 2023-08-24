#ifndef PACKET_WRITE_H
#define PACKET_WRITE_H

#include "header.h"

#define joint123_Acceleration 100
#define joint1_Dgain 5000
#define joint2_Dgain 700
#define grippermax 2000
#define grippermin 1200
#define grippervelocity 50
#define grippercurrentlimit 300

class ManiPacketGenerator
{
private:

public:
  ManiPacketGenerator();
  ~ManiPacketGenerator();

  void divideByte(vector<uint8_t> &packet, int value, int length);
  void update_Length(vector<uint8_t> &packet);
  void update_crc(vector<uint8_t> &data_blk_ptr);
  void update_Header(vector<uint8_t> &packet);
  void readMotorStatus(vector<uint8_t> &packet);
  void initMani(vector<uint8_t> &packet, int flag, int angle[], int velocity[], int gripper_control);
  void writePacket(vector<uint8_t> &packet, int angle[], int velocity[], int gripper_control);
};

#endif // PACKET_WRITE_H
