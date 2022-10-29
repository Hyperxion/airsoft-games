#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <string.h>
#include "classes/game.cpp"

#define key1 3 // connect wire 1 to pin 2
#define key2 4 // connect wire 2 to pin 3
#define key3 5 // connect wire 3 to pin 4
#define key4 6 // connect wire 4 to pin 5

LiquidCrystal lcd(12, 13, 8, 9, 10, 11); /// REGISTER SELECT PIN,ENABLE PIN,D4 PIN,D5 PIN, D6 PIN, D7 PIN

Bomb bomb;
Game game;

// void clearLine(int line);

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
    In menu: changes game mode
    In any game mode: goes back to menu
  */
  if (!key1S)
  {
    if (game.currentState == IN_MENU)
    {
      game.switchGameMode();
      game.displayGameMenu(lcd);
    }
    else if (game.currentState != IN_MENU)
    {
      if (game.currentMode == BOMB)
      {
        game.bomb.reset();
      }
      else if (game.currentMode == DOMINATOR)
      {
        // not implemented yet
        // dominator.reset();
      }

      game.currentState = IN_MENU;
      game.displayGameMenu(lcd);
    }

    delay(200);
  }

  /*
    In menu: Chooses game mode and enter it's menu
    In game mode:
        - BOMB:
            - Not Planted: will plant bomb (hold)
            - Planted: will defuse bomb (hold)
  */
  if (!key2S)
  {
    if (game.currentState == IN_MENU)
    {
      if (game.currentMode == BOMB)
      {
        game.currentState = IN_BOMB_MODE;
        game.displayBombMode(lcd);
      }
    }
    else if (game.currentState == IN_BOMB_MODE)
    {
      game.currentState = BOMB_PLANTED;
      game.bomb.plantBomb(lcd);
      game.bomb.startTimer(lcd);
    }

    delay(500);
  }

  if (!key3S)
  {

    if (game.currentMode == BOMB)
    {

      if (game.currentState == IN_BOMB_MODE)
      {
        game.bomb.decreaseTimerByStep();
        game.displayBombMode(lcd);
        delay(150);
      }
    }
  }

  if (!key4S)
  {
    if (game.currentMode == BOMB)
    {
      if (game.currentState == IN_BOMB_MODE)
      {
        game.bomb.increaseTimerByStep();
        game.displayBombMode(lcd);
        delay(150);
      }
    }
  }
}