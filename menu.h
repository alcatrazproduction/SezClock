/***************************************************************************************************
 *    File:       menu.h
 *    Creation:   27.02.2018  Yves Hugueniin
 *    
 */

/**************************************************************************************************
 * new Version...
 * Return the selected function
 */

int  processTouch( void )
{
  
int r;

  switch( touch_val / 11 )
  {
    case  8:                          // Back
      r = S_BACK_BIT;
      break;
    case  7:                          // Back-Enter
      r = S_BACK_BIT || S_ENTER_BIT;
      break;
    case  6:                          // Back-Menu
      r = S_BACK_BIT || S_MENU_BIT;
      break;
    case 92:                          // Idle
      r = S_IDLE_BIT;
      break;
    case  16:                         // Down
      r = S_DOWN_BIT;
      break;
    case  14:                         // Up-Down
      r = S_UP_BIT || S_DOWN_BIT;
      break;
    case  11:                         // Menu-Down
      r = S_DOWN_BIT || S_MENU_BIT;
      break;
    case  22:                         // Menu-Enter
      r = S_ENTER_BIT || S_MENU_BIT;
      break;
    case  25:                         // Menu-Up
      r = S_UP_BIT || S_MENU_BIT;
      break;
    case  29:                         // Menu
      r = S_MENU_BIT;
      break;
    case  45:                         // Enter
      r = S_ENTER_BIT;
      break;
    case  62:                         // Up
      r = S_UP_BIT;
      break;
    default:                          // Undef
      r = S_UNDEF_BIT;
      break;
  }
  return r;
}

/***************************************************************************************************
 * 
 */
void processInput( void )
{
 int touch_read  = analogRead( LCD_SWITCH );

 int  v = touch_val-touch_read;

  if( v < 0 )
    v = -v;

  if( v > 20 )
  {
    touch_millis  = millis();
    touch_val     = touch_read;
  }
  if( ( millis() - touch_millis ) > S_T_PRESSED )
  {
  int t;

    lcd.setCursor(0,0);
    t = processTouch(  );
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
            LCD_STRINGAT( 0,0, menu_0001[0] );
            LCD_STRINGAT( 0,1, menu_0001[1] );
          }
          break;
          
        case S_UP_BIT:                        
          switch( menu_stage )               
          {
            case M_NORMAL:
              current_item++;
              if( current_item > 3  )
                current_item = 1;
              LCD_STRINGAT( 0,1, menu_0001[current_item ] );
              break;
            case M_NORMAL | M_SETMODE:
              current_item++;
              if( current_item > 2  )
                current_item = 1;
              LCD_STRINGAT( 0,2, menu_0101[current_item ] );
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
              LCD_STRINGAT( 0,1, menu_0001[current_item ] );
              break;
            case M_NORMAL | M_SETMODE:
              current_item++;
              if( current_item < 1  )
                current_item = 2;
              LCD_STRINGAT( 0,2, menu_0101[current_item ] );
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
                  LCD_STRINGAT( 0,1, menu_0101[0] );
                  LCD_STRINGAT( 0,2, menu_0101[current_item ] );
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
              LCD_STRINGAT( 0,0,  menu_0001[0] );
              LCD_STRINGAT( 0,1,  menu_0001[ current_item ] );
              break;
          }
          break;
        case S_BACK_BIT:                        
          switch( menu_stage )               
          {
            case M_NORMAL:
                menu_stage = 0;
                lcd.clear();
            //    LCD_STRINGAT( 0,1, menu_0101[0] );
            //    LCD_STRINGAT( 0,2, menu_0101[current_item ] );
              break;
            case M_NORMAL | M_SETMODE:
                menu_stage    = M_NORMAL;
                current_item  = 1;
                lcd.clear();
                LCD_STRINGAT( 0,0, menu_0001[0] )
                LCD_STRINGAT( 0,1, menu_0001[ current_item ] )
              break;
          }
          break;
          
        default:                              // do nothing :-)
          break;
      }
      SEND_DBG(t);
      SEND_DBG(" -> ");
      SEND_DBG( menu_stage );
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
}
