#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <string.h>
#include "classes/game.cpp"

#define key1 3 // connect wire 1 to pin 2
#define key2 4 // connect wire 2 to pin 3
#define key3 5 // connect wire 3 to pin 4
#define key4 6 // connect wire 4 to pin 5

#define debounce 20        // ms debounce period to prevent flickering when pressing or releasing the button
#define holdTimeShort 1000 // ms hold period: how long to wait for press+hold event
#define holdTimeLong 2500

// Button variables for press and hold function
int btn1Last = HIGH;          // buffered value of the button's previous state
long btn1DnTime;              // time the button was pressed down
long btn1UpTime;              // time the button was released
boolean btn1IgnoreUp = false; // whether to ignore the button release because the click+hold was triggered

int btn2Last = HIGH;          // buffered value of the button's previous state
long btn2DnTime;              // time the button was pressed down
long btn2UpTime;              // time the button was released
boolean btn2IgnoreUp = false; // whether to ignore the button release because the click+hold was triggered

int btn3Last = HIGH;          // buffered value of the button's previous state
long btn3DnTime;              // time the button was pressed down
long btn3UpTime;              // time the button was released
boolean btn3IgnoreUp = false; // whether to ignore the button release because the click+hold was triggered

int btn4Last = HIGH;          // buffered value of the button's previous state
long btn4DnTime;              // time the button was pressed down
long btn4UpTime;              // time the button was released
boolean btn4IgnoreUp = false; // whether to ignore the button release because the click+hold was triggered

LiquidCrystal lcd(12, 13, 8, 9, 10, 11); /// REGISTER SELECT PIN,ENABLE PIN,D4 PIN,D5 PIN, D6 PIN, D7 PIN

Bomb bomb;
Game game;

int timeBombPlanted;
int timeBombStaredDefusing;
int lap = 0;

void setup()
{
  game = Game();

  analogWrite(6, 75);
  lcd.begin(16, 2);

  pinMode(key1, INPUT_PULLUP); // set pin as input
  pinMode(key2, INPUT_PULLUP); // set pin as input
  pinMode(key3, INPUT_PULLUP); // set pin as input
  pinMode(key4, INPUT_PULLUP); // set pin as input

  game.displayGameMenu(lcd);
}

