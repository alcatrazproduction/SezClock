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

