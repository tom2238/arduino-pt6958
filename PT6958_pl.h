#ifndef PT6958_PL_ARDUINO_H
#define PT6958_PL_ARDUINO_H

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
#define PT6958_CMD_READ    0x42 // Data read, address increment, mode normal
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
enum PT6958Dimming {
  PT6958_DQS_1,
  PT6958_DQS_2,
  PT6958_DQS_4,
  PT6958_DQS_10,
  PT6958_DQS_11,
  PT6958_DQS_12,
  PT6958_DQS_13,
  PT6958_DQS_14
};
// Pressed buttons
enum PT6958PressButton {
  PT6958_BUTTON_LEAVE,
  PT6958_BUTTON_POWER,
  PT6958_BUTTON_MENU,
  PT6958_BUTTON_RADIO,
  PT6958_BUTTON_LEFT,
  PT6958_BUTTON_UP,
  PT6958_BUTTON_OK,
  PT6958_BUTTON_DOWN,
  PT6958_BUTTON_RIGHT
};

// Key groups
typedef struct {
  unsigned char group1;
  unsigned char group2;
  unsigned char group3;
} PT6958KeyGroup_t;
// Lookup table
typedef struct {
  unsigned char znak; 	// original
  unsigned char kod; 	// code
} PT6958CharTable_t;
static const PT6958CharTable_t PT6958CharTableData[]={
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
#define PT6958_TABLE_LEN 		(sizeof(PT6958CharTableData)/sizeof(PT6958CharTable_t))
#define PT6958_MAX_CHARS		4

// Functions
void PT6958Init(byte STB, byte CLK, byte DATA);
void PT6958PrintDisplayNumber(unsigned short number);
unsigned char PT6958ReadDataRaw();
PT6958KeyGroup_t PT6958ReadKeyGroups();
byte PT6958ButtonDetect();
void PT6958SendDataRaw(byte number);
void PT6958DataSettingsCommand(byte settings, byte increment, byte mode);
void PT6958AddressSettingsCommand(byte address);
void PT6958DisplayControlCommand(byte dimming, byte show);
void PT6958WriteToAddress(byte address, byte data);
void PT6958DisplayClear();
void PT6958DisplaySet();
unsigned char PT6958TranslateLookup(char in);

#endif // PT6958_PL_ARDUINO_H
