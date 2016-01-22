#include <xc.h>
#include "Encoder.h"
#include "MainLibrary.h"
#include "ADC.h"

extern long _lowEdge;
extern long _highEdge;
extern long halfS;
extern long halfV;
extern int Vstart;
extern int halfVstart;
extern int R;
extern int Amax;
extern int Vmax;
extern double distancePerMark;
extern long exactStopZone;
extern long slowdown_zone;
extern int RevisionSpeed;
extern int SpeedUpZone;
extern int RevisionExtraEdge;

long EncoderPositionCounter = 0;
long EncoderPrevPositionCounter = 0;
int currentSpeed = 0;
int currentAccel = 0;

unsigned int direction = 0;
unsigned char reverse = 0;
unsigned int necessarySpeed;// = Vmax;
//unsigned int dot_speed = DOT_SPEED;
unsigned char handmode = 0;

long prevV[3] = {0, 0, 0};
int slowdown_state = 0;
int stop = 0;
int start = 0;
char regim1 = 0;
char regim2 = 0;
int wasExactStop = 0;

char speedup = 0;
char slowdown = 0;
long flatcnt=0,flatcnt1=0;
long new_pr=0;

unsigned short load = 0;
char autoload_allowed = 0;


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
    difference *= distancePerMark;
    EncoderPrevPositionCounter = EncoderPositionCounter;
    return difference/0.01;
}
long EncGetS()
{
    return EncoderPositionCounter*distancePerMark;
}

