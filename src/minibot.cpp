#include "../inc/minibot.h"

Minibot::Minibot(Serial *s, bool *serialRead, mutex *m, queue<uint8_t> *readByte)
{
  this->s = s;
  this->serialRead = serialRead;
  this->m = m;
  this->readByte = readByte;

  conversationStatus = CONVERSATION_REQUEST;
}

Minibot::~Minibot()
{

}

void Minibot::converter()
{
  if(joy.buttons[9] == 1)
  {
    if(LED == 0)
    {
      LED = 1;
    }
    else
    {
      LED = 0;
    }
  }

  if(joy.axes[1] != 0 && joy.axes[3] == 0)
  {
    target_L = joy.axes[1] * 200;
    target_R = joy.axes[1] * 200;
  }
  else if(joy.axes[1] == 0 && joy.axes[3] != 0)
  {
    target_L = -joy.axes[3] * 200;
    target_R = joy.axes[3] * 200;
  }
  else if(joy.axes[1] > 0 && joy.axes[3] > 0) //small TL
  {
    target_L = 0;
    target_R = joy.axes[3] * 200;
  }
  else if(joy.axes[1] > 0 && joy.axes[3] < 0) //small TR
  {
    target_L = joy.axes[3] * 200;
    target_R = 0;
  }
  else
  {
    target_L = 0;
    target_R = 0;
  }
}

void Minibot::joyCallback(const sensor_msgs::Joy::ConstPtr &msg)
{
  joy.axes = msg->axes;
  joy.buttons = msg->buttons;

  converter();
}

void Minibot::Algorithm_Minibot()
{
  switch (conversationStatus)
  {
  case CONVERSATION_REQUEST:
  {
    try
    {
      vector<uint8_t> packet;
      stmpacketGenerator.writePacket_mini(packet, LED, target_L, target_R);
      s->write(packet);
    }
    catch (serial::IOException e)
    {
      cerr << "Port open failed." << e.what() << endl;
      s->close();
    }

    conversationStatus = CONVERSATION_RESPONSE;

    break;
  }
  case CONVERSATION_RESPONSE:
  {
    if(!readByte->empty())
    {
      cout << "serial received:" << endl;

      while(!readByte->empty())
      {
        int readBytefront = (int)readByte->front();
        cout << readBytefront << endl;

        /*if(manipacketTranslator.readPacket(readBytefront) == true)
        {
          initcomplete++;
        }*/

        m->lock();
        readByte->pop();
        m->unlock();
      }
    }

    conversationStatus = CONVERSATION_REQUEST;

    break;
  }
  }
}
