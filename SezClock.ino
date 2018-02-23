#include <EEPROM.h>

#include <UbxGpsNavPosllh.h>
#include <UbxGps.h>
#include <UbxGpsNavSol.h>
#include <UbxGpsNavPosecef.h>
#include <UbxGpsNavPvt.h>

#include <TimeLib.h>
#include <Time.h>

/***************************************************************************************************************************
 * Please use version 1.8.5 of environement
 */

#include <LiquidCrystal.h>

#include "Adafruit_NeoPixel.h"


#include "FastLED.h"


#include "board.h"
#include "function.h"
#include <stdio.h>
#include <time.h>

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
UbxGpsNavPvt  gps(Serial2);
                  
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
bool          race_running  = FALSE;              //  Race is running
bool          race_ended    = TRUE;               //  Race is finished
bool          race_waiting  = FALSE;              // Race is waiting for start


int   readEEProm()
{
  EEPROM.get( 0, eeprom );
  if( eeprom.saveTime == 0xffffffff )
  {
    eeprom.saveTime    = 1519401377;               // Saved time
    eeprom.timeZone    = 1;
    eeprom.summerTime  = false;
    eeprom.reserved1   = false;
    eeprom.reserved2   = false;
    eeprom.reserved3   = false;
    eeprom.reserved4   = false;
    eeprom.reserved5   = false;
    eeprom.reserved6   = false;
    eeprom.reserved7   = false;
    eeprom.lastRace    = 20*60;
    eeprom.lastLap     = 2;
    EEPROM.put( 0, eeprom );
    return 1;
  }
  return 0;
}

int   writeEEProm()
{
  EEPROM.put( 0, eeprom );
  return 0;
}

//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, 6, NEO_GRB + NEO_KHZ800);

void  writeLed( int witch, unsigned long col )
{
  if( witch < sizeof( leds ) )
    leds[ witch ] = col;
}

void  writeDigit( int witch, unsigned char data )
{
  if( witch < NUM_DIGITS )
  {
    unsigned char pixel = font[data < sizeof( font) ? data : sizeof( font ) - 1];   // gets the char data, if to big, take the last ( all black )
    for( int bits=0;bits < 8; bits ++ )
    {
      writeLed( ( witch * 8 ) + bits , pixel & 0x01 ? color : CRGB::Black );
 //     pixels.setPixelColor( ( witch * 8 ) + bits , pixel & 0x01 ? pixels.Color(127,0,0) : pixels.Color(0,0,0) );
      pixel = pixel >> 1;
    }
  }
};

void  writeDots( int witch, unsigned long col )
{
  if( witch < NUM_DOTS )
  {
    writeLed( ( DOTS * 8 ) + witch  , col );
   // pixels.setPixelColor(  ( DOTS * 8 ) + witch , col  ? pixels.Color(127,0,0) : pixels.Color(0,0,0) );
  }
};

