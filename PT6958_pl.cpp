#include <Arduino.h>
#include "PT6958_pl.h"

// Arduino ports
byte PT6958_PORT_STB;   // Strobe pin
byte PT6958_PORT_CLK;   // Shift clock pin
byte PT6958_PORT_DATA;  // Input/Output data pin

void PT6958Init(byte STB, byte CLK, byte DATA) {
  PT6958_PORT_STB = STB;
  PT6958_PORT_CLK = CLK;
  PT6958_PORT_DATA = DATA;
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
  delay(400);
  PT6958DisplayClear();
}

void PT6958PrintDisplayNumber(unsigned short number) {
  if(number > 9999) {
    PT6958WriteToAddress(PT6958_CMD_ADDR_DISP1, PT6958_LED_9);
    PT6958WriteToAddress(PT6958_CMD_ADDR_DISP2, PT6958_LED_9);
    PT6958WriteToAddress(PT6958_CMD_ADDR_DISP3, PT6958_LED_9);
    PT6958WriteToAddress(PT6958_CMD_ADDR_DISP4, PT6958_LED_9);
  } else {
    byte seg4 = (number % 10);
    byte seg3 = (number / 10) % 10;
    byte seg2 = (number / 100) % 10;
    byte seg1 = (number / 1000) % 10;
    PT6958WriteToAddress(PT6958_CMD_ADDR_DISP1, PT6958TranslateLookup(seg1+0x30));
    PT6958WriteToAddress(PT6958_CMD_ADDR_DISP2, PT6958TranslateLookup(seg2+0x30));
    PT6958WriteToAddress(PT6958_CMD_ADDR_DISP3, PT6958TranslateLookup(seg3+0x30));
    PT6958WriteToAddress(PT6958_CMD_ADDR_DISP4, PT6958TranslateLookup(seg4+0x30));
  }
}

unsigned char PT6958ReadDataRaw() {
  int i;
  unsigned char keycode = 0;
  for(i=0; i<8; i++) {
    digitalWrite(PT6958_PORT_CLK, LOW);
    if(digitalRead(PT6958_PORT_DATA)) {
      keycode |= (1 << i);
    }
    delayMicroseconds(1);
    digitalWrite(PT6958_PORT_CLK, HIGH);
    delayMicroseconds(1);
  }
  return keycode;
}

PT6958KeyGroup_t PT6958ReadKeyGroups() {
  PT6958KeyGroup_t kgroup;
  digitalWrite(PT6958_PORT_STB, LOW);
  PT6958SendDataRaw(PT6958_CMD_READ); // Data read, address increment, mode normal
  pinMode(PT6958_PORT_DATA, INPUT);
  kgroup.group1 = PT6958ReadDataRaw();
  kgroup.group2 = PT6958ReadDataRaw();
  kgroup.group3 = PT6958ReadDataRaw();
  digitalWrite(PT6958_PORT_STB, HIGH);
  pinMode(PT6958_PORT_DATA, OUTPUT);
  PT6958DataSettingsCommand(PT6958_DATA_WRITE, PT6958_INC_FIXED, PT6958_MODE_NORMAL);
  return kgroup;
}

byte PT6958ButtonDetect() {
  PT6958KeyGroup_t kg = PT6958ReadKeyGroups();
  if(kg.group1 == 0x10 && kg.group2 == 0x00 && kg.group3 == 0x00){
    return PT6958_BUTTON_UP;
  }
  else if(kg.group1 == 0x00 && kg.group2 == 0x01 && kg.group3 == 0x00){
    return PT6958_BUTTON_DOWN;
  }
  else if(kg.group1 == 0x10 && kg.group2 == 0x00 && kg.group3 == 0x00){
    return PT6958_BUTTON_POWER;
  }
  else if(kg.group1 == 0x20 && kg.group2 == 0x00 && kg.group3 == 0x00){
    return PT6958_BUTTON_MENU;
  }
  else if(kg.group1 == 0xFF && kg.group2 == 0xFF && kg.group3 == 0xFF){ // Key read failed
    return PT6958_BUTTON_RADIO;
  }
  else if(kg.group1 == 0x00 && kg.group2 == 0x02 && kg.group3 == 0x00){
    return PT6958_BUTTON_LEFT;
  }
  else if(kg.group1 == 0x04 && kg.group2 == 0x00 && kg.group3 == 0x00){
    return PT6958_BUTTON_OK;
  }
  else if(kg.group1 == 0x40 && kg.group2 == 0x00 && kg.group3 == 0x00){
    return PT6958_BUTTON_RIGHT;
  }
  else {
    return PT6958_BUTTON_LEAVE;
  }
}

