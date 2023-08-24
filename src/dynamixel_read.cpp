#include"../inc/dynamixel_read.h"

ManiPacketTranslator::ManiPacketTranslator()
{
  state = H1;
}

ManiPacketTranslator::~ManiPacketTranslator()
{

}

bool ManiPacketTranslator::readPacket(int front)
{
  switch (state)
  {
  case H1:
  {
    if(front == 0xFF)
    {
      state = H2;
    }
    else
    {
      state = H1;
    }
    return false;
  }
  case H2:
  {
    if(front == 0xFF)
    {
      state = H3;
    }
    else
    {
      state = H1;
    }
    return false;
  }
  case H3:
  {
    if(front == 0xFD)
    {
      state = RSRV;
    }
    else
    {
      state = H1;
    }
    return false;
  }
  case RSRV:
  {
    if(front == 0x00)
    {
      state = PacketID;
    }
    else
    {
      state = H1;
    }
    return false;
  }
  case PacketID:
  {
    if(front == 0xFE)
    {
      state = LEN1;
    }
    else
    {
      state = H1;
    }
    return false;
  }
  case LEN1:
  {
    if(front == 0x10)
    {
      state = LEN2;
    }
    else
    {
      state = H1;
    }
    return false;
  }
  case LEN2:
  {
    if(front == 0x00)
    {
      state = INST;
    }
    else
    {
      state = H1;
    }
    return false;
  }
  case INST:
  {
    if(front == 0x55)
    {
      state = ERR1;
    }
    else
    {
      state = H1;
    }
    return false;
  }
  case ERR1:
  {
    if(front == 0x00)
    {
      state = ID1;
    }
    else
    {
      state = H1;
    }
    return false;
  }
  case ID1:
  {
    if(front == 0x0A)
    {
      state = D1;
    }
    else
    {
      state = H1;
    }
    return false;
  }
  case D1:
  {
    movingstatus1 = front;
    state = CRC1_1;
    return false;
  }
  case CRC1_1:
  {
    state = CRC2_1;
    return false;
  }
  case CRC2_1:
  {
    state = ERR2;
    return false;
  }
  case ERR2:
  {
    state = ID2;
    return false;
  }
  case ID2:
  {
    if(front == 0x0B)
    {
      state = D2;
    }
    else
    {
      state = H1;
    }
    return false;
  }
  case D2:
  {
    movingstatus2 = front;
    state = CRC1_2;
    return false;
  }
  case CRC1_2:
  {
    state = CRC2_2;
    return false;
  }
  case CRC2_2:
  {
    state = ERR3;
    return false;
  }
  case ERR3:
  {
    state = ID3;
    return false;
  }
  case ID3:
  {
    if(front == 0x0C)
    {
      state = D3;
    }
    else
    {
      state = H1;
    }
    return false;
  }
  case D3:
  {
    movingstatus3 = front;
    state = H1;

    calculation1 = movingstatus1 & 1;
    calculation2 = movingstatus2 & 1;
    calculation3 = movingstatus3 & 1;

    if(calculation1 == 1 && calculation2 == 1 && calculation3 == 1)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  default:
  {
    state = H1;
    return false;
  }
  }
}
