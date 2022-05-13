/* You have a class named roboticArm and have class fuction to control the angle of each motor for controlling the DoF of the Robotic Arm*/


//Defining all 6 servo's pins of arduino
#define servo1 3
#define servo2 5
#define servo3 6
#define servo4 9
#define servo5 10
#define servo6 11 // This is for the gripper of the robotic arm


#include <Servo.h>

class RoboticArm {
  private:
    Servo botServo[6];
    byte _Servopin;
    byte Servopins[6];
    int defualtPositions[6] = {90, 90, 90, 90, 90, 0};
    int Positions[];
  public:
    RoboticArm(byte s1, byte s2, byte s3, byte s4, byte s5, byte s6) {
      Servopins[0] = s1;
      Servopins[1] = s2;
      Servopins[2] = s3;
      Servopins[3] = s4;
      Servopins[4] = s5;
      Servopins[5] = s6;

    }
    void setPos(int pos1, int pos2, int pos3, int pos4, int pos5, int pos6) {
      Positions[0] = pos1;
      Positions[1] = pos2;
      Positions[2] = pos3;
      Positions[3] = pos4;
      Positions[4] = pos5;
      Positions[5] = pos6;
    }

    void inits() {
      for (int i = 0; i < 6; i++)
      {
        botServo[i].attach(Servopins[i]);
      }
    }
    void updatePos() {
      for (int i = 0; i < 6; i++)
      {
        botServo[i].write(Positions[i]);
      }

    }
    void defualtPos() {
      for (int i = 0; i < 6; i++)
      {
        botServo[i].write(defualtPositions[i]);
      }
    }
};


RoboticArm myBot = RoboticArm(servo1, servo2, servo3, servo4, servo5, servo6);

void setup() {
  myBot.inits();
  delay(100);
  myBot.defualtPos();
}

void loop() {
  myBot.setPos(120, 100, 150, 70, 80, 0);
  myBot.updatePos();
  delay(1000);
  myBot.setPos(85, 60, 75, 100, 50, 0);
  myBot.updatePos();

}
