#include "../inc/test.h"

Test::Test(Serial *s, bool *serialRead, mutex *m, queue<uint8_t> *readByte)
{
  this->s = s;
  this->serialRead = serialRead;
  this->m = m;
  this->readByte = readByte;

  conversationStatus = CONVERSATION_REQUEST;
}

Test::~Test()
{

}

void Test::Algorithm_Test()
{
    cout << "send" << endl;

  switch (conversationStatus)
  {
  case CONVERSATION_REQUEST:
  {
    try
    {
      vector<uint8_t> packet;

      //MAKE YOUR PACKET
      srand((unsigned int)time(NULL));
      packet.push_back(0xff);
      packet.push_back(rand() % 100);
      packet.push_back(rand() % 100);

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

        //MAKE YOUR FSM

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
