
#include "ServoRACtrl.h"

ServoRACtrl RActrl;

void setup() 
{
  Serial.begin(115200);
  Serial.print("Setup... ");

  RActrl.setServo(0, 180, 0, 90);
  RActrl.setServo(1, 180, 0, 90);
  RActrl.setServo(2, 0, 180, 90);
  RActrl.setServo(3, 180, 0, 90);
  RActrl.setServo(4, 0, 180, 90);
  RActrl.setServo(5, 0, 60, 60);

  Serial.println("ok");
  Serial.println("Write MOTOR:RANGE to control the motors");
  Serial.println("Write MOTOR:RANGE with RANGE < 0 to disable/enable a motor");
  Serial.println("Write MOTOR:RANGE with RANGE > 100 to home all motors");
}

void loop() 
{
  if (Serial.available() >= 1)
  {
    String message = Serial.readStringUntil('\n');
    String smessage = message.substring(0, message.indexOf(":"));
    message = message.substring(message.indexOf(":") + 1);
    int sNum = smessage.toInt();
    int sVal = message.toInt();

    if (sNum >= 0 && sNum <= 5)
    {
      if (sVal >= 0 && sVal <= 100)
      {
        Serial.print("Moving motor ");
        Serial.print(sNum);
        Serial.print(" to ");
        Serial.print(sVal);
        Serial.println("% range");

        RActrl.moveTo(sNum, sVal, 1500);
      }
      else if (sVal < 0)
      {
        if (RActrl.isEnabled(sNum))
        {
          Serial.print("Disabling motor ");
          Serial.println(sNum);

          RActrl.disable(sNum);
        }
        else
        {
          Serial.print("Enabling motor ");
          Serial.println(sNum);

          RActrl.enable(sNum);
        }
      }
      else
      {
        Serial.println("Homing all motors");

        RActrl.homeAll(1500);
      }
    }
  }
  RActrl.loop();
}