void  displayClock( void )
{
struct tm       theTm;
//struct tm       theTm2;
unsigned long   actTime = theTime / 1000;
char            *lcd_buf = "                    ";
 
  lcd.setCursor(0,3);
  memset(&theTm,0,sizeof( theTm ) );
  switch( display_mode )
  {
    case 0:
        actTime = now();
        localtime_r( &actTime , &theTm);
      
        writeDigit( HOURS_10    , theTm.tm_hour   / 10 );
        writeDigit( HOURS_1     , theTm.tm_hour   % 10 );
        writeDigit( MINUTES_10  , theTm.tm_min    / 10 );
        writeDigit( MINUTES_1   , theTm.tm_min    % 10 );
        writeDigit( SECONDS_10  , theTm.tm_sec    / 10 );
        writeDigit( SECONDS_1   , theTm.tm_sec    % 10 );
        writeDots(  0           , (theTm.tm_sec%2)==0 ? 0: 0xffffff );
        writeDots(  1           , (theTm.tm_sec%2)==0 ? 0: 0xffffff );
        writeDots(  2           , (theTm.tm_sec%2)==1 ? 0: 0xffffff );
        writeDots(  3           , (theTm.tm_sec%2)==1 ? 0: 0xffffff );
    
        snprintf(lcd_buf,21,"Time: %2.2d:%2.2d:%2.2d",theTm.tm_hour,theTm.tm_min,theTm.tm_sec );
      break;
    case 1:
      break;
    case 2:
      if( ( tours <= 0 ) & ( tours != -2 ))
      {
        localtime_r( &actTime , &theTm);
      
    Serial.print( theTime / 1000 );
    Serial.print("\t Time :");
    Serial.print(theTm.tm_hour);
        writeDigit( HOURS_10    , theTm.tm_hour   / 10 );
        writeDigit( HOURS_1     , theTm.tm_hour   % 10 );
    Serial.print(":");
    Serial.print(theTm.tm_min);
        writeDigit( MINUTES_10  , theTm.tm_min    / 10 );
        writeDigit( MINUTES_1   , theTm.tm_min    % 10 );
    Serial.print(":");
    Serial.print(theTm.tm_sec / 10);
        writeDigit( SECONDS_10  , theTm.tm_sec    / 10 );
    Serial.print(theTm.tm_sec % 10);
        writeDigit( SECONDS_1   , theTm.tm_sec    % 10 );
    Serial.println(" ");
        writeDots(  0           , (theTm.tm_sec%2)==0 ? 0: 0xffffff );
        writeDots(  1           , (theTm.tm_sec%2)==0 ? 0: 0xffffff );
        writeDots(  2           , (theTm.tm_sec%2)==1 ? 0: 0xffffff );
        writeDots(  3           , (theTm.tm_sec%2)==1 ? 0: 0xffffff );
    
        snprintf(lcd_buf,21,"%2.2d:%2.2d:%2.2d",theTm.tm_hour,theTm.tm_min,theTm.tm_sec );
      }
      else
      {
        if( tours == -2 )
        {
          writeDigit( HOURS_10    , ' ' - '0'-1 );
          writeDigit( HOURS_1     , ' ' - '0'-1 );
          writeDigit( MINUTES_10  , ' ' - '0'-1 );
          writeDigit( MINUTES_1   , ' ' - '0'-1 );
          writeDigit( SECONDS_10  , ' ' - '0'-1 );
          writeDigit( SECONDS_1   , ' ' - '0'-1 );
          writeDots(  0           , 0x000000 );
          writeDots(  1           , 0x000000 );
          writeDots(  2           , 0x000000 );
          writeDots(  3           , 0x000000 );
          color = 0xffffff;
          tours = -1;
        }
        else
        {
          if( tours == 2 )
            color = 0x00ffff;
          else
            color = 0x00ffff;
            
          writeDigit( HOURS_10    , tours );
          writeDigit( HOURS_1     , ' ' - '0'-1 );
          writeDigit( MINUTES_10  , 'T' - '0'-1 );
          writeDigit( MINUTES_1   , 'O' - '0'-1 );
          writeDigit( SECONDS_10  , 'U' - '0'-1 );
          writeDigit( SECONDS_1   , 'R' - '0'-1 );
          writeDots(  0           , color );
          writeDots(  1           , color );
          writeDots(  2           , color );
          writeDots(  3           , color );
    
          snprintf(lcd_buf,21,"%s",tours,tours > 1 ?" tours   ":" tour  ");
        }    
    }
    case 3:
      break;
    default:
      break;
  }
  lcd.print(lcd_buf);
  FastLED.show();
}
/**************************************************************************************************
 * old Version...
 */

void  _processTouch( void )
{
int v10   = touch_val / 10;

  switch( touch_val / 100 )
  {
    case 00:      // Back,Back-Enter, Menu-Back
      switch( v10 )
      {
        case  9:
//          lcd.print( "Back       " );
          break;
        case  8:
//          lcd.print( "Back-Enter " );
          break;
        case  7:
//          lcd.print( "Back-Menu  " );
          break;
        default:
          lcd.print( "Unknow ");
          lcd.print( touch_val );
          break;
      }
      break;
    case 10:      // Idle
//      lcd.print("Idle       ");
      break;
    case  1:      // Down, Up-Down, Menu-Down
      switch( v10 )
      {
        case  17:
//          lcd.print( "Down       " );
          break;
        case  16:
//          lcd.print( "Up-Down    " );
          break;
        case  12:
//          lcd.print( "Menu-Down  " );
          break;
        default:
//          lcd.print( "Unknow ");
//          lcd.print( touch_val );
          break;
      }      
      break;
    case  2:      // Menu-Enter, Menu-Up
      switch( v10 )
      {
        case  24:
//          lcd.print( "Menu-Enter " );
          break;
        case  27:
//          lcd.print( "Menu-Up    " );
          break;
        default:
//          lcd.print( "Unknow ");
//          lcd.print( touch_val );
          break;
      }
      break;
    case  3:      // Menu
//      lcd.print("Menu       ");
      break;
    case  5:      // Enter
//      lcd.print("Enter      ");
      break;
    case  6:      // Up
//      lcd.print("Up        ");
      break;
    default:      // Undef
//      lcd.print( "Unknow ");
//      lcd.print( touch_val );
      break;
  }
  
}
/**************************************************************************************************
 * new Version...
 * Return the selected function
 */

