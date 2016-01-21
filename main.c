/* 
 * File:   main.c
 * Author: Æåíÿ
 *
 * Created on 14 ??????? 2014 ?., 16:05
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "Timer.h"
#include "Can.h"
#include "CanOpen.h"
#include "Encoder.h"
#include "MainLibrary.h"
#include "Configuration.h"
#include "ADC.h"
#include "Spi.h"
#include "FRAM.h"
#include "WriteParameters.h"
#include "globals.h"


// FOSC
#pragma config FOSFPR = XT_PLL16        // Oscillator (XT w/PLL 16x)
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)

// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF            // Watchdog Timer (Disabled)

// FBORPOR
#pragma config FPWRT = PWRT_64          // POR Timer Value (64ms)
#pragma config BODENV = NONE            // Brown Out Voltage (Reserved)
#pragma config BOREN = PBOR_OFF         // PBOR Enable (Disabled)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)

// FBS
#pragma config BWRP = WR_PROTECT_BOOT_OFF// Boot Segment Program Memory Write Protect (Boot Segment Program Memory may be written)
#pragma config BSS = NO_BOOT_CODE       // Boot Segment Program Flash Memory Code Protection (No Boot Segment)
#pragma config EBS = NO_BOOT_EEPROM     // Boot Segment Data EEPROM Protection (No Boot EEPROM)
#pragma config RBS = NO_BOOT_RAM        // Boot Segment Data RAM Protection (No Boot RAM)

// FSS
#pragma config SWRP = WR_PROT_SEC_OFF   // Secure Segment Program Write Protect (Disabled)
#pragma config SSS = NO_SEC_CODE        // Secure Segment Program Flash Memory Code Protection (No Secure Segment)
#pragma config ESS = NO_SEC_EEPROM      // Secure Segment Data EEPROM Protection (No Segment Data EEPROM)
#pragma config RSS = NO_SEC_RAM         // Secure Segment Data RAM Protection (No Secure RAM)

// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = GSS_OFF            // General Segment Code Protection (Disabled)

// FICD
#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)


// Function prototype for timer 1 ISR
void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void);
// Function prototype for timer 2 ISR
void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void);
// Function prototype for timer 3 ISR
void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt(void);

void __attribute__((__interrupt__, __auto_psv__)) _T5Interrupt(void);
// Can1 Receive Parameter
void __attribute__ ((__interrupt__, __auto_psv__)) _C1Interrupt (void);

char startSignal = -1;
char fReadS = 0;
char interCount = 0;

/*int _nodeId = 5;
long _lowEdge; //mm
long _highEdge; //mm
int Vmax; //mm/sec
int Vstart;
int Amax;  //mm/sec^2
int R; //mm/sec^3

long halfS;
long halfV;
int halfVstart;
*/
char was_sdo = 0;
int main(int argc, char** argv) {
    ADPCFG = 0xFFFF; //RA only digit
    InitADC();
    FramInitialization();
    //WriteAllParameters();
    ReadConfig();

    TRISDbits.TRISD8 = 0;//set output on RD8
    TRISDbits.TRISD9 = 0;//set output on RD9
    LATDbits.LATD8 = 0; // LE1
    LATDbits.LATD9 = 0; // LE2

    Delay(3000000);
    TRISCbits.TRISC13 = 0;//set output on RC13
    LATCbits.LATC13 = 1;

    halfS = (labs(_lowEdge)+labs(_highEdge))/2 - labs(_highEdge);
    halfV = Vmax/2;
    halfVstart = Vstart/2;
    //int i = 0;
    //for(i; i < 10000; i++)
    //{
        //ForwardDirectionStroke();
        //Delay(2000);
        //LATCbits.LATC13 = 1 - LATCbits.LATC13; // RC13 value (LED VD1 => ON)
    //}
    
    Can1Initialization();
    InitCounter();
    WriteOutputSignals(1);
  //  float distancePerMark = DISTANCE_PER_MARK;

   /* StartTimer1();
    StartTimer2();
    StartTimer3(); */
    while(1);
    return (EXIT_SUCCESS);
}

// Timer 1 interrupt service write data to uart
void __attribute__((__interrupt__, __auto_psv__)) _T5Interrupt(void)
{
    _T5IF = 0;
    if(!EncGetDirection())
        EncForwardDirectionStroke();
    else
        EncReverseDirectionStroke();
}

void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void)
{
    _T2IF = 0;
    SetLoadSignalState();
    EncReadDirectionSignal();
    EncStartControl();
    
    EncSpeedControl();
    ExactStopSensors();
    
    EncStopControl();
   // EncSlowdownControl();
    SetSpeedByJoystick();
}

void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt(void)
{
    _T3IF = 0;
    TrySetOverRise();
    char signal;
    /*char signal1 = EncIsDirectionChosen();
    char signal2 = EncIsDirectionChosen();
    signal = signal1==1||signal2==1 ? 1:0; */
    /*if(EncGetStart() == 1)
        startSignal = 2; */
    signal = 1;
    if(startSignal==signal)
            return;
        startSignal = signal;
        if(startSignal == 1)
        {
            T4CONbits.TON = 0;
            T2CONbits.TON = 1;
        }
        else if(startSignal == 0)
        {
            T4CONbits.TON = 0;
            T2CONbits.TON = 0;
        }
}

void __attribute__ ((__interrupt__, __auto_psv__)) _C1Interrupt (void){
    IFS1bits.C1IF = 0; //Clear CAN1 interrupt flag
   // C1INTFbits.RX0IF = 0; //Clear CAN1 RX interrupt flag
   // C1INTFbits.RX1IF = 0; //Clear CAN1 RX interrupt flag
    char rxData[8];
    unsigned int sId;
    
    if(C1CTRLbits.ICODE == 7) //check filters
    {
        C1INTFbits.WAKIF = 0;
        return;
    }
    
    if(C1INTFbits.RX0IF == 1)
    {
        sId = C1RX0SIDbits.SID;
        C1INTFbits.RX0IF = 0;
        CAN1ReceiveMessage(rxData,8,0);
    } else if (C1INTFbits.RX1IF == 1)
    {
        sId = C1RX1SIDbits.SID;
        C1INTFbits.RX1IF = 0;
        CAN1ReceiveMessage(rxData,8,1);
    }
   
    if(sId == 0x607)
    {
        was_sdo = 1; 
        //Can1ReceiveData(rxData);
    }
    if(fReadS == 0 && interCount == 10)
    {
        ExactStopSensors();
        SetStartDirection();
        StartTimer4();
        StartTimer2();
        StartTimer3();
        fReadS = 1;
    }
    if(interCount < 10)
    {
        ParseTPDO1(sId, rxData);//parse TPDO message
        interCount++;
    }
    ParseTPDO3(sId, rxData);
    CanOpenParseRSDO(sId,rxData,1); //parse message and send response
    
    C1RX0CONbits.RXFUL = 0;
    C1RX1CONbits.RXFUL = 0;
  }

void SetStartSignal(char value)
{
    startSignal = value;
}