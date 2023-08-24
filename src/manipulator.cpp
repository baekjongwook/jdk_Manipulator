#include "../inc/manipulator.h"

Manipulator::Manipulator(Serial *s, bool *serialRead, mutex *m, queue<uint8_t> *readByte, ros::Publisher *initPub)
{
  this->s = s;
  this->serialRead = serialRead;
  this->m = m;
  this->readByte = readByte;
  this->initPub = initPub;

  conversationStatus = CONVERSATION_REQUEST;
  packetstate = INIT_WRITE;
  initstate = INIT0;
}

Manipulator::~Manipulator()
{

}

void Manipulator::converter()
{
  angle_control[0] = angle[0] * 251417 / 180;
  angle_control[1] = (angle[1] * 501923 / 180) * -1;
  angle_control[2] = angle[2] * 501923 / 180;
  angle_control[3] = (angle[3] + 180) * 4095 / 360;
  angle_control[4] = (-angle[4] + 180) * 4095 / 360;
  angle_control[5] = (angle[5] + 180) * 4095 / 360;
  velocity_control[0] = velocity[0] / 0.01;
  velocity_control[1] = velocity[1] / 0.01;
  velocity_control[2] = velocity[2] / 0.01;
  velocity_control[3] = velocity[3] / 0.229;
  velocity_control[4] = velocity[4] / 0.229;
  velocity_control[5] = velocity[5] / 0.229;
}

void Manipulator::ikCallback(const std_msgs::Float32MultiArray &msg)
{
  if(packetstate == POSITION_WRITE)
  {
    if(msg.layout.dim[0].label == "init")
    {
      initstate = INIT0;
      initcomplete = false;
      initrequest = true;
    }
    angle.clear();
    velocity.clear();

    for(int i = 0; i < 12; i++)
    {
      if(i < 6)
      {
        angle.push_back(msg.data[i]);
      }
      else
      {
        velocity.push_back(msg.data[i]);
      }
    }

    converter();
  }
  else
  {
    return;
  }
}

void Manipulator::gripperCallback(const std_msgs::BoolConstPtr &msg)
{
  if(packetstate == POSITION_WRITE)
  {
    if(msg->data == 0)
    {
      gripper_control = grippermax;
    }
    else if(msg->data == 1)
    {
      gripper_control = grippermin;
    }
  }
}

void Manipulator::Algorithm_Manipulator()
{
  switch (conversationStatus)
  {
  case CONVERSATION_REQUEST:
  {
    try
    {
      vector<uint8_t> packet;
      std_msgs::Bool msg;

      switch (packetstate)
      {
      case INIT_WRITE:
      {
        gripper_control = grippermin;

        if(initcomplete < 3)
        {
          switch (initstate)
          {
          case INIT0:
          {
            manipacketGenerator.initMani(packet, 0, angle_control, velocity_control, gripper_control);
            s->write(packet);

            initstate = INIT1;

            break;
          }
          case INIT1:
          {
            manipacketGenerator.initMani(packet, 1, angle_control, velocity_control, gripper_control);
            s->write(packet);

            initstate = INIT2;

            break;
          }
          case INIT2:
          {
            manipacketGenerator.initMani(packet, 2, angle_control, velocity_control, gripper_control);
            s->write(packet);

            initstate = INIT3;

            break;
          }
          case INIT3:
          {
            manipacketGenerator.initMani(packet, 3, angle_control, velocity_control, gripper_control);
            s->write(packet);

            initstate = INIT2;

            break;
          }
          }

          msg.data = false;
          packetstate = INIT_WRITE;
        }
        else
        {
          msg.data = true;
          initPub->publish(msg);
          ROS_INFO("inited: %d", msg.data);
          initcomplete = 0;
          initrequest = false;
          packetstate = POSITION_WRITE;
        }

        break;
      }
      case POSITION_WRITE:
      {
        if(initrequest == true)
        {
          packetstate = INIT_WRITE;
        }
        else
        {
          manipacketGenerator.writePacket(packet, angle_control, velocity_control, gripper_control); //initrequest½Ã ¸¶Áö¸· ¸í·ÉÀÌ init¸í·É¿¡ ¼¯¿© µé¾î°¨ ¹æÁö
          s->write(packet);

          packetstate = POSITION_WRITE;
        }

        break;
      }
      }
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
      while(!readByte->empty())
      {
        if(packetstate == INIT_WRITE && (initstate == INIT2 || initstate == INIT3))
        {
          int readBytefront = (int)readByte->front();

          if(manipacketTranslator.readPacket(readBytefront) == true)
          {
            initcomplete++;
          }
        }

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
