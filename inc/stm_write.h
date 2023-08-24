#ifndef STM_WRITE_H
#define STM_WRITE_H

#include "header.h"

class StmPacketGenerator
{
private:

public:
  StmPacketGenerator();
  ~StmPacketGenerator();

  void divideByte(vector<uint8_t> &packet, int value, int length);
  void update_Length(vector<uint8_t> &packet);
  void update_crc(vector<uint8_t> &data_blk_ptr);
  void update_Header(vector<uint8_t> &packet);
  void writePacket_mini(vector<uint8_t> &packet, int LED, int Target_L, int Target_R);
};

#endif // STM_WRITE_H
