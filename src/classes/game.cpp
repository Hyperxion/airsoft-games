#include <LiquidCrystal.h>
#include "classes/bomb.cpp"
#include "classes/dominator.cpp"

enum GAME_STATES
{
    IN_MENU,
    IN_BOMB_MODE,
    BOMB_PLANTED,
    BOMB_EXPLODED,
    IN_DOMINATOR_MODE,
    DOMINATOR_RED,
    DOMINATOR_BLUE
};

enum GAME_MODES
{
    BOMB,
    DOMINATOR
};

class Game
{
public:
    Bomb bomb;
    Dominator dominator;
    GAME_STATES currentState;
    GAME_MODES currentMode;

    Game()
    {
        bomb = Bomb();
        dominator = Dominator();
        currentState = IN_MENU;
        currentMode = BOMB;
    }

    void switchGameMode()
    {
        if (currentMode == BOMB)
        {
            currentMode = DOMINATOR;
        }
        else
        {
            currentMode = BOMB;
        }
    }

    void displayGameMenu(LiquidCrystal lcd)
    {
        if (currentMode == BOMB)
        {
            lcd.clear();
            lcd.print(">Bomb");
            lcd.setCursor(0, 1);
            lcd.print(" Dominator");
        }
        else if (currentMode == DOMINATOR)
        {
            lcd.clear();
            lcd.print(" Bomb");
            lcd.setCursor(0, 1);
            lcd.print(">Dominator");
        }
    }

    void displayBombMode(LiquidCrystal lcd)
    {
        bomb.displayTimer(lcd);
        lcd.setCursor(0, 1);
        lcd.print(">Plant Bomb");
    }

    void displayDominatorMode(LiquidCrystal lcd)
    {
        dominator.displayTimer(lcd);
        //clearLine(lcd, 1);
        lcd.setCursor(0, 1);
        if (currentState == IN_DOMINATOR_MODE)
        {
            lcd.print("Red   >Ne   Blue");
        }
        else if (currentState == DOMINATOR_BLUE)
        {
            lcd.print("Red   Ne   >Blue");
        }
        else
        {
            lcd.print(">Red   Ne   Blue");
        }
    }

    void clearLine(LiquidCrystal lcd, int line)
    {
        lcd.setCursor(0, line);
        lcd.print("                ");
        lcd.setCursor(0, line);
    }
};