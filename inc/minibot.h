#ifndef MINIBOT_H
#define MINIBOT_H

#include "header.h"
#include "stm_read.h"
#include "stm_write.h"

class Minibot
{
private:
  StmPacketGenerator stmpacketGenerator;
  StmPacketTranslator stmpacketTranslator;

  sensor_msgs::Joy joy;

  int LED = 0;
  int target_L = 0;
  int target_R = 0;

  enum ConversationStatus
  {
    CONVERSATION_REQUEST,
    CONVERSATION_RESPONSE
  };
  ConversationStatus conversationStatus;

public:
  Minibot(Serial *s, bool *serialRead, mutex *m, queue<uint8_t> *readByte);
  ~Minibot();

  void converter();
  void joyCallback(const sensor_msgs::Joy::ConstPtr &msg);
  void Algorithm_Minibot();

  Serial *s;
  bool *serialRead;
  mutex *m;
  queue<uint8_t> *readByte;
};

#endif // MINIBOT_H
