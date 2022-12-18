
#include <OneWire.h>
#define DS18S20_ID 0x10
#define DS18B20_ID 0x28
#define SEN_PIN 0
#define OUT_PIN 1
#define DS_PIN 2
#define TEMP_THRESHOLD 94

OneWire ds(DS_PIN);

byte data[12];
byte addr[8]; 
int temp;
byte state = 0;
byte preState = 0;
byte count = 0;

void setup(){
  pinMode(OUT_PIN, OUTPUT); // Geyser out pin
  digitalWrite(OUT_PIN, LOW); // default turn off the geyser
  pinMode(SEN_PIN, INPUT);  // geyser sensor
  preState = digitalRead(SEN_PIN);  // set previous state
}

void loop(){
  // check water flow first  
  state = digitalRead(SEN_PIN);
  if (state == preState && count < 10) {// water is not running
    count++;  // increment count
    delay(200); // wait for some time
    return;
  }
  if (state == preState && count >= 10) {  // water is not running since last more then 2 sec off the geyser
    digitalWrite(OUT_PIN, LOW);    // turn the geyser off by making the voltage LOW
    return; // water is not running no need to check temparature
  }
  preState = state; // state is changed set previous state as current state
  count = 0;  // reset counter
  // water flow is there check temp
  //find a device
  if (!ds.search(addr)) {
    ds.reset_search();
    delay(250);
    return;
  }
  if (OneWire::crc8( addr, 7) != addr[7]) {
    return;
  }
  if (addr[0] != DS18S20_ID && addr[0] != DS18B20_ID) {
    return;
  }
 
  ds.reset();
  ds.select(addr);
  // start conversion, with parasite power on at the end
  ds.write(0x44, 1);
  // Wait some time...
  delay(1000);  // maybe 750ms is enough, maybe not

  byte i;  
  byte present = 0;
  present = ds.reset();
  ds.select(addr);
  // Issue Read scratchpad command
  ds.write(0xBE);
  // Receive 9 bytes
  for ( i = 0; i < 9; i++) {
    data[i] = ds.read();
  }
  // Calculate temperature value
  temp = ((( (data[1] << 8) + data[0] )*0.0625)*1.8)+32;

  if (temp > TEMP_THRESHOLD && count < 10) {  // input water temp is greater then set value
    count++;
    return;
  }
  if (temp > TEMP_THRESHOLD && count >= 10) { //solar hot water is comming from last 10 sec so we can turn off geyser
    digitalWrite(OUT_PIN, LOW);    // turn the LED off by making the voltage LOW
    return; // water is already hot no need turn geyser on
  }
  count = 0;
  // water is running and temp is less then threshould so turn geyser on
  digitalWrite(OUT_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
}
