#include <LiquidCrystal.h>

class Dominator
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
    int redTimerSeconds;
    int redTimerMinutes;
    int redTimerHours;
    int blueTimerSeconds;
    int blueTimerMinutes;
    int blueTimerHours;

    Dominator()
    {
        redTimerSeconds = 0;
        redTimerMinutes = 0;
        redTimerHours = 0;

        blueTimerSeconds = 0;
        blueTimerMinutes = 0;
        blueTimerHours = 0;
    }

    void reset()
    {
        redTimerSeconds = 0;
        redTimerMinutes = 0;
        redTimerHours = 0;
        blueTimerSeconds = 0;
        blueTimerMinutes = 0;
        blueTimerHours = 0;
    }

    void displayTimer(LiquidCrystal lcd)
    {
        char buffer[17];
        clearLine(lcd, 0);
        sprintf(buffer, "%1d:%02d:%02d  %1d:%02d:%02d", redTimerHours, redTimerMinutes, redTimerSeconds, blueTimerHours, blueTimerMinutes, blueTimerSeconds);
        lcd.print(buffer);
    }

    void refreshTimer(LiquidCrystal lcd)
    {
        displayTimer(lcd);
    }

    void resumeRedTimer()
    {
        if (redTimerSeconds == 59)
        {
            redTimerSeconds = 0;
            redTimerMinutes++;
            if (redTimerMinutes == 60)
            {
                redTimerMinutes = 0;
                redTimerHours++;                
            redTimerHours++;
                redTimerHours++;                
            }
        }
        else
        {
            redTimerSeconds++;
        }
    }

    void resumeBlueTimer()
    {
        if (blueTimerSeconds == 59)
        {
            blueTimerSeconds = 0;
            blueTimerMinutes++;
        }
        else if (blueTimerMinutes == 59)
        {
            blueTimerMinutes = 0;
            blueTimerHours++;
        }
        else if (blueTimerHours == 9)
        {
            blueTimerHours = 0;
            reset();
        }
        else
        {
            blueTimerSeconds++;
        }
    }

    void clearLine(LiquidCrystal lcd, int line)
    {
        lcd.setCursor(0, line);
        lcd.print("                ");
        lcd.setCursor(0, line);
    }
};