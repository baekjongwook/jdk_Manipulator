#ifndef PACKET_READ_H
#define PACKET_READ_H

#include "header.h"

class ManiPacketTranslator
{
private:
  enum State
  {
    H1,
    H2,
    H3,
    RSRV,
    PacketID,
    LEN1,
    LEN2,
    INST,
    ERR1,
    ID1,
    D1,
    CRC1_1,
    CRC2_1,
    ERR2,
    ID2,
    D2,
    CRC1_2,
    CRC2_2,
    ERR3,
    ID3,
    D3,
    CRC1_3,
    CRC2_3
  };
  State state;

  int movingstatus1 = 0;
  int movingstatus2 = 0;
  int movingstatus3 = 0;
  int calculation1 = 0;
  int calculation2 = 0;
  int calculation3 = 0;

public:
  ManiPacketTranslator();
  ~ManiPacketTranslator();

  bool readPacket(int front);
};

#endif // PACKET_READ_H
