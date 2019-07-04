#include <Arduino.h>
// Data
#define PT6958_DATA_READ   0x2
#define PT6958_DATA_WRITE  0x0
#define PT6958_INC_AUTO    0x0
#define PT6958_INC_FIXED   0x1
#define PT6958_MODE_NORMAL 0x0
#define PT6958_MODE_TEST   0x1
#define PT6958_DISPLAY_OFF 0x0
#define PT6958_DISPLAY_ON  0x1
#define PT6958_RAM_SIZE    10
// Arduino ports
#define PT6958_PORT_STB    10
#define PT6958_PORT_CLK    13
#define PT6958_PORT_DOUT   11
#define PT6958_PORT_DIN    12
// Segments address
#define PT6958_CMD_ADDR_DISP1	0x00 // 8.:..
#define PT6958_CMD_ADDR_DISP2	0x02 // .8:..
#define PT6958_CMD_ADDR_DISP3	0x04 // ..:8.
#define PT6958_CMD_ADDR_DISP4	0x06 // ..:.8
/*
 * 0 1 2 3 4 5 6 6 7 8 9
 */
#define PT6958_LED_0_dot		(0x3F|0x80)
#define PT6958_LED_1_dot		(0x06|0x80)
#define PT6958_LED_2_dot		(0x5B|0x80)
#define PT6958_LED_3_dot		(0x4F|0x80)
#define PT6958_LED_4_dot		(0x66|0x80)
#define PT6958_LED_5_dot		(0x6D|0x80)
#define PT6958_LED_6_dot		(0x7D|0x80)
#define PT6958_LED_7_dot		(0x27|0x80)
#define PT6958_LED_8_dot		(0x7F|0x80)
#define PT6958_LED_9_dot		(0x6F|0x80)
#define PT6958_LED_0			(0x3F|0x00)
#define PT6958_LED_1			(0x06|0x00)
#define PT6958_LED_2			(0x5B|0x00)
#define PT6958_LED_3			(0x4F|0x00)
#define PT6958_LED_4			(0x66|0x00)
#define PT6958_LED_5			(0x6D|0x00)
#define PT6958_LED_6			(0x7D|0x00)
#define PT6958_LED_7			(0x27|0x00)
#define PT6958_LED_8			(0x7F|0x00)
#define PT6958_LED_9			(0x6F|0x00)
#define PT6958_LED_UNDERSCORE	(0x08|0x00)
// Leds status
#define PT6958_LED_OFF			0
#define PT6958_LED_ON			1
#define PT6958_LED_RED 		    1
#define PT6958_LED_GREEN 		2
#define PT6958_LED_ORANGE		3
// Leds address
#define PT6958_CMD_ADDR_LED1	0x05 // xxO..
#define PT6958_CMD_ADDR_LED2	0x07 // xx.O.
#define PT6958_CMD_ADDR_LED3	0x09 // xx..O
#define PT6958_CMD_ADDR_DOTDIV  0x08 // ..(:)..
// Dimming val
enum Dimming {
  PT6958_DQS_1,
  PT6958_DQS_2,
  PT6958_DQS_4,
  PT6958_DQS_10,
  PT6958_DQS_11,
  PT6958_DQS_12,
  PT6958_DQS_13,
  PT6958_DQS_14
};
// Key groups
typedef struct {
  unsigned char group1;
  unsigned char group2;
  unsigned char group3;
} pt6958_key_group_t;
// Lookup table
typedef struct {
  unsigned char znak; 	// original
  unsigned char kod; 	// code
} pt6958_char_table_t;
static const pt6958_char_table_t pt6958_char_table_data[]={
    // cyfry
    { '0', PT6958_LED_0, },
    { '1', PT6958_LED_1, },
    { '2', PT6958_LED_2, },
    { '3', PT6958_LED_3, },
    { '4', PT6958_LED_4, },
    { '5', PT6958_LED_5, },
    { '6', PT6958_LED_6, },
    { '7', PT6958_LED_7, },
    { '8', PT6958_LED_8, },
    { '9', PT6958_LED_9, },
};
#define PT6958_TABLE_LEN 		(sizeof(pt6958_char_table_data)/sizeof(pt6958_char_table_t))
#define PT6958_MAX_CHARS		4

// Functions
void SendDataRaw(bool b7, bool b6, bool b5, bool b4, bool b3, bool b2, bool b1, bool b0);
void SendDataRawDec(byte number);
void DataSettingsCommand(byte settings, byte increment, byte mode);
void AddressSettingsCommand(byte address);
void DisplayControlCommand(byte dimming, byte show);
void WriteToAddress(byte address, byte data);
void DisplayClear();
void DisplaySet();
void PrintDisplayNumber(unsigned short number);
unsigned char ReadDataRaw();
pt6958_key_group_t ReadKeyGroups();
static unsigned char pt6958_translate_lookup(char in);
unsigned int setDelay = 1;
int counter = 0;

