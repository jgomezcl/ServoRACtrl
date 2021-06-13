
#ifndef ServoRACtrl_h
#define ServoRACtrl_h

#include <Ramp.h>         //https://github.com/siteswapjuggler/RAMP
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>      //https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library

#define MOVEMENT_TYPE SINUSOIDAL_INOUT   //movements types from https://github.com/siteswapjuggler/RAMP
#define SERVO_COUNT 6   //Number of active servos
#define DEFAULT_RATE 10  //min position update period in ms

//PWMServoDriver config
#define SERVOMIN  95 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  470 // This is the 'maximum' pulse length count (out of 4096)
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
#define OSC_FREQ 27000000 // Oscillator frequency

typedef struct {
  boolean isEnabled;
  int angleMin;
  int angleMax;
  int homingAngle;
  rampInt pos;
} ServoData;

class ServoRACtrl
{
  public:
    ServoRACtrl();
    void moveTo(uint8_t snum, int target);                  // instantaneous movement in a range between 0 and 100
    void moveTo(uint8_t snum, int target, int duration);    // interpolated movement in a range between 0 and 100
    void disable(uint8_t snum);                             // disable a servo
    void enable(uint8_t snum);                              // enable a servo
    void setServo(uint8_t snum, int angleMin, int angleMax, int homingAngle);   // setup and enable a servo
    void homeAll(int duration);                             // home all servos
    void loop();                                            // main movement routine
    boolean isEnabled(uint8_t snum);                        // returns true if servo is enabled, false otherwise

  private:
    boolean _isStarted;
    Adafruit_PWMServoDriver _servoController;
    ServoData _servos[SERVO_COUNT];
    unsigned long _timer;   //last position update
};

#endif