int  processTouch( void )
{
  
int r;

  switch( touch_val / 11 )
  {
    case  8:
//      lcd.print( "Back       " );
      r = S_BACK_BIT;
      break;
    case  7:
//      lcd.print( "Back-Enter " );
      r = S_BACK_BIT || S_ENTER_BIT;
      break;
    case  6:
//      lcd.print( "Back-Menu  " );
      r = S_BACK_BIT || S_MENU_BIT;
      break;
    case 92:      // Idle
//      lcd.print("Idle       ");
      r = S_IDLE_BIT;
      break;
    case  16:
//      lcd.print( "Down       " );
      r = S_DOWN_BIT;
      break;
    case  14:
//      lcd.print( "Up-Down    " );
      r = S_UP_BIT || S_DOWN_BIT;
      break;
    case  11:
//      lcd.print( "Menu-Down  " );
      r = S_DOWN_BIT || S_MENU_BIT;
      break;
    case  22:
//      lcd.print( "Menu-Enter " );
      r = S_ENTER_BIT || S_MENU_BIT;
      break;
    case  25:
//      lcd.print( "Menu-Up    " );
      r = S_UP_BIT || S_MENU_BIT;
      break;
    case  29:      // Menu
//      lcd.print("Menu       ");
      r = S_MENU_BIT;
      break;
    case  45:      // Enter
//      lcd.print("Enter      ");
      r = S_ENTER_BIT;
      break;
    case  62:      // Up
      r = S_UP_BIT;
//      lcd.print("Up        ");
      break;
    default:      // Undef
      r = S_UNDEF_BIT;
//      lcd.print( "Unknow ");
//      lcd.print( touch_val );
      break;
  }
  return r;
}
/***************************************************************************************************
 * 
 */
void setup() 
{
  Serial.begin(115200);
  gps.begin(9600); // 115200
  //Serial2.begin( 1200 );
  FastLED.addLeds<WS2811, DATA_PIN,RGB>(leds, NUM_LEDS);
  theTime = 130L*1000L;
  currentTime = millis();

  lcd.begin( LCD_COLUMN, LCD_ROW );

  lcd.print("Sezegnin Clock");
  lcd.setCursor(0,1);
  lcd.print("(c)Yves Huguenin");
  lcd.setCursor(0,2);
  lcd.print("2018 Press Menu");

  touch_val     = analogRead( LCD_SWITCH );
  touch_millis  = millis();

  readEEProm();
  setTime( eeprom.saveTime );
}

int flag = 0;

