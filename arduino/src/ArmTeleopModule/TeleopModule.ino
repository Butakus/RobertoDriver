/* Main program to run in the arduino.
*  Receive the commands from the Raspberry Pi through ArdPiComm library
*  and control the robot and its arm with RobotBase and ArmController libraries.
* 
*  Commands implemented in commands.ino file.
*
*  Note: Works only on version 1.0 and above of Arduino IDE. (only arduino.h)
*
*  Author:  Francisco Miguel Moreno
*  Email:   butakus@gmail.com
*  Version: 0.1
*  Date:    12/11/2015
*/

#include <Motor.h>
#include <RobotBase.h>
#include <ArmController.h>
#include <ArdPiComm.h>


// Arduino MEGA pins
// Motors pins
#define MotorA1_PIN 52
#define MotorA2_PIN 53
#define EnableA_PIN 6
#define MotorB1_PIN 48
#define MotorB2_PIN 49
#define EnableB_PIN 7

// Servos pins
#define ARM_H_PIN 8
#define ARM_V_PIN 9
#define WRIST_PIN 10
#define GRASP_PIN 11

/*
// Arduino UNO pins
// Motors pins
#define MotorA1_PIN 2
#define MotorA2_PIN 4
#define EnableA_PIN 5
#define MotorB1_PIN 7
#define MotorB2_PIN 8
#define EnableB_PIN 6

// Servos pins
#define ARM_H_PIN 9
#define ARM_V_PIN 10
#define WRIST_PIN 11
#define GRASP_PIN 3
*/


Motor mRight(MotorA1_PIN, MotorA2_PIN, EnableA_PIN);
Motor mLeft(MotorB1_PIN, MotorB2_PIN, EnableB_PIN);
RobotBase base(&mLeft, &mRight);

//ArmController arm(ARM_H_PIN, ARM_V_PIN, WRIST_PIN, GRASP_PIN);
ArmController arm;

ArdPiComm comms;

void setup(){
    Serial3.begin(9600);
    comms.begin(&Serial3);
    arm.init(ARM_H_PIN, ARM_V_PIN, WRIST_PIN, GRASP_PIN);
  
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
  
    base.stop();
    base.set_speed(255);
    delay(5000);
    digitalWrite(13, LOW);
  //Serial.println("Ready");
}

void loop(){
    // digitalWrite(13, HIGH);
    // delay(500);
    // digitalWrite(13, LOW);

    if (comms.read())
    {
        uint8_t command = comms.get_command();
        uint8_t payload[1];
        uint8_t payload_size = comms.get_payload(payload);
        if (payload_size > 0)
        {
            // Payload contains always 1 argument byte
            uint8_t argument = payload[0];

            switch (command){
                case 0x00:  stop();                 break;
                case 0x03:  forward(argument);      break;
                case 0x0C:  backward(argument);     break;
                case 0x0F:  left(argument);         break;
                case 0x30:  right(argument);        break;
                case 0x33:  arm_left();             break;
                case 0x3C:  arm_right();            break;
                case 0x3F:  arm_up();               break;
                case 0xC0:  arm_down();             break;
                case 0xC3:  arm_h_pos(argument);    break;
                case 0xCC:  arm_v_pos(argument);    break;
                case 0xCF:  wrist_left();           break;
                case 0xF0:  wrist_right();          break;
                case 0xF3:  wrist_pos(argument);    break;
                case 0xFC:  open_grasp();           break;
                case 0xFF:  close_grasp();          break;
                default:    stop();                 break;
            }
        }
    }
    delay(5);
}