void InitCounter()
{
    long highEdge = _highEdge;
    float distancePerMark = distancePerMark;
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

unsigned char EncReadHandModeSignal()
{
    TRISBbits.TRISB14 = 0;//set output on RB14
    TRISDbits.TRISD10 = 0;//set output on RD10
    TRISDbits.TRISD2 = 1;//set input on RD2
    LATBbits.LATB14 = 0; // OE1
    LATDbits.LATD10 = 1; // LE3
    Delay(10);
    LATBbits.LATB14 = 0; // OE1
    LATDbits.LATD10 = 0; // LE3
    Delay(10);
    if(PORTDbits.RD2 == 0)
        handmode = 1;
    else
        handmode = 0;
    return handmode;
}

void EncReadDirectionSignal()
{
    if(EncReadHandModeSignal() == 1)
    {
        //unsigned int dir = 0;
        TRISBbits.TRISB14 = 0;//set output on RB14
        TRISDbits.TRISD10 = 0;//set output on RD10
        TRISDbits.TRISD3 = 1;//set input on RD3
        TRISDbits.TRISD4 = 1;//set input on RD4
        LATBbits.LATB14 = 0; // OE1
        LATDbits.LATD10 = 1; // LE3
        Delay(10);
        LATBbits.LATB14 = 0; // OE1
        LATDbits.LATD10 = 0; // LE3
        Delay(10);
        if(PORTDbits.RD3 == 0 && PORTDbits.RD4 == 1)
        {
            direction = 0;
            stop = 0;
        }
        else if(PORTDbits.RD3 == 1 && PORTDbits.RD4 == 0)
        {
            direction = 1;
            stop = 0;
        }
        else if(PORTDbits.RD3 == 1 && PORTDbits.RD4 == 1)
            stop = 1;
    }
}

unsigned char EncIsDirectionChosen()
{
        TRISBbits.TRISB14 = 0;//set output on RB14
        TRISDbits.TRISD10 = 0;//set output on RD10
        TRISDbits.TRISD3 = 1;//set input on RD3
        TRISDbits.TRISD4 = 1;//set input on RD4
        LATBbits.LATB14 = 0; // OE1
        LATDbits.LATD10 = 1; // LE3
        Delay(10);
        LATBbits.LATB14 = 0; // OE1
        LATDbits.LATD10 = 0; // LE3
        Delay(10);
        if(PORTDbits.RD3 == 1 && PORTDbits.RD4 == 1)
            return 0;
        return 1;
}

unsigned char EncReadPuskSignal()
{
    unsigned char pusk = 0;
    TRISBbits.TRISB14 = 0;//set output on RB14
    TRISDbits.TRISD10 = 0;//set output on RD10
    TRISDbits.TRISD5 = 1;//set input on RD5
    LATBbits.LATB14 = 0; // OE1
    LATDbits.LATD10 = 1; // LE3
    Delay(10);
    LATBbits.LATB14 = 0; // OE1
    LATDbits.LATD10 = 0; // LE3
    Delay(10);
    if(PORTDbits.RD5 == 0)
        pusk = 1;
    else
        pusk = 0;
    return pusk;
}

void SetLoadSignalState()
{
    TRISBbits.TRISB14 = 0;//set output on RB14
    TRISDbits.TRISD10 = 0;//set output on RD10
    TRISDbits.TRISD1 = 1;//set input on RD5
    LATBbits.LATB14 = 0; // OE1
    LATDbits.LATD10 = 1; // LE3
    Delay(10);
    LATBbits.LATB14 = 0; // OE1
    LATDbits.LATD10 = 0; // LE3
    Delay(10);
    if(PORTDbits.RD1 == 0)
        autoload_allowed = 1;
    else
        autoload_allowed = 0;
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
    if(!start) return;

    if(!handmode)
    {
        long currS = labs(EncGetS());

        char cond=1;

        if(((direction==0)&&(currS > halfS))||((direction==1)&&(currS < halfS)))
           cond=0;

        if(cond)
        {
            if(speedup)
               flatcnt++;
            else
                SpeedUp(currS);
        }
        else
        {
            if(flatcnt<=0)
                SlowDown();
            else
                flatcnt--;
        }
    
    currentSpeed+=currentAccel;
    }
    
   //double distancePerMark = DISTANCE_PER_MARK;
    double s_per_mark = (abs(currentSpeed)/distancePerMark);
    s_per_mark = 1/s_per_mark;
    unsigned long pr = (s_per_mark/0.0416)*1000000;
   
    //unsigned long pr = 24000000/(currentSpeed/distancePerMark);
    _T5IE = 0;
    TMR5HLD = 0;
    TMR4 = 0;
    PR5 = (pr>>16);
    PR4 = (pr&0x0000FFFF);
    _T5IE = 1;
   /* unsigned int speedIncrease = SPEED_INCREASE;
    float distancePerMark = DISTANCE_PER_MARK;
    int rez = currentSpeed - necessarySpeed;
    if(rez==0)
        return;
    currentSpeed += rez < 0 ? speedIncrease : -speedIncrease;
    PR1 = 24000000/(currentSpeed/distancePerMark);*/
}

void SpeedUp(long currS)
{    
    long speedup_point = direction<1 ?_highEdge:_lowEdge;
    speedup_point = labs(speedup_point)-SpeedUpZone;;

    int absV = abs(currentSpeed);

    if(absV < (halfV+halfVstart))
        if(abs(currentAccel) < Amax)
        {
            if(currS > speedup_point)
            {
              if(absV<Vstart)
                {currentSpeed=direction<1 ? Vstart : -Vstart; }
            }else 
              currentAccel+=direction<1 ? R : -R;
        }
        else
          flatcnt1++;
    else
        if(flatcnt1==0)
            if(abs(currentAccel)<=0)
            {
                currentAccel=0;
                speedup=1;
                flatcnt=0;
                flatcnt1=0;
            }
            else
                currentAccel-= direction<1 ? R : -R;
        else
            flatcnt1--;
}

void SlowDown()
{
    if(abs(currentSpeed) < (halfV+halfVstart))
    {
        if(flatcnt1==0)
            if(currentAccel==0)
            {
                currentAccel=0;
                slowdown = 1;
            }
            else
                currentAccel+= direction<1 ? R : -R;
        else
            flatcnt1--;
    }
    else
    {
        if(abs(currentAccel) >= Amax)
            flatcnt1++;
        else
            currentAccel-= direction<1 ? R : -R;
        }
}

void EncStopControl()
{
    long s = EncGetS();

    if(stop == 1 && EncReadHandModeSignal() == 1)
    {
        T4CONbits.TON = 0;
        necessarySpeed = 0;
        start=0;
        currentSpeed = 0;
        currentAccel = 0;
    }
    if((direction == 0 && s <= _lowEdge) || (direction == 1 && s >= _highEdge))
    {
        T4CONbits.TON = 0;
        necessarySpeed = 0;

        currentSpeed = 0;//111
        currentAccel = 0;//!!!

        wasExactStop = 1;
        start=0;
        Delay(200);
    }
    if(EncIsDirectionChosen() == 0 && EncReadHandModeSignal() == 0)
    {
        T4CONbits.TON = 0;
        necessarySpeed = 0;
        start=0;
        currentSpeed = 0;//!!
        currentAccel = 0;
    }
}

void ExactStopSensors()
{
   // long highEdge = HIGH_SEN_POS;
   // long lowEdge = LOW_SEN_POS;
    long s = EncGetS();

    if(!autoload_allowed)
        load = 80;

    if(s <= _lowEdge && s >= _lowEdge - exactStopZone)
    {
        if(load < 20)
        {
         WriteOutputSignals(2|(1<<3)|(0<<4));
         load++;
        }
        else if(load < 60)
        {
         WriteOutputSignals(2|(1<<3)|(0<<4)|(1<<5));
         load++;
        }
        else if(load < 80)
        {
         WriteOutputSignals(2|(1<<3)|(0<<4)|(0<<5));
         load++;
        }
        if(load==80)
         WriteOutputSignals(2|(1<<4));
    }
    else if(s >= _highEdge && s <= _highEdge + exactStopZone)
    {
         if(load < 20)
         {
          WriteOutputSignals(1|(1<<3)|(0<<4));
          load++;
         }
         else if(load < 60)
         {
              WriteOutputSignals(1|(1<<3)|(0<<4)|(1<<5));
             load++;
         }
         else if(load < 80)
         {
          WriteOutputSignals(1|(1<<3)|(0<<4)|(0<<5));
          load++;
         }
        if(load==80)
         WriteOutputSignals(1|(1<<4));
       // WriteOutputSignals(1|(1<<4));
    }
    else
    {
        WriteOutputSignals(3|(1<<4));
        load = 0;
    }
}
/*void ExactStopSensors()
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
}*/

void EncSlowdownControl()
{
    if(EncReadHandModeSignal() == 0)
    {
        long s = EncGetS();
        if((direction == 0 && s <= _lowEdge + slowdown_zone) || (direction == 1 && s >= _highEdge - slowdown_zone))
            necessarySpeed = 400;
    }
}

void EncStartControl()
{
    if(T4CONbits.TON)
        return;
    if(EncReadHandModeSignal() == 1 && stop == 0)
    {
        Delay(100000);
        T4CONbits.TON = 1;
        start=1;
    }
    else if(EncReadHandModeSignal() == 0 && EncIsDirectionChosen() == 1) //if(EncReadPuskSignal() == 1)
    {
        necessarySpeed = Vmax;
        if(wasExactStop == 1)
        {
            direction = 1 - direction;
            wasExactStop = 0;
        }
        Delay(100000);
        T4CONbits.TON = 1;
        start=1;
        speedup=0;
        slowdown=0;
    }
}

unsigned int EncGetDirection()
{
    return direction;
}

unsigned int EncGetStop()
{
    return stop;
}

unsigned int EncGetStart()
{
    return start;
}

void TrySetOverRise()
{
    long lowSenPos = _lowEdge;//LOW_SEN_POS;
    long highSenPos = _highEdge;//HIGH_SEN_POS;

    if((regim1 == 0 && regim2 == 0) && (_lowEdge > lowSenPos - RevisionExtraEdge))
    {
         _lowEdge -= RevisionExtraEdge;
    }
    else if (regim1 != 0 || regim2 != 0)
        _lowEdge = lowSenPos;

    if((regim1 == 0 && regim2 == 0) && (_highEdge < highSenPos + RevisionExtraEdge))
    {
        _highEdge += RevisionExtraEdge;
    }
    else if (regim1 != 0 || regim2 != 0)
        _highEdge = highSenPos;
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
    EncoderPositionCounter = s/distancePerMark;
}

void ParseTPDO3(unsigned int sid, unsigned char* data)
{
    if((sid&0x780)!=0x380)//if it's not TPDO3
        return;
    char vio0 = ~data[0];
    char vio7 = data[4];
    regim1 = vio0&0x8;
    regim2 = vio0&0x10;
    int f = vio7&0b01000000;
    if(f > 0)
    {
        if((sid&0x3) == 0x1)
            prevV[0] = 1;
        if((sid&0x3) == 0x2)
            prevV[1] = 1;
        if((sid&0x3) == 0x3)
            prevV[2] = 1;
    }
    else if(f == 0)
    {
        if((sid&0x3) == 0x1)
            prevV[0] = 0;
        if((sid&0x3) == 0x2)
            prevV[1] = 0;
        if((sid&0x3) == 0x3)
            prevV[2] = 0;
    }
    AnalyzePrevV();
}

void AnalyzePrevV()
{
    if((prevV[0] == 1 && prevV[1] == 1)||(prevV[0] == 1 && prevV[2] == 1)||(prevV[1] == 1 && prevV[2] == 1))
    {
        T4CONbits.TON = 0;
        T2CONbits.TON = 0;
        T3CONbits.TON = 0;
        currentSpeed = 0;
    }
    else if((prevV[0] == 0 && prevV[1] == 0)||(prevV[0] == 0 && prevV[2] == 0)||(prevV[1] == 0 && prevV[2] == 0))
    {
        if(T3CONbits.TON == 0)
        {
            WriteOutputSignals(3|(1<<4));
            SetStartSignal(2);
            T3CONbits.TON = 1;
        }
    }
}

void SetSpeedByJoystick()
{
    if(EncReadHandModeSignal() == 1)
    {
        unsigned int AdcMaximum = ADC_MAXIMUM;
        unsigned int MaxSpeed = Vmax;
        if(regim1 == 0 && regim2 == 0)
            MaxSpeed = RevisionSpeed;

        float koef = (float)MaxSpeed/AdcMaximum;
        unsigned int AdcNumber = GetAnalogSignal(0);

        int necessaryV = (AdcNumber*koef);
        int def = necessaryV - currentSpeed;
        currentSpeed+= def;

        
       /* unsigned int MaxSpeed = Vmax;
        if(regim1 == 0 && regim2 == 0)
            MaxSpeed = REVISION_SPEED;
        unsigned int koef = MaxSpeed/AdcMaximum;
        unsigned int AdcNumber = GetAnalogSignal(0);
        necessarySpeed = (AdcNumber*koef);*/
    }
}

void SetStartDirection()
{
    if(EncReadHandModeSignal() == 0)
    {
        long s = EncGetS();
        if(s <= _lowEdge + 1000)
            direction = 1;
    }
}


