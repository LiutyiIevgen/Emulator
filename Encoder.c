#include <xc.h>
#include "Encoder.h"
#include "MainLibrary.h"
#include "Configuration.h"

long EncoderPositionCounter = 0;
long EncoderPrevPositionCounter = 0;
unsigned int currentSpeed = 0;
unsigned int direction = 0;
unsigned char reverse = 0;
unsigned int necessarySpeed = SPEED;

void EncForwardDirectionStroke()
{
    long delay = 15;
    TRISBbits.TRISB0 = 0;//set output on RB0
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 0;
    //
    LATBbits.LATB0 = 0; //B
    LATBbits.LATB1 = 0; //A
    LATBbits.LATB2 = 0;
    LATBbits.LATB3 = 0;
    LATBbits.LATB4 = 0;
    LATBbits.LATB5 = 0;
    Delay(delay);
    LATBbits.LATB0 = 0;
    LATBbits.LATB1 = 1;
    LATBbits.LATB2 = 0;
    LATBbits.LATB3 = 1;
    LATBbits.LATB4 = 0;
    LATBbits.LATB5 = 1;
    Delay(delay);
    LATBbits.LATB0 = 1;
    LATBbits.LATB1 = 1;
    LATBbits.LATB2 = 1;
    LATBbits.LATB3 = 1;
    LATBbits.LATB4 = 1;
    LATBbits.LATB5 = 1;
    Delay(delay);
    LATBbits.LATB0 = 1;
    LATBbits.LATB1 = 0;
    LATBbits.LATB2 = 1;
    LATBbits.LATB3 = 0;
    LATBbits.LATB4 = 1;
    LATBbits.LATB5 = 0;
    Delay(delay);
    LATBbits.LATB0 = 0;
    LATBbits.LATB1 = 0;
    LATBbits.LATB2 = 0;
    LATBbits.LATB3 = 0;
    LATBbits.LATB4 = 0;
    LATBbits.LATB5 = 0;
    EncoderPositionCounter++;
}

void EncReverseDirectionStroke()
{
    long delay = 15;
    TRISBbits.TRISB0 = 0;//set output on RB0
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 0;
    //
    LATBbits.LATB0 = 1; //B
    LATBbits.LATB1 = 0; //A
    LATBbits.LATB2 = 1;
    LATBbits.LATB3 = 0;
    LATBbits.LATB4 = 1;
    LATBbits.LATB5 = 0;
    Delay(delay);
    LATBbits.LATB0 = 1;
    LATBbits.LATB1 = 1;
    LATBbits.LATB2 = 1;
    LATBbits.LATB3 = 1;
    LATBbits.LATB4 = 1;
    LATBbits.LATB5 = 1;
    Delay(delay);
    LATBbits.LATB0 = 0;
    LATBbits.LATB1 = 1;
    LATBbits.LATB2 = 0;
    LATBbits.LATB3 = 1;
    LATBbits.LATB4 = 0;
    LATBbits.LATB5 = 1;
    Delay(delay);
    LATBbits.LATB0 = 0;
    LATBbits.LATB1 = 0;
    LATBbits.LATB2 = 0;
    LATBbits.LATB3 = 0;
    LATBbits.LATB4 = 0;
    LATBbits.LATB5 = 0;
    Delay(delay);
    LATBbits.LATB0 = 1;
    LATBbits.LATB1 = 0;
    LATBbits.LATB2 = 1;
    LATBbits.LATB3 = 0;
    LATBbits.LATB4 = 1;
    LATBbits.LATB5 = 0;
    EncoderPositionCounter--;
}

int EncCountV()
{
    long difference = EncoderPositionCounter - EncoderPrevPositionCounter;
    difference *= DISTANCE_PER_MARK;
    EncoderPrevPositionCounter = EncoderPositionCounter;
    return difference/0.01;
}
long EncGetS()
{
    return EncoderPositionCounter*DISTANCE_PER_MARK;
}
unsigned char EncReadStartSignal()
{
    unsigned char start = 0;
    TRISBbits.TRISB14 = 0;//set output on RB14
    TRISDbits.TRISD8 = 0;//set output on RD8
    TRISDbits.TRISD0 = 1;//set input on RD0
    LATBbits.LATB14 = 0; // OE1
    LATDbits.LATD8 = 1; // LE1
    Delay(100);
    LATBbits.LATB14 = 0; // OE1
    LATDbits.LATD8 = 0; // LE1
    Delay(10);
    if(PORTDbits.RD0 == 0)
        start = 1;
    else
        start = 0;
    return start;
}

void EncSpeedControl()
{
    unsigned int speedIncrease = SPEED_INCREASE;
    float distancePerMark = DISTANCE_PER_MARK;
    int rez = currentSpeed - necessarySpeed;
    if(rez==0)
        return;
    currentSpeed += rez < 0 ? speedIncrease : -speedIncrease;
    PR1 = 24000000/(currentSpeed/distancePerMark);
}

void EncStopControl()
{
    long distance = DISTANCE;
    long s = EncGetS();

    if(direction == 0 && s >= distance || direction == 1 && s <= 0)
    {
        T1CONbits.TON = 0;
        necessarySpeed = 0;
        //T2CONbits.TON = 0;
    }
}

void EncSlowdownControl()
{
    long distance = DISTANCE;
    long slowdown_zone = 52000;
    long s = EncGetS();

    if( (direction == 0 && s >= distance - slowdown_zone) ||
            (direction == 1 && s <= slowdown_zone))
        necessarySpeed = 400;

}

void EncStartControl()
{
    if(T1CONbits.TON)
        return;
    direction = 1 - direction;
    necessarySpeed = SPEED;
    Delay(5000000);
    T1CONbits.TON = 1;
}

unsigned int EncGetDirection()
{
    return direction;
}

/*void EncSpeedControll()
{
    //int v = EncCountV();

    unsigned int speed_increase = SPEED_INCREASE;
    long distance = DISTANCE;
    float distance_per_mark = DISTANCE_PER_MARK;
    long slowdown_zone = 52000;
    long s = EncGetS();

    reverse = EncReadReverseSignal();
    if(reverse == 1)
    {
        if(direction == 0) // forward
        {
            if(s >= distance - slowdown_zone)
            {
                if(current_speed <= 400)
                {
                    if(s >= distance)
                    {
                        T1CONbits.TON = 0;
                        T2CONbits.TON = 0;
                        Delay(20000000);
                        direction = 1;
                        T1CONbits.TON = 1;
                        T2CONbits.TON = 1;
                    }
                }
                else
                {
                    current_speed -= speed_increase;
                    PR1 = 24000000/(current_speed/distance_per_mark);
                }
                return;
            }
            if(current_speed <= speed)
            {
                current_speed += speed_increase;
                PR1 = 24000000/(current_speed/distance_per_mark);
            }
        }
        else // reverse
        {
            if(s <= slowdown_zone)
            {
                if(current_speed <= 400)
                {
                    if(s <= 0)
                    {
                        T1CONbits.TON = 0;
                        T2CONbits.TON = 0;
                        Delay(20000000);
                        direction = 0;
                        T1CONbits.TON = 1;
                        T2CONbits.TON = 1;
                    }
                }
                else
                {
                    current_speed -= speed_increase;
                    PR1 = 24000000/(current_speed/distance_per_mark);
                }
                return;
            }
            if(current_speed <= speed)
            {
                current_speed += speed_increase;
                PR1 = 24000000/(current_speed/distance_per_mark);
            }
        }
    }
} */