void PT6958SendDataRaw(byte number) {
  bool b0 = (number & 0x01) >> 0;
  bool b1 = (number & 0x02) >> 1;
  bool b2 = (number & 0x04) >> 2;
  bool b3 = (number & 0x08) >> 3;
  bool b4 = (number & 0x10) >> 4;
  bool b5 = (number & 0x20) >> 5;
  bool b6 = (number & 0x40) >> 6;
  bool b7 = (number & 0x80) >> 7;
  digitalWrite(PT6958_PORT_CLK, LOW);
  digitalWrite(PT6958_PORT_DATA, b0);
  digitalWrite(PT6958_PORT_CLK, HIGH);
  digitalWrite(PT6958_PORT_CLK, LOW);
  digitalWrite(PT6958_PORT_DATA, b1);
  digitalWrite(PT6958_PORT_CLK, HIGH);
  digitalWrite(PT6958_PORT_CLK, LOW);
  digitalWrite(PT6958_PORT_DATA, b2);
  digitalWrite(PT6958_PORT_CLK, HIGH);
  digitalWrite(PT6958_PORT_CLK, LOW);
  digitalWrite(PT6958_PORT_DATA, b3);
  digitalWrite(PT6958_PORT_CLK, HIGH);
  digitalWrite(PT6958_PORT_CLK, LOW);
  digitalWrite(PT6958_PORT_DATA, b4);
  digitalWrite(PT6958_PORT_CLK, HIGH);
  digitalWrite(PT6958_PORT_CLK, LOW);
  digitalWrite(PT6958_PORT_DATA, b5);
  digitalWrite(PT6958_PORT_CLK, HIGH);
  digitalWrite(PT6958_PORT_CLK, LOW);
  digitalWrite(PT6958_PORT_DATA, b6);
  digitalWrite(PT6958_PORT_CLK, HIGH);
  digitalWrite(PT6958_PORT_CLK, LOW);
  digitalWrite(PT6958_PORT_DATA, b7);
  digitalWrite(PT6958_PORT_CLK, HIGH);
}

void PT6958DataSettingsCommand(byte settings, byte increment, byte mode) {
  byte output = 0x40 | settings; // 2 bit setup
  bitWrite(output,2,increment);  // 1 bit inc
  bitWrite(output,3,mode);       // 1 bit mode
  digitalWrite(PT6958_PORT_STB, LOW);
  PT6958SendDataRaw(output);
  digitalWrite(PT6958_PORT_STB, HIGH);
}

void PT6958AddressSettingsCommand(byte address) {
  byte output = 0xC0;        // Zero address default
  if ((address >= 0x00) && (address < PT6958_RAM_SIZE)) {
    output = 0xC0 | address; // 4 bit address
  }
  PT6958SendDataRaw(output);
}

void PT6958DisplayControlCommand(byte dimming, byte show) {
  byte output = 0x80 | dimming;  // 3 bit pulse width
  bitWrite(output,3,show);       // 1 bit show
  digitalWrite(PT6958_PORT_STB, LOW);
  PT6958SendDataRaw(output);
  digitalWrite(PT6958_PORT_STB, HIGH);
}

void PT6958WriteToAddress(byte address, byte data) {
  digitalWrite(PT6958_PORT_STB, LOW);
  PT6958AddressSettingsCommand(address);
  PT6958SendDataRaw(data); //data
  digitalWrite(PT6958_PORT_STB, HIGH);
}

void PT6958DisplayClear() {
  int i;
  for(i=0;i<PT6958_RAM_SIZE;i++) {
    PT6958WriteToAddress(i,0x00);
  }
}

void PT6958DisplaySet() {
  int i;
  for(i=0;i<PT6958_RAM_SIZE;i++) {
    PT6958WriteToAddress(i,0xFF);
  }
}

unsigned char PT6958TranslateLookup(char in){
   int i;
   unsigned char out;
   PT6958CharTable_t *table = (PT6958CharTable_t *)PT6958CharTableData;

   for(i=0; i<PT6958_TABLE_LEN; i++)   {
      if(table->znak == in) {
        out = table->kod; break;
      }
      table++;
   }
   if(i==PT6958_TABLE_LEN) out = PT6958_LED_UNDERSCORE; //otherwise _
   return out;
}
