#ifndef MANIPULATOR_H
#define MANIPULATOR_H

#include "header.h"
#include "dynamixel_read.h"
#include "dynamixel_write.h"

class Manipulator
{
private:
  ManiPacketGenerator manipacketGenerator;
  ManiPacketTranslator manipacketTranslator;

  vector<float> angle;
  vector<float> velocity;
  int angle_control[6] = {0, 0, 0, 2048, 1876, 2048};
  int velocity_control[6] = {300, 300, 300, 50, 50, 50};
  int gripper_control = 1200;
  int initcomplete = 0;
  bool initrequest = false;

  enum ConversationStatus
  {
    CONVERSATION_REQUEST,
    CONVERSATION_RESPONSE
  };
  ConversationStatus conversationStatus;
  enum PacketState
  {
    INIT_WRITE,
    POSITION_WRITE,
  };
  PacketState packetstate;
  enum InitState
  {
    INIT0,
    INIT1,
    INIT2,
    INIT3
  };
  InitState initstate;

public:
  Manipulator(Serial *s, bool *serialRead, mutex *m, queue<uint8_t> *readByte, ros::Publisher *initPub);
  ~Manipulator();

  void converter();
  void ikCallback(const std_msgs::Float32MultiArray &msg);
  void gripperCallback(const std_msgs::BoolConstPtr &msg);
  void Algorithm_Manipulator();

  Serial *s;
  bool *serialRead;
  mutex *m;
  queue<uint8_t> *readByte;
  ros::Publisher *initPub;
};

#endif // MANIPULATOR_H
