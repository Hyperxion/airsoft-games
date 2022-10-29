#include <LiquidCrystal.h>

class Bomb
{
private:
    // in seconds
    int plantTime = 5;
    int defuseTime = 5;
    int timerStep = 5;

    int timerHighLimitMinutes = 99;
    int timerLowLimitMinutes = 0;

    int timerHighLimitSeconds = 59;
    int timerLowLimitSeconds = 5;

public:
    int timerSeconds;
    int timerMinutes;
    int currentTimerSeconds;
    int currentTimerMinutes;

    Bomb()
    {
        timerSeconds = 45;
        timerMinutes = 0;
        currentTimerSeconds = 45;
        currentTimerMinutes = 0;
    }

    // When counting down after bomb is planted
    void decreaseCurrentTimerBySecond()
    {
        if (currentTimerMinutes > 0 && currentTimerSeconds == 0)
        {
            currentTimerMinutes--;
            currentTimerSeconds = timerHighLimitSeconds;
        }
        else
        {
            currentTimerSeconds--;
        }
    }

    // when setting up timer before bomb is planted
    void increaseTimerByStep()
    {
        if (timerSeconds >= 55)
        {
            timerSeconds = 0;
            if (timerMinutes == 99)
            {
                timerMinutes = 0;
            }
            else
            {
                timerMinutes++;
            }
        }
        else
        {
            timerSeconds += timerStep;
        }

        currentTimerMinutes = timerMinutes;
        currentTimerSeconds = timerSeconds;
    }

    // when setting up timer before bomb is planted
    void decreaseTimerByStep()
    {
        if (timerSeconds == 0 && timerMinutes > 0)
        {
            timerSeconds = 55;
            timerMinutes--;
        }
        else if (timerSeconds <= 5 && timerMinutes == 0)
        {
            timerSeconds = 5;
            timerMinutes = 0;
        }
        else
        {
            timerSeconds -= timerStep;
        }

        currentTimerMinutes = timerMinutes;
        currentTimerSeconds = timerSeconds;
    }

    void reset()
    {
        currentTimerSeconds = timerSeconds;
        currentTimerMinutes = timerMinutes;
    }

    void displayTimer(LiquidCrystal lcd)
    {
        char buffer[17];
        clearLine(lcd, 0);
        sprintf(buffer, "Timer:     %02d:%02d", timerMinutes, timerSeconds);
        lcd.print(buffer);
    }

    void refreshBombTimer(LiquidCrystal lcd)
    {
        char buffer[17];
        clearLine(lcd, 0);
        sprintf(buffer, "Timer:     %02d:%02d", currentTimerMinutes, currentTimerSeconds);
        lcd.print(buffer);
    }

    // nebudem moct pouzit start timer pretoze nebudem vediet prerusit  loop.
    // namiesto toho urobim v main.cpp while loop kde to buudem manazovat stylom kazdy chvilku taha pilku
    // odpocita sekundu, checkne ci je stlaceny defuse
    // odpocita dalsiu sekundu, checkne ci je stlaceny defuse
    // zrazu je stlaceny defuse, vojde do defuse cyklu kde bude checkovat ci sa drzi defuse
    // ak ano, dalej odpocita sekundu a pozrie na chhvilu ci sa stale drzi defuse a doplni defuse progress
    void startTimer(LiquidCrystal lcd)
    {
        currentTimerMinutes = timerMinutes;
        currentTimerSeconds = timerSeconds;

        while (currentTimerSeconds != 0)
        {
            decreaseCurrentTimerBySecond();
            refreshBombTimer(lcd);
            delay(1000);
        }

        clearLine(lcd, 1);
        lcd.print(timerMinutes);
    }

    void plantBomb(LiquidCrystal lcd)
    {
        // There are 16 segments on LCD.
        // This frequency determines how often will one segment be added in progress bar
        int plantTimeInMilis = plantTime * 1000;
        int frequency = plantTimeInMilis / 16;
        int timePassed = 0;
        int cursorCol = 0;

        lcd.setCursor(0, 1);
        lcd.print("                ");

        while (timePassed <= plantTimeInMilis)
        {
            lcd.setCursor(cursorCol, 1);
            lcd.print(">");

            timePassed += frequency;
            cursorCol++;
            delay(frequency);
        }

        clearLine(lcd, 1);
        lcd.print("Bomb planted!   ");
        delay(2000);
        clearLine(lcd, 1);
        lcd.print(">Defuse        ");
    }

    void clearLine(LiquidCrystal lcd, int line)
    {
        lcd.setCursor(0, line);
        lcd.print("                ");
        lcd.setCursor(0, line);
    }
};