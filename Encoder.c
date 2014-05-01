#include <xc.h>
#include "Encoder.h"
#include "MainLibrary.h"
#include "Configuration.h"

//global vars
extern long _lowEdge;
extern long _highEdge;

long EncoderPositionCounter = 0;
long EncoderPrevPositionCounter = 0;
unsigned int currentSpeed = 0;
unsigned int direction = 0;
unsigned char reverse = 0;
unsigned int necessarySpeed = SPEED;
long slowdown_zone = SLOWDOWN_ZONE;

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
    EncoderPositionCounter--;
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
    EncoderPositionCounter++;
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

void InitCounter()
{
    long highEdge = _highEdge;
    float distancePerMark = DISTANCE_PER_MARK;
    EncoderPositionCounter = highEdge/distancePerMark;
}

unsigned char EncReadStartSignal()
{
    unsigned char start = 0;
    TRISBbits.TRISB14 = 0;//set output on RB14
    TRISDbits.TRISD10 = 0;//set output on RD10
    TRISDbits.TRISD0 = 1;//set input on RD0
    LATBbits.LATB14 = 0; // OE1
    LATDbits.LATD10 = 1; // LE3
    Delay(10);
    LATBbits.LATB14 = 0; // OE1
    LATDbits.LATD10 = 0; // LE3
    Delay(10);
    if(PORTDbits.RD0 == 0)
        start = 1;
    else
        start = 0;
    return start;
}

unsigned char EncReadOverZeroSignal()
{
    unsigned char start = 0;
    TRISBbits.TRISB14 = 0;//set output on RB14
    TRISDbits.TRISD10 = 0;//set output on RD10
    TRISDbits.TRISD1 = 1;//set input on RD1
    LATBbits.LATB14 = 0; // OE1
    LATDbits.LATD10 = 1; // LE3
    Delay(10);
    LATBbits.LATB14 = 0; // OE1
    LATDbits.LATD10 = 0; // LE3
    Delay(10);
    if(PORTDbits.RD1 == 0)
        start = 1;
    else
        start = 0;
    return start;
}

void WriteOutputSignals(int data)
{
    data = ~data;
    TRISD = TRISD&0xFF00;
    TRISDbits.TRISD8 = 0;//set output on RD8
    TRISDbits.TRISD9 = 0;//set output on RD9

    TRISDbits.TRISD10 = 0;//set output on RD10
    TRISDbits.TRISD11 = 0;//set output on RD11
    TRISBbits.TRISB14 = 0;//set output on RB14

    LATDbits.LATD10 = 0; // LE3 // set z state
    LATDbits.LATD11 = 0; // LE4
    LATBbits.LATB14 = 1; // OE1

    char buf = data&0x00FF;
    LATDbits.LATD0 = buf&0x01; buf>>=1;
    LATDbits.LATD1 = buf&0x01; buf>>=1;
    LATDbits.LATD2 = buf&0x01; buf>>=1;
    LATDbits.LATD3 = buf&0x01; buf>>=1;
    LATDbits.LATD4 = buf&0x01; buf>>=1;
    LATDbits.LATD5 = buf&0x01; buf>>=1;
    LATDbits.LATD6 = buf&0x01; buf>>=1;
    LATDbits.LATD7 = buf&0x01; buf>>=1;

    LATDbits.LATD8 = 1; // LE1
    Delay(100);
    LATDbits.LATD8 = 0; // LE1
    Delay(100);

    buf = (data>>8)&0x00FF;
    LATDbits.LATD0 = buf&0x01; buf>>=1;
    LATDbits.LATD1 = buf&0x01; buf>>=1;
    LATDbits.LATD2 = buf&0x01; buf>>=1;
    LATDbits.LATD3 = buf&0x01; buf>>=1;
    LATDbits.LATD4 = buf&0x01; buf>>=1;
    LATDbits.LATD5 = buf&0x01; buf>>=1;
    LATDbits.LATD6 = buf&0x01; buf>>=1;
    LATDbits.LATD7 = buf&0x01; buf>>=1;
    LATDbits.LATD9 = 1; // LE2
    Delay(100);
    LATDbits.LATD9 = 0; // LE2
    Delay(100);
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
    long s = EncGetS();

    if(direction == 0 && s <= _lowEdge || direction == 1 && s >= _highEdge)
    {
        T1CONbits.TON = 0;
        necessarySpeed = 0;
    }
}

void ExactStopSensors()
{
    long highEdge = HIGH_SEN_POS;
    long lowEdge = LOW_SEN_POS;
    long exactStopZone = EXACT_STOP_ZONE;
    long s = EncGetS();
    if(s <= lowEdge && s >= lowEdge - exactStopZone)
    {
         WriteOutputSignals(2);
    }
    else if(s >= highEdge && s <= highEdge + exactStopZone)
    {
        WriteOutputSignals(1);
    }
    else
        WriteOutputSignals(3);
}

void EncSlowdownControl()
{
    //long slowdown_zone = SLOWDOWN_ZONE;
    long s = EncGetS();

    if( (direction == 0 && s <= _lowEdge + slowdown_zone) ||
            (direction == 1 && s >= _highEdge - slowdown_zone))
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

void TrySetOverRise()
{
    long lowSenPos = LOW_SEN_POS;
    if(EncReadOverZeroSignal() == 1 && (_lowEdge > lowSenPos - 1000))
    {
        _lowEdge -= 1000;
    }
    else if(EncReadOverZeroSignal() == 0)
        _lowEdge = lowSenPos;
}

void ParseTPDO1(unsigned int sid, unsigned char* data)
{
    if((sid&0x780)!=0x180)//if it's not TPDO1
        return;
    long s = 0;
    long* distance = 0;
    char buf[4] = {0, data[3], data[4], data[5]};
    distance = buf;
    *distance=(*distance)/256;
    if((sid&0x3) == 0x1)
        s = *distance;
    if((sid&0x3) == 0x2)
        s = *distance;
    if((sid&0x3) == 0x3)
        s = *distance;
    EncoderPositionCounter = s/DISTANCE_PER_MARK;
}
