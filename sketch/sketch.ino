long lastReceived = 0;
byte data[256];
uint8_t recPos = 0;
int16_t channels[256];

void setup() {
  Serial.begin(100000);
  Serial1.begin(115200);
  // Make it invalid
  data[0] = 0;
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
  
  return count;
}