void loop() 
{
unsigned long actTime = millis() / 1000;
unsigned long preTime = currentTime / 1000;

  if( ( actTime ) != ( preTime ) )
  {
    currentTime = millis();
 //   Serial.print(  actTime );
 //   Serial.print(" = ");
 //   Serial.print(  preTime );
 //   Serial.print("\t");
 //   Serial.println( gps.iTOW );
    
    if( race_running )                            // we are in a race
    {
      if( theTime > 999 )
      {
        theTime -= 1000;
      }
      else
      {
        if( --tours > 0 )
        {
           ;
        }
        else
        {
         if( tours == 0 )
          {
            race_ended    = TRUE;
            race_running  = FALSE;
          }
          else
          {
            tours         = _race_laps;
          }
        }
      }
    }
    
    displayClock();
  }
 int touch_read  = analogRead( LCD_SWITCH );

 int  v = touch_val-touch_read;

  if( v < 0 )
    v = -v;

  if( v > 10 )
  {
    touch_millis  = millis();
    touch_val     = touch_read;
  }
  if( ( millis() - touch_millis ) > S_T_PRESSED )
  {
  int t;

    lcd.setCursor(0,0);
    t           = processTouch(  );
    if( touch_bits != t )                     // we pressed some button
    {
      touch_bits  = t;
      switch( t )
      {
        case S_IDLE_BIT:
        case S_UNDEF_BIT:
          break;
        case S_MENU_BIT:                      // we request to enter menu
          if( menu_stage == 0 )               // we are not in menu...
          {
            menu_stage    = M_NORMAL;         // set menu
            current_item  = 1;
            lcd.clear();
            lcd.setCursor( 0,0 ); lcd.print( menu_0001[0] );
            lcd.setCursor( 0,1 ); lcd.print( menu_0001[1] );
          }
          break;
          
        case S_UP_BIT:                        
          switch( menu_stage )               
          {
            case M_NORMAL:
              current_item++;
              if( current_item > 3  )
                current_item = 1;
              lcd.setCursor( 0,1 ); lcd.print( menu_0001[current_item ] );
              break;
            case M_NORMAL | M_SETMODE:
              current_item++;
              if( current_item > 2  )
                current_item = 1;
              lcd.setCursor( 0,2 ); lcd.print( menu_0101[current_item ] );
              break;
          }
          break;
          
        case S_DOWN_BIT:                        
          switch( menu_stage )               
          {
            case M_NORMAL:
              current_item--;
              if( current_item < 1  )
                current_item = 3;
              lcd.setCursor( 0,1 ); lcd.print( menu_0001[current_item ] );
              break;
            case M_NORMAL | M_SETMODE:
              current_item++;
              if( current_item < 1  )
                current_item = 2;
              lcd.setCursor( 0,2 ); lcd.print( menu_0101[current_item ] );
              break;
          }
          break;
          
        case S_ENTER_BIT:                        
          switch( menu_stage )               
          {
            case M_NORMAL:
              switch( current_item )
              {
                case 1:
                  menu_stage |= M_SETMODE;
                  current_item = 1;
                  lcd.setCursor( 0,1 ); lcd.print( menu_0101[0] );
                  lcd.setCursor( 0,2 ); lcd.print( menu_0101[current_item ] );
                  break;
                default:
                  break;
              }
              break;
            case M_NORMAL | M_SETMODE:
              menu_stage ^= M_SETMODE;
              switch( current_item )
              {
                case 1:       // Clock
                  display_mode  = 0;
                  break;
                case 2:       // Race
                  display_mode  = 1;
                  break;
              
              }
              current_item  = 1;
              lcd.clear();
              lcd.setCursor( 0,0 ); lcd.print( menu_0001[0] );
              lcd.setCursor( 0,1 ); lcd.print( menu_0001[ current_item ] );
              break;
          }
          break;
        case S_BACK_BIT:                        
          switch( menu_stage )               
          {
            case M_NORMAL:
                menu_stage = 0;
                lcd.clear();
            //    lcd.setCursor( 0,1 ); lcd.print( menu_0101[0] );
            //    lcd.setCursor( 0,2 ); lcd.print( menu_0101[current_item ] );
              break;
            case M_NORMAL | M_SETMODE:
                menu_stage    = M_NORMAL;
                current_item  = 1;
                lcd.clear();
                lcd.setCursor( 0,0 ); lcd.print( menu_0001[0] );
                lcd.setCursor( 0,1 ); lcd.print( menu_0001[ current_item ] );
              break;
          }
          break;
          
        default:                              // do nothing :-)
          break;
      }
      Serial.print(t);
      Serial.print(" -> ");
      Serial.println( menu_stage );
    }
    else if( t != S_IDLE_BIT )                // do nothing if nothing pressed for now
    {                                         // this is for long pressing
    int d = ( millis() - touch_millis ) % ( S_T_PRESSED * 4 );
      switch( t )
      {
        case S_MENU_BIT:                      // we request to enter menu
          break;
        default:                              // do nothing :-)
          break;
      }
    }
  }
 /******************************************
     if (Serial2.available())
    {
        Serial.write(Serial2.read());
    }

    if (Serial.available())
    {
        Serial2.write(Serial.read());
    }
    //**********************************/
}
