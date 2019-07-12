#include <Arduino.h>
#include "PT6958_pl.h"

unsigned int setDelay = 1;
int counter = 0;

void setup() {
  Serial.begin(9600);
  PT6958Init(10, 13, 11);
  PT6958WriteToAddress(PT6958_CMD_ADDR_LED1,PT6958_LED_RED);
  PT6958WriteToAddress(PT6958_CMD_ADDR_LED2,PT6958_LED_GREEN);
  PT6958WriteToAddress(PT6958_CMD_ADDR_LED3,PT6958_LED_ORANGE);
}

// the loop routine runs over and over again forever:
void loop() {

  /*PT6958WriteToAddress(PT6958_CMD_ADDR_DISP1, PT6958_LED_0);
  PT6958WriteToAddress(PT6958_CMD_ADDR_DISP2, PT6958_LED_0);
  PT6958WriteToAddress(PT6958_CMD_ADDR_DISP3, PT6958_LED_0);
  PT6958WriteToAddress(PT6958_CMD_ADDR_DISP4, PT6958_LED_0);*/

  if(counter>9999) {counter=0;}
  PT6958PrintDisplayNumber(counter);
  delay(setDelay);
  switch(PT6958ButtonDetect()) {
    case PT6958_BUTTON_UP:
      setDelay++;
      if(setDelay > 100) {setDelay = 100;}
    break;
    case PT6958_BUTTON_DOWN:
      setDelay--;
      if(setDelay < 1) {setDelay = 1;}
    break;
  }

  /*Serial.print("D:");
  Serial.print(g1.group1, HEX);
  Serial.print(":");
  Serial.print(g1.group2, HEX);
  Serial.print(":");
  Serial.println(g1.group3, HEX);


  delay(100);*/
  counter++;
}
