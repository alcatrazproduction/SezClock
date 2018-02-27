// Globals

CRGB          leds[NUM_LEDS];               // Led array
long          color           = 0xffffff;   // Color of LED
unsigned long theTime,currentTime;
int           tours = -1;
S_EEProm      eeprom;

LiquidCrystal lcd(
                  LCD_RS, 
                  LCD_E,
                  LCD_DB4, 
                  LCD_DB5, 
                  LCD_DB6, 
                  LCD_DB7
                  );
UbxGpsNavPvt  gps( GPS_SERIAL );
                  
int           touch_val;
unsigned long touch_millis;
char          touch_bits    = S_IDLE_BIT;
int           menu_stage    = 0;
int           current_item  = 0;

//********************* Mode setting *********************************
char          display_mode  = 0;                  //  Mode 0, Stanby mode, time displayed ( with RTC )
                                                  //  Mode 1, Stanby mode, waiting for race start
                                                  //  Mode 2, Race Running
                                                  //  Mode 3, Race finished, stanby
unsigned long race_duration   = 60;               //  Race duration in seconds
int           race_laps       = 2;                //  Number of laps at end of time, if 0, end at time end ( trainning )
unsigned long _race_duration  = race_duration;    //  Race duration in seconds ( copy )
int           _race_laps      = race_laps;        //  Number of laps at end of time, if 0, end at time end ( trainning ) ( copy )
bool          race_running    = FALSE;              //  Race is running
bool          race_ended      = TRUE;               //  Race is finished
bool          race_waiting    = FALSE;              // Race is waiting for start

