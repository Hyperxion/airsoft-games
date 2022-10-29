#include <LiquidCrystal.h>
#include "classes/bomb.cpp"

enum GAME_STATES
{
    IN_MENU,
    IN_BOMB_MODE,
    BOMB_PLANTED,
    BOMB_EXPLODED,
    IN_DOMINATOR_MODE,
    DOMINATOR_RED,
    DOMINATOR_BLUE,
    DOMINATOR_NEUTRAL
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
    GAME_STATES currentState;
    GAME_MODES currentMode;

    Game()
    {
        bomb = Bomb();
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
};