#include <Arduino.h>
#include "PT6958_pl.h"

unsigned int setDelay = 1;
int counter = 0;

void setup() {
  Serial.begin(9600);
  pinMode(PT6958_PORT_CLK, OUTPUT);
  pinMode(PT6958_PORT_DATA, OUTPUT);
  pinMode(PT6958_PORT_STB, OUTPUT);
  digitalWrite(PT6958_PORT_STB, LOW);
  digitalWrite(PT6958_PORT_DATA, LOW);
  digitalWrite(PT6958_PORT_CLK, HIGH);
  delay(200);
  PT6958DataSettingsCommand(PT6958_DATA_WRITE, PT6958_INC_FIXED, PT6958_MODE_NORMAL);
  PT6958DisplaySet();
  PT6958DisplayControlCommand(PT6958_DQS_14,PT6958_DISPLAY_ON);
  delay(450);
  PT6958DisplayClear();
  PT6958WriteToAddress(PT6958_CMD_ADDR_LED1,PT6958_LED_RED);
  PT6958WriteToAddress(PT6958_CMD_ADDR_LED2,PT6958_LED_GREEN);
  PT6958WriteToAddress(PT6958_CMD_ADDR_LED3,PT6958_LED_ORANGE);
}

// the loop routine runs over and over again forever:
void loop() {

  /*WriteToAddress(PT6958_CMD_ADDR_DISP1, PT6958_LED_0);
  WriteToAddress(PT6958_CMD_ADDR_DISP2, PT6958_LED_0);
  WriteToAddress(PT6958_CMD_ADDR_DISP3, PT6958_LED_0);
  WriteToAddress(PT6958_CMD_ADDR_DISP4, PT6958_LED_0);*/

  if(counter>9999) {counter=0;}
  PT6958PrintDisplayNumber(counter);
  delay(setDelay);
  PT6958KeyGroup_t g1 = PT6958ReadKeyGroups();
  if(g1.group1 == 0x10 && g1.group2 == 0x00 && g1.group3 == 0x00){
    setDelay++;
    if(setDelay > 100) {setDelay = 100;}
  }
  else if(g1.group1 == 0x00 && g1.group2 == 0x01 && g1.group3 == 0x00){
    setDelay--;
    if(setDelay < 1) {setDelay = 1;}
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
