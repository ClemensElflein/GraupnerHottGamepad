#include "Joystick.h"

// Create Joystick
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD, 0,
    0,true, true, false, false, false, false, true, true, false, false, false);
    
long lastReceived = 0;
byte data[256];
uint8_t recPos = 0;
int16_t channels[256];

void setup() {
  Serial1.begin(115200);
  // Make it invalid
  data[0] = 0;

  Joystick.begin(false);
  Joystick.setXAxisRange(0, 15000);
  Joystick.setYAxisRange(0, 15000);
  Joystick.setRudderRange(0, 15000);
  Joystick.setThrottleRange(0, 15000);
}

void loop() {
  while(Serial1.available()) {
    if(millis() - lastReceived > 5) {
      recPos = 0;
      parseSumd();
    }
    data[recPos++] = Serial1.read();
    lastReceived = millis();
  }
}

// Returns the number of channels received. 0 if invalid
byte parseSumd() {
  // check if its valid
  if(data[0] != 0xA8)
    return 0;
  if(data[1] != 0x01)
    return 0;
  byte count = data[2];
  for(byte c = 0; c < count; c++)
    channels[c] = ((int16_t)data[3+c*2]) << 8 | data[4+c*2];

  Joystick.setXAxis(channels[1]);
  Joystick.setYAxis(channels[2]);
  Joystick.setRudder(channels[3]);
  Joystick.setThrottle(channels[0]);
  Joystick.sendState();


  delay(10);
  
  return count;
}

