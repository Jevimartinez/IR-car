#include <IRremote.hpp>
#include <Servo.h>

int IRrecv = 11;
int PinMotorLeft = 4;
int PinMotorRight = 5;
unsigned long MotorLeftTime = millis();
unsigned long MotorRightTime = millis();

Servo ServoRCLeft;
Servo ServoRCRight;

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IRrecv, ENABLE_LED_FEEDBACK);

  pinMode(PinMotorLeft, OUTPUT);
  pinMode(PinMotorRight, OUTPUT);

  ServoRCLeft.attach(PinMotorLeft);
  ServoRCRight.attach(PinMotorRight);

  Serial.println("Started");
}


void attach() {
  if (!ServoRCRight.attached()) {
    ServoRCRight.attach(PinMotorRight);
  }
  if (!ServoRCLeft.attached()) {
    ServoRCLeft.attach(PinMotorLeft);
  }
}


void cleanupRight() {
  if (!(ServoRCRight.read() == 90)) {
    ServoRCRight.write(90);
  }
  if (ServoRCRight.attached()) {
    ServoRCRight.detach();
  }
}


void cleanupLeft() {
  if (!(ServoRCLeft.read() == 90)) {
    ServoRCLeft.write(90);
  }
  if (ServoRCLeft.attached()) {
    ServoRCLeft.detach();
  }
}


void cleanup() { // Workaround to prevent the motor from rotating when it shouldn't
  cleanupRight();
  cleanupLeft();
}


void checkToCleanup() {
  if (MotorRightTime < millis()) {
    cleanupRight();
  }
  if (MotorLeftTime < millis()) {
    cleanupLeft();
  }
}


// 250ms is the time between IR pulses in my controller.

void updateLeftTime() {
  MotorLeftTime = millis() + 250;
}



void updateRightTime() {
  MotorRightTime = millis() + 250;
}


void forward() {
  attach();
  if (!(ServoRCRight.read() == 180)) {
    ServoRCRight.write(180);
  }
  updateRightTime();

  if (!(ServoRCLeft.read() == 0)) {
    ServoRCLeft.write(0);
  }
  updateLeftTime();
}


void backward() {
  attach();
  if (!(ServoRCRight.read() == 0)) {
    ServoRCRight.write(0);
  }
  updateRightTime();

  if (!(ServoRCLeft.read() == 180)) {
    ServoRCLeft.write(180);
  }
  updateLeftTime();
}


void right() {
  attach();
  
  // You may want to uncomment this if you want the car to rotate around itself.
  //if (!(ServoRCRight.read() == 0)) {
  //  ServoRCRight.write(0);
  //}
  //updateRightTime();

  if (!(ServoRCLeft.read() == 0)) {
    ServoRCLeft.write(0);
  }
  updateLeftTime();
}


void left() {
  attach();
  if (!(ServoRCRight.read() == 180)) {
    ServoRCRight.write(180);
  }
  updateRightTime();

  // You may want to uncomment this if you want the car to rotate around itself.
  //if (!(ServoRCLeft.read() == 180)) {
  //  ServoRCLeft.write(180);
  //}
  //updateLeftTime();
}


void loop() {
  if (IrReceiver.decode()) {
    IrReceiver.printIRResultShort(&Serial);
    if (IrReceiver.decodedIRData.command == 0x0A) {
      forward();
    } else if (IrReceiver.decodedIRData.command == 0xB) {
      backward();
    } else if (IrReceiver.decodedIRData.command == 0xC) {
      left();
    } else if (IrReceiver.decodedIRData.command == 0xD) {
      right();
    } else if (IrReceiver.decodedIRData.command == 0x1) {
      cleanup();
    }

    IrReceiver.resume();
  }
  checkToCleanup();
}
