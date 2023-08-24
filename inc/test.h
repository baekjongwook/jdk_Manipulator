#ifndef TEST_H
#define TEST_H

#include "header.h"

class Test
{
private:
  enum ConversationStatus
  {
    CONVERSATION_REQUEST,
    CONVERSATION_RESPONSE
  };
  ConversationStatus conversationStatus;

public:
  Test(Serial *s, bool *serialRead, mutex *m, queue<uint8_t> *readByte);
  ~Test();

  void Algorithm_Test();

  Serial *s;
  bool *serialRead;
  mutex *m;
  queue<uint8_t> *readByte;
};

#endif // MINIBOT_H
