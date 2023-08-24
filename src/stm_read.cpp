#include"../inc/stm_read.h"

StmPacketTranslator::StmPacketTranslator()
{
  state = H1;
}

StmPacketTranslator::~StmPacketTranslator()
{

}

bool StmPacketTranslator::readPacket(int byte)
{
  switch (state)
  {
  case H1:
  {
    if(byte == 0xFF)
    {
      state = H2;
    }
    break;
  }
  case H2:
  {
    if(byte == 0xFF)
    {
      state = H3;
    }
    else
    {
      state = H1;
    }
    break;
  }
  case H3:
  {
    if(byte == 0xFD)
    {
      state = RSRV;
    }
    else
    {
      state = H1;
    }
    break;
  }
  case RSRV:
  {
    if(byte == 0x00)
    {
      state = PacketID;
    }
    else
    {
      state = H1;
    }
    break;
  }
  case PacketID:
  {
    if(byte == 0xFE)
    {
      state = LEN1;
    }
    else
    {
      state = H1;
    }
    break;
  }
  case LEN1:
  {
    if(byte == 0x10)
    {
      state = LEN2;
    }
    else
    {
      state = H1;
    }
    break;
  }
  case LEN2:
  {
    if(byte == 0x00)
    {
      state = INST;
    }
    else
    {
      state = H1;
    }
    break;
  }
  case INST:
  {
    if(byte == 0x55)
    {
      state = ERR1;
    }
    else
    {
      state = H1;
    }
    break;
  }
  case ERR1:
  {
    if(byte == 0x00)
    {
      state = ID1;
    }
    else
    {
      state = H1;
    }
    break;
  }
  case ID1:
  {
    if(byte == 0x0A)
    {
      state = D1;
    }
    else
    {
      state = H1;
    }
    break;
  }
  case D1:
  {
    movingstatus1 = byte;
    state = CRC1_1;
    break;
  }
  case CRC1_1:
  {
    state = CRC2_1;
    break;
  }
  case CRC2_1:
  {
    state = ERR2;
    break;
  }
  case ERR2:
  {
    state = ID2;
    break;
  }
  case ID2:
  {
    if(byte == 0x0B)
    {
      state = D2;
    }
    else
    {
      state = H1;
    }
    break;
  }
  case D2:
  {
    movingstatus2 = byte;
    state = CRC1_2;
    break;
  }
  case CRC1_2:
  {
    state = CRC2_2;
    break;
  }
  case CRC2_2:
  {
    state = ERR3;
    break;
  }
  case ERR3:
  {
    state = ID3;
    break;
  }
  case ID3:
  {
    if(byte == 0x0C)
    {
      state = D3;
    }
    else
    {
      state = H1;
    }
    break;
  }
  case D3:
  {
    movingstatus3 = byte;
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
      break;
    }
  }
  default:
  {
    state = H1;
    break;
  }
  }

  return false;
}
