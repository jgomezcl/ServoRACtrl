#include <Arduino.h>
#include "ServoRACtrl.h"

ServoRACtrl::ServoRACtrl()
{
  _servoController = Adafruit_PWMServoDriver();
  _isStarted = false;
  for (int i = 0; i < SERVO_COUNT; i++)
  {
    _servos[i].isEnabled = false;
  }
}

void ServoRACtrl::setServo(uint8_t snum, int angleMin, int angleMax, int homingAngle)
{
  if (!_isStarted)
  {
    _servoController.begin();
    _servoController.setOscillatorFrequency(OSC_FREQ);
    _servoController.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  }

  _servos[snum].isEnabled = true;
  _servos[snum].angleMin =  map(angleMin, 0, 180, SERVOMIN, SERVOMAX);
  _servos[snum].angleMax = map(angleMax, 0, 180, SERVOMIN, SERVOMAX);
  _servos[snum].homingAngle = map(homingAngle, 0, 180, SERVOMIN, SERVOMAX);
  _servos[snum].pos.go(_servos[snum].homingAngle);
}

void ServoRACtrl::moveTo(uint8_t snum, int target, int duration)
{
  if (_servos[snum].isEnabled)
  {
    _servos[snum].pos.go(map(target, 0, 100, _servos[snum].angleMin, _servos[snum].angleMax), duration, MOVEMENT_TYPE);
  }
}

void ServoRACtrl::moveTo(uint8_t snum, int target)
{
  if (_servos[snum].isEnabled)
  {
    _servos[snum].pos.go(map(target, 0, 100, _servos[snum].angleMin, _servos[snum].angleMax));
  }
}

void ServoRACtrl::disable(uint8_t snum)
{
  _servos[snum].isEnabled = false;
  _servoController.setPWM(snum, 0, 0);
}

void ServoRACtrl::enable(uint8_t snum)
{
  _servos[snum].isEnabled = true;
}

void ServoRACtrl::homeAll(int duration)
{
  for (int i = 0; i < SERVO_COUNT; i++)
  {
    _servos[i].pos.go(_servos[i].homingAngle, duration, MOVEMENT_TYPE);
  }
}

boolean ServoRACtrl::isEnabled(uint8_t snum)
{
  return _servos[snum].isEnabled;
}

void ServoRACtrl::loop()
{
  if (millis() - _timer > DEFAULT_RATE)
  {
    _timer = millis();
    for (int i = 0; i < SERVO_COUNT; i++)
    {
      if (_servos[i].isEnabled)
      {
        _servoController.setPWM(i, 0, _servos[i].pos.update());
      }
    }
  }
}
