
#include <stdio.h>
#include <time.h>

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


#include  "FastLED.h"


#include  "board.h"
#include  "function.h"
#include  "globals.h"
#include  "esp.h"
#include  "menu.h"



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
      
    SEND_DBG( theTime / 1000 );
    SEND_DBG("\t Time :");
    SEND_DBG(theTm.tm_hour);
        writeDigit( HOURS_10    , theTm.tm_hour   / 10 );
        writeDigit( HOURS_1     , theTm.tm_hour   % 10 );
    SEND_DBG(":");
    SEND_DBG(theTm.tm_min);
        writeDigit( MINUTES_10  , theTm.tm_min    / 10 );
        writeDigit( MINUTES_1   , theTm.tm_min    % 10 );
    SEND_DBG(":");
    SEND_DBG(theTm.tm_sec / 10);
        writeDigit( SECONDS_10  , theTm.tm_sec    / 10 );
    SEND_DBG(theTm.tm_sec % 10);
        writeDigit( SECONDS_1   , theTm.tm_sec    % 10 );
    SEND_DBG(" ");
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


/***************************************************************************************************
 * 
 */
void setup() 
{
  Serial.begin(115200);
  gps.begin(9600); // 115200
  ESP_SERIAL.begin( 115200 );
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
/***************************************************************************************************
 * 
 */
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
  processInput();
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