void loop()
{
  int btn1 = digitalRead(key1);
  int btn2 = digitalRead(key2);
  int btn3 = digitalRead(key3);
  int btn4 = digitalRead(key4);

  // Test for button pressed and store the down time
  if (btn1 == LOW && btn1Last == HIGH && (millis() - btn1UpTime) > long(debounce))
  {
    btn1DnTime = millis();
  }

  // Test for button release and store the up time
  // Basically if button is pressed
  if (btn1 == HIGH && btn1Last == LOW && (millis() - btn1DnTime) > long(debounce))
  {
    // button is pressed
    if (btn1IgnoreUp == false)
    {
      if (game.currentState == IN_MENU)
      {
        game.switchGameMode();
        game.displayGameMenu(lcd);
      }

      else if (game.currentState == IN_BOMB_MODE)
      {
        // What to do if btn1 is pressed in bomb mode
      }

      else if (game.currentState == IN_DOMINATOR_MODE)
      {
        // What to do if btn2 is pressed in dominator mode
      }

      delay(200);
    }
    // button is held
    else
    {
      btn1IgnoreUp = false;
      btn1UpTime = millis();
    }
  }

  // Test for button held down for longer than the hold time
  // Basically if button is held more than hold time
  if (btn1 == LOW && (millis() - btn1DnTime) > long(holdTimeLong))
  {
    if (game.currentState == IN_BOMB_MODE)
    {
      game.bomb.reset();
      game.currentState = IN_MENU;
      game.displayGameMenu(lcd);
    }

    else if (game.currentState == IN_DOMINATOR_MODE)
    {
      game.dominator.reset();
      game.currentState = IN_MENU;
      game.displayGameMenu(lcd);
    }

    delay(200);

    btn1IgnoreUp = true;
    btn1DnTime = millis();
  }

  // Test for button pressed and store the down time
  if (btn2 == LOW && btn2Last == HIGH && (millis() - btn2UpTime) > long(debounce))
  {
    btn2DnTime = millis();

    // if we are in bomb mode, this will mark time of planting sequence start
    // later we will compare this time with actual time of program and udpate progress bar
    if (game.currentState == IN_BOMB_MODE)
    {
      timeBombPlanted = millis();
    }
  }

  // Test for button release and store the up time
  // Basically if button is pressed
  if (btn2 == HIGH && btn2Last == LOW && (millis() - btn2DnTime) > long(debounce))
  {
    // button is pressed
    if (btn2IgnoreUp == false)
    {
      if (game.currentState == IN_MENU)
      {
        // What to do if button 2 is pressed in menu
        if (game.currentMode == BOMB)
        {
          game.currentState = IN_BOMB_MODE;
          game.displayBombMode(lcd);
        }

        if (game.currentMode == DOMINATOR)
        {
          game.currentState = IN_DOMINATOR_MODE;
          game.displayDominatorMode(lcd);
        }
      }
      else if (game.currentState == IN_BOMB_MODE)
      {
        // What to do if button 2 is pressed in bomb mode
      }

      else if (game.currentState == IN_DOMINATOR_MODE)
      {
        // What to do if button 2 is pressed in dominator mode
      }

      else if (game.currentState == BOMB_DEFUSED || game.currentState == BOMB_EXPLODED)
      {
        game.currentState = IN_BOMB_MODE;
        game.displayBombMode(lcd);
      }

      delay(200);
    }
    // button is held
    else
    {
      btn2IgnoreUp = false;
      btn2UpTime = millis();
    }
  }

  if (btn2 == LOW && (millis() - btn2DnTime) > long(holdTimeLong - 2000))
  {
    /* Play BEEP sound*/
  }

  // Test for button held down for longer than the hold time
  // Basically if button is held more than hold time
  if (btn2 == LOW && (millis() - btn2DnTime) > long(holdTimeLong))
  {
    if (game.currentState == IN_MENU)
    {
      // What to do if button 2 is held in menu
    }

    else if (game.currentState == IN_BOMB_MODE)
    {
      game.currentState = BOMB_PLANTED;
      game.bomb.plantBomb(lcd);
      // game.bomb.startTimer(lcd);
    }
    else if (game.currentState == BOMB_PLANTED)
    {
      game.bomb.defuseBomb(lcd);
      game.currentState = BOMB_DEFUSED;
    }

    else if (game.currentState == DOMINATOR_BLUE || game.currentState == DOMINATOR_RED)
    {
      game.currentState = IN_DOMINATOR_MODE;
      game.displayDominatorMode(lcd);
    }

    btn2IgnoreUp = true;
    btn2DnTime = millis();
  }

  // Test for button pressed and store the down time
  if (btn3 == LOW && btn3Last == HIGH && (millis() - btn3UpTime) > long(debounce))
  {
    btn3DnTime = millis();
  }

  // Test for button release and store the up time
  // Basically if button is pressed
  if (btn3 == HIGH && btn3Last == LOW && (millis() - btn3DnTime) > long(debounce))
  {
    // button is pressed
    if (btn3IgnoreUp == false)
    {
      if (game.currentState == IN_MENU)
      {
        /* This button  does nothing when in menu*/
      }

      else if (game.currentState == IN_BOMB_MODE)
      {
        if (game.currentState == BOMB_PLANTED)
        {
          /* This button  does nothing when bomb is planted*/
        }

        if (game.currentState == BOMB_EXPLODED)
        {
          /* This button  does nothing when bomb is planted*/
        }
        else
        {
          game.bomb.decreaseTimerByStep();
          game.displayBombMode(lcd);
        }
      }

      else if (game.currentState == IN_DOMINATOR_MODE)
      {
        if (game.currentState == DOMINATOR_RED)
        {
          /* code */
        }

        if (game.currentState == DOMINATOR_BLUE)
        {
          /* code */
        }
      }

      delay(200);
    }
    // button is held
    else
    {
      btn3IgnoreUp = false;
      btn3UpTime = millis();
    }
  }

  // Test for button held down for longer than the hold time
  // Basically if button is held more than hold time
  if (btn3 == LOW && (millis() - btn3DnTime) > long(holdTimeLong))
  {
    if (game.currentState == IN_DOMINATOR_MODE)
    {
      game.currentState = DOMINATOR_RED;
      game.displayDominatorMode(lcd);
    }

    delay(200);

    btn3IgnoreUp = true;
    btn3DnTime = millis();
  }

  // Test for button pressed and store the down time
  if (btn4 == LOW && btn4Last == HIGH && (millis() - btn4UpTime) > long(debounce))
  {
    btn4DnTime = millis();
  }

  // Test for button release and store the up time
  // Basically if button is pressed
  if (btn4 == HIGH && btn4Last == LOW && (millis() - btn4DnTime) > long(debounce))
  {
    // button is pressed
    if (btn4IgnoreUp == false)
    {
      if (game.currentState == IN_MENU)
      {
        /* This button  does nothing when in menu*/
      }

      else if (game.currentState == IN_BOMB_MODE)
      {
        if (game.currentState == BOMB_PLANTED)
        {
          /* This button  does nothing when bomb is planted*/
        }

        if (game.currentState == BOMB_EXPLODED)
        {
          /* This button  does nothing when bomb is planted*/
        }
        else
        {
          game.bomb.increaseTimerByStep();
          game.displayBombMode(lcd);
        }
      }

      else if (game.currentState == IN_DOMINATOR_MODE)
      {
        if (game.currentState == DOMINATOR_RED)
        {
          /* code */
        }

        if (game.currentState == DOMINATOR_BLUE)
        {
          /* code */
        }
      }

      delay(200);
    }
    // button is held
    else
    {
      btn4IgnoreUp = false;
      btn4UpTime = millis();
    }
  }

  // Test for button held down for longer than the hold time
  // Basically if button is held more than hold time
  if (btn4 == LOW && (millis() - btn4DnTime) > long(holdTimeLong))
  {
    if (game.currentState == IN_DOMINATOR_MODE)
    {
      game.currentState = DOMINATOR_BLUE;
      game.displayDominatorMode(lcd);
    }

    delay(200);

    btn4IgnoreUp = true;
    btn4DnTime = millis();
  }

  if (game.currentState == BOMB_PLANTED)
  {
    delay(1000);
    game.bomb.decreaseCurrentTimerBySecond();
    game.bomb.refreshTimer(lcd);
    if (game.bomb.currentTimerMinutes == 0 && game.bomb.currentTimerSeconds == 0)
    {
      game.currentState = BOMB_EXPLODED;
      game.bomb.explodeBomb(lcd);
    }
  }

  // Dominator time measuring
  if (game.currentState == DOMINATOR_RED)
  {
    if (game.dominator.redTimerHours == 10)
    {
      game.dominator.reset();
      game.currentState = IN_DOMINATOR_MODE;      
    }
    else
    {
      delay(1000);
      game.dominator.resumeRedTimer();      
    }
    game.displayDominatorMode(lcd);
  }
  else if (game.currentState == DOMINATOR_BLUE)
  {
    if (game.dominator.blueTimerHours == 10)
    {
      game.dominator.reset();
      game.currentState = IN_DOMINATOR_MODE;      
    }
    else
    {
      delay(1000);
      game.dominator.resumeBlueTimer();      
    }
    game.displayDominatorMode(lcd);
  }

  btn1Last = btn1;
  btn2Last = btn2;
  btn3Last = btn3;
  btn4Last = btn4;
}