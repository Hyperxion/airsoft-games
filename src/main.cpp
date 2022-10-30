#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <string.h>
#include "classes/game.cpp"

#define key1 3 // connect wire 1 to pin 2
#define key2 4 // connect wire 2 to pin 3
#define key3 5 // connect wire 3 to pin 4
#define key4 6 // connect wire 4 to pin 5

#define debounce 20   // ms debounce period to prevent flickering when pressing or releasing the button
#define holdTime 1000 // ms hold period: how long to wait for press+hold event

// Button variables for press and hold function
// int buttonVal = LOW;      // value read from button
int button2SLast = HIGH;  // buffered value of the button's previous state
long btn2SDnTime;         // time the button was pressed down
long btn2SUpTime;         // time the button was released
boolean ignoreUp = false; // whether to ignore the button release because the click+hold was triggered

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
  int key1S = digitalRead(key1);
  int key2S = digitalRead(key2);
  int key3S = digitalRead(key3);
  int key4S = digitalRead(key4);

  /*
   Defines what button one does in different game states when pressed
  */
  if (!key1S)
  {
    if (game.currentState == IN_MENU)
    {
      game.switchGameMode();
      game.displayGameMenu(lcd);
    }

    else if (game.currentState == IN_BOMB_MODE)
    {
      if (game.currentState == BOMB_PLANTED)
      {
        /* code */
      }

      if (game.currentState == BOMB_EXPLODED)
      {
        /* code */
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

      if (game.currentState == DOMINATOR_NEUTRAL)
      {
        /* code */
      }
    }

    delay(200);
  }

  // Source for press and hold: http://jmsarduino.blogspot.com/2009/05/click-for-press-and-hold-for-b.html

  /*
   Defines what button two does in different game states when pressed
  */
  // Test for button pressed and store the down time
  if (key2S == LOW && button2SLast == HIGH && (millis() - btn2SUpTime) > long(debounce))
  {
    btn2SDnTime = millis();

    // if we are in bomb mode, this will mark time of planting sequence start
    // later we will compare this time with actual time of program and udpate progress bar
    if (game.currentState == IN_BOMB_MODE)
    {
      timeBombPlanted = millis();
    }
  }

  // Test for button release and store the up time
  // Basically if button is pressed
  if (key2S == HIGH && button2SLast == LOW && (millis() - btn2SDnTime) > long(debounce))
  {
    // button is pressed
    if (ignoreUp == false)
    {
      if (game.currentState == IN_MENU)
      {
        // What to do if button 2 is pressed in menu
        if (game.currentMode == BOMB)
        {
          game.currentState = IN_BOMB_MODE;
          game.displayBombMode(lcd);
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

      delay(200);
    }
    // button is held
    else
    {
      ignoreUp = false;
      btn2SUpTime = millis();
    }
  }

  // Test for button held down for longer than the hold time
  // Basically if button is held more than hold time
  if (key2S == LOW && (millis() - btn2SDnTime) > long(holdTime))
  {
    if (game.currentState == IN_MENU)
    {
      // What to do if button 2 is held in menu
    }

    else if (game.currentState == IN_BOMB_MODE)
    {
      // What to do if button 2 is held in bomb mode
      while (key2S == LOW && (millis() - btn2SDnTime) > long(holdTime))
      {       
        game.currentState = BOMB_PLANTED;
        game.bomb.plantBomb(lcd);
        game.bomb.startTimer(lcd);
      }
      game.bomb.clearLine(lcd, 1);
      lcd.print("Bomb planted!   ");
    }

    else if (game.currentState == IN_DOMINATOR_MODE)
    {
      // What to do if button 2 is held in dominator mode
    }

    ignoreUp = true;
    btn2SDnTime = millis();
  }

  /*
   Defines what button three does in different game states when pressed
  */
  if (!key3S)
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

      if (game.currentState == DOMINATOR_NEUTRAL)
      {
        /* code */
      }
    }

    delay(200);
  }

  /*
   Defines what button four does in different game states when pressed
  */
  if (!key4S)
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

      if (game.currentState == DOMINATOR_NEUTRAL)
      {
        /* code */
      }
    }

    delay(200);
  }

  button2SLast = key2S;
}