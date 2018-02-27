#ifndef TRUE
# define TRUE (1==1)
#endif

#ifndef FALSE
# define FALSE (0==1)
#endif

/*************************************************************************************************
 * GPS Module
 * Used to get time sync
 */
#define   GPS_SERIAL    Serial2
/*************************************************************************************************
 * ESP Module (ESP8266MOD ESP-12 )
 * Data sync with timming system, will receive cmd when race are running ( auto start time, 
 * laps,....)
 * Also, ntp server
 */

#define   ESP_SERIAL    Serial3
/*************************************************************************************************
 * Display controler
 * 
 */
#define   LCD_DB4         30    // Databus d4
#define   LCD_DB5         31    // Databus d5
#define   LCD_DB6         32    // Databus d6
#define   LCD_DB7         33    // Databus d7

#define   LCD_E           38
#define   LCD_RS          39

#define   LCD_SWITCH      15      // AD15

#define   LCD_COLUMN      20      // LCD max columns
#define   LCD_ROW         4       // LCD Line

#define   LCD_TIME        3       // Line for displaying clock 
#define   LCD_STATUS      2       // Status line 


#define   S_IDLE_BIT      0x00
#define   S_UP_BIT        0x01
#define   S_BACK_BIT      0x02
#define   S_MENU_BIT      0x04
#define   S_ENTER_BIT     0x08
#define   S_DOWN_BIT      0x10
#define   S_UNDEF_BIT     0x80

#define   S_T_PRESSED     20                        // time buttons pressed in millis to accept

/**************************************************************************************************
 * 
 * Led strip 
 * 
 */
 
#define   DATA_PIN        37

#define   HOURS_10        0       // Digit A
#define   HOURS_1         1       // Digit B
#define   MINUTES_10      2       // Digit C
#define   MINUTES_1       3       // Digit D
#define   SECONDS_10      4       // Digit E
#define   SECONDS_1       5       // Digit F
#define   DOTS            6       // Dots and special
#define   NUM_DIGITS      6       // We have 6 digits plus dots
#define   NUM_DOTS        4       // We have 4 dots

#define   NUM_LEDS        ( (NUM_DIGITS * 8 ) + NUM_DOTS +8)

// Data for char display ( 8 LED )

static unsigned char font[] = 
{
  0b01110111,    // 0
  0b00100100,    // 1
  0b01011101,    // 2
  0b01101101,    // 3
  0b00101110,    // 4
  0b01101011,    // 5
  0b01111011,    // 6
  0b00100101,    // 7
  0b01111111,    // 8,
  0b01101111,    // 9,
  0b00000000,    // :
  0b00000000,    // ;
  0b00000000,    // <
  0b01001000,    // =
  0b00000000,    // ?
  0b00000000,    // @
  0b00111111,    // A
  0b01111010,    // b
  0b01011000,    // c
  0b01111100,    // d
  0b01011011,    // E
  0b00011011,    // F
  0b00000000,    // G
  0b00111110,    // H
  0b00010010,    // I
  0b10100111,    // J
  0b10011010,    // K
  0b01010010,    // L
  0b00000000,    // M
  0b00111000,    // n
  0b01111000,    // o
  0b00011111,    // P
  0b11111000,    // q
  0b10011111,    // R
  0b01101011,    // S
  0b00011010,    // t
  0b01110000,    // u
  0b10100110,    // V
  0b00000000,    // W
  0b00000000,    // X
  0b00000000,    // Y
  0b01011101,    // Z
  0
};