void setup() {
  Serial.begin(9600);
  pinMode(PT6958_PORT_CLK, OUTPUT);
  pinMode(PT6958_PORT_DOUT, OUTPUT);
  pinMode(PT6958_PORT_DIN, INPUT);
  pinMode(PT6958_PORT_STB, OUTPUT);
  digitalWrite(PT6958_PORT_STB, LOW);
  digitalWrite(PT6958_PORT_DOUT, LOW);
  digitalWrite(PT6958_PORT_CLK, HIGH);
  delay(200);
  DataSettingsCommand(PT6958_DATA_WRITE, PT6958_INC_FIXED, PT6958_MODE_NORMAL);
  DisplaySet();
  DisplayControlCommand(PT6958_DQS_14,PT6958_DISPLAY_ON);
  delay(450);
  DisplayClear();
  WriteToAddress(PT6958_CMD_ADDR_LED1,PT6958_LED_RED);
  WriteToAddress(PT6958_CMD_ADDR_LED2,PT6958_LED_GREEN);
  WriteToAddress(PT6958_CMD_ADDR_LED3,PT6958_LED_ORANGE);
}

// the loop routine runs over and over again forever:
void loop() {

  /*WriteToAddress(PT6958_CMD_ADDR_DISP1, PT6958_LED_0);
  WriteToAddress(PT6958_CMD_ADDR_DISP2, PT6958_LED_0);
  WriteToAddress(PT6958_CMD_ADDR_DISP3, PT6958_LED_0);
  WriteToAddress(PT6958_CMD_ADDR_DISP4, PT6958_LED_0);*/

 /* int i,j,k,l;
  for(i=0;i<10;i++) {
    WriteToAddress(PT6958_CMD_ADDR_DISP1, pt6958_translate_lookup(i+0x30));
    for(j=0;j<10;j++) {
      WriteToAddress(PT6958_CMD_ADDR_DISP2, pt6958_translate_lookup(j+0x30));
      for(k=0;k<10;k++) {
        WriteToAddress(PT6958_CMD_ADDR_DISP3, pt6958_translate_lookup(k+0x30));
        for(l=0;l<10;l++) {
          WriteToAddress(PT6958_CMD_ADDR_DISP4, pt6958_translate_lookup(l+0x30));
          delay(100);
        }
      }
    }
  }*/
  if(counter>9999) {counter=0;}
  PrintDisplayNumber(counter);
  delay(setDelay);

  pt6958_key_group_t g1 = ReadKeyGroups();
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

void PrintDisplayNumber(unsigned short number) {
  if(number > 9999) {
    WriteToAddress(PT6958_CMD_ADDR_DISP1, PT6958_LED_9);
    WriteToAddress(PT6958_CMD_ADDR_DISP2, PT6958_LED_9);
    WriteToAddress(PT6958_CMD_ADDR_DISP3, PT6958_LED_9);
    WriteToAddress(PT6958_CMD_ADDR_DISP4, PT6958_LED_9);
  } else {
    byte seg4 = (number % 10);
    byte seg3 = (number / 10) % 10;
    byte seg2 = (number / 100) % 10;
    byte seg1 = (number / 1000) % 10;
    WriteToAddress(PT6958_CMD_ADDR_DISP1, pt6958_translate_lookup(seg1+0x30));
    WriteToAddress(PT6958_CMD_ADDR_DISP2, pt6958_translate_lookup(seg2+0x30));
    WriteToAddress(PT6958_CMD_ADDR_DISP3, pt6958_translate_lookup(seg3+0x30));
    WriteToAddress(PT6958_CMD_ADDR_DISP4, pt6958_translate_lookup(seg4+0x30));
  }
}

void SendDataRaw(bool b7, bool b6, bool b5, bool b4, bool b3, bool b2, bool b1, bool b0){
  digitalWrite(PT6958_PORT_CLK, LOW);
  digitalWrite(PT6958_PORT_DOUT, b0);
  digitalWrite(PT6958_PORT_CLK, HIGH);
  digitalWrite(PT6958_PORT_CLK, LOW);
  digitalWrite(PT6958_PORT_DOUT, b1);
  digitalWrite(PT6958_PORT_CLK, HIGH);
  digitalWrite(PT6958_PORT_CLK, LOW);
  digitalWrite(PT6958_PORT_DOUT, b2);
  digitalWrite(PT6958_PORT_CLK, HIGH);
  digitalWrite(PT6958_PORT_CLK, LOW);
  digitalWrite(PT6958_PORT_DOUT, b3);
  digitalWrite(PT6958_PORT_CLK, HIGH);
  digitalWrite(PT6958_PORT_CLK, LOW);
  digitalWrite(PT6958_PORT_DOUT, b4);
  digitalWrite(PT6958_PORT_CLK, HIGH);
  digitalWrite(PT6958_PORT_CLK, LOW);
  digitalWrite(PT6958_PORT_DOUT, b5);
  digitalWrite(PT6958_PORT_CLK, HIGH);
  digitalWrite(PT6958_PORT_CLK, LOW);
  digitalWrite(PT6958_PORT_DOUT, b6);
  digitalWrite(PT6958_PORT_CLK, HIGH);
  digitalWrite(PT6958_PORT_CLK, LOW);
  digitalWrite(PT6958_PORT_DOUT, b7);
  digitalWrite(PT6958_PORT_CLK, HIGH);
}

unsigned char ReadDataRaw() {
  int i;
  unsigned char keycode = 0;
  for(i=0; i<8; i++) {
    digitalWrite(PT6958_PORT_CLK, LOW);
    if(digitalRead(PT6958_PORT_DOUT)) {
      keycode |= (1 << i);
    }
    delayMicroseconds(1);
    digitalWrite(PT6958_PORT_CLK, HIGH);
    delayMicroseconds(1);
  }
  return keycode;
}

pt6958_key_group_t ReadKeyGroups() {
  pt6958_key_group_t kgroup;
  digitalWrite(PT6958_PORT_STB, LOW);
  SendDataRawDec(0x42); // Data read, address increment, mode normal
  pinMode(PT6958_PORT_DOUT, INPUT);
  kgroup.group1 = ReadDataRaw();
  kgroup.group2 = ReadDataRaw();
  kgroup.group3 = ReadDataRaw();
  digitalWrite(PT6958_PORT_STB, HIGH);
  pinMode(PT6958_PORT_DOUT, OUTPUT);
  DataSettingsCommand(PT6958_DATA_WRITE, PT6958_INC_FIXED, PT6958_MODE_NORMAL);
  return kgroup;
}

void SendDataRawDec(byte number) {
  bool b0 = (number & 0x01) >> 0;
  bool b1 = (number & 0x02) >> 1;
  bool b2 = (number & 0x04) >> 2;
  bool b3 = (number & 0x08) >> 3;
  bool b4 = (number & 0x10) >> 4;
  bool b5 = (number & 0x20) >> 5;
  bool b6 = (number & 0x40) >> 6;
  bool b7 = (number & 0x80) >> 7;
  SendDataRaw(b7,b6,b5,b4,b3,b2,b1,b0);
}

void DataSettingsCommand(byte settings, byte increment, byte mode) {
  byte output = 0x40 | settings; // 2 bit setup
  bitWrite(output,2,increment);  // 1 bit inc
  bitWrite(output,3,mode);       // 1 bit mode
  digitalWrite(PT6958_PORT_STB, LOW);
  SendDataRawDec(output);
  digitalWrite(PT6958_PORT_STB, HIGH);
}

void AddressSettingsCommand(byte address) {
  byte output = 0xC0;        // Zero address default
  if ((address >= 0x00) && (address < PT6958_RAM_SIZE)) {
    output = 0xC0 | address; // 4 bit address
  }
  SendDataRawDec(output);
}

void DisplayControlCommand(byte dimming, byte show) {
  byte output = 0x80 | dimming;  // 3 bit pulse width
  bitWrite(output,3,show);       // 1 bit show
  digitalWrite(PT6958_PORT_STB, LOW);
  SendDataRawDec(output);
  digitalWrite(PT6958_PORT_STB, HIGH);
}

void WriteToAddress(byte address, byte data) {
  digitalWrite(PT6958_PORT_STB, LOW);
  AddressSettingsCommand(address);
  SendDataRawDec(data); //data
  digitalWrite(PT6958_PORT_STB, HIGH);
}

void DisplayClear() {
  int i;
  for(i=0;i<PT6958_RAM_SIZE;i++) {
    WriteToAddress(i,0x00);
  }
}

void DisplaySet() {
  int i;
  for(i=0;i<PT6958_RAM_SIZE;i++) {
    WriteToAddress(i,0xFF);
  }
}

static unsigned char pt6958_translate_lookup(char in){
   int i;
   unsigned char out;
   pt6958_char_table_t *table = (pt6958_char_table_t *)pt6958_char_table_data;

   for(i=0; i<PT6958_TABLE_LEN; i++)   {
      if(table->znak == in) {
        out = table->kod; break;
      }
      table++;
   }
   if(i==PT6958_TABLE_LEN) out = PT6958_LED_UNDERSCORE; //otherwise _
   return out;
}
