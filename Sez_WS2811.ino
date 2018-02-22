#include <Adafruit_NeoPixel.h>


#include <FastLED.h>


#include "board.h"
#include "function.h"


// Globals

CRGB          leds[NUM_LEDS];       // Led array
long          color           = 0xffffff;  // Color of LED
unsigned long theTime,currentTime;
int           tours = -1;

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
struct tm       theTm2;
unsigned long   actTime = theTime / 1000;

 memset(&theTm,0,sizeof( theTm ) );
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
      writeDots(  0           , 0x0f0f0f );
      writeDots(  1           , 0x0f0f0f );
      writeDots(  2           , 0x0f0f0f );
      writeDots(  3           , 0x0f0f0f );
    }    
  }
  FastLED.show();
 // pixels.show();
}

void setup() 
{
  Serial.begin(9800);
  FastLED.addLeds<WS2811, DATA_PIN,RGB>(leds, NUM_LEDS);
  theTime = 130L*1000L;
  currentTime = millis();
  
 // pixels.begin();
 // pixels.show();
 // pinMode( 3, OUTPUT );
}

int flag = 0;

void loop() 
{
unsigned long actTime = millis() / 1000;
unsigned long preTime = currentTime / 1000;
  if( ( actTime ) != ( preTime ) )
  {
    currentTime = millis();
    Serial.print(  actTime );
    Serial.print(" = ");
    Serial.print(  preTime );
    Serial.print("\t");
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
          theTime = 20L*60L*1000L;
          tours   = -2;
        }
        else
        {
          tours = 2;
        }
      }
    }
    displayClock();
  }
}
