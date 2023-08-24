#ifndef NODE_H
#define NODE_H

#include "header.h"
#include "test.h"
#include "manipulator.h"
#include "minibot.h"

#define baudrate 115200 //manipulator = 1000000
#define rosrate 100 //manipulator = 400
#define startdelay 0 //manipulator = 100000000

class Node
{
public:
  Node(int &argc, char **argv);
  ~Node();
  void run();

private:
  string portname = "ttyUSB1";

  Test test;

  Manipulator manipulator;
  ros::Publisher initPub;
  ros::Subscriber ikSub;
  ros::Subscriber gripperSub;

  Minibot minibot;
  ros::Subscriber joySub;

  bool retry();
  void timerCallback();
  static void serialReadThread();

  static Serial s;
  static bool serialRead;
  static mutex m;
  static queue<uint8_t> readByte;
};

#endif // NODE_H
