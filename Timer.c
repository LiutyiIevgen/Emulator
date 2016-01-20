#include <xc.h>
#include "Timer.h"
#include "Configuration.h"

void StartTimer1()
{
    // Configure Timer 1.
    // PR1 and TCKPS are set to call interrupt every 500ms.
    // Period = PR1 * prescaler * Tcy = 58594 * 256 * 33.33ns = 500ms
    T1CON = 0;            // Clear Timer 1 configuration
    T1CONbits.TCKPS = 0;  // Set timer 1 prescaler (0=1:1, 1=1:8, 2=1:64, 3=1:256)
    PR1 = 60000;//24000000/(speed/distance_per_mark);          // Set Timer 1 period (max value is 65535)
    _T1IP = 6;            // Set Timer 1 interrupt priority
    _T1IF = 0;            // Clear Timer 1 interrupt flag
    _T1IE = 1;            // Enable Timer 1 interrupt
    T1CONbits.TON = 0;    // Turn on Timer 1
}

void StartTimer2()// 0.01 sec
{
    // Configure Timer 2.
    // PR1 and TCKPS are set to call interrupt every 500ms.
    // Period = PR1 * prescaler * Tcy = 58594 * 256 * 33.33ns = 500ms
    T2CON = 0;            // Clear Timer 1 configuration
    T2CONbits.TCKPS = 3;  // Set timer 1 prescaler (0=1:1, 1=1:8, 2=1:64, 3=1:256)
    PR2 = 9390;//11764;//24000000/(speed/distance_per_mark);          // Set Timer 1 period (max value is 65535)
    _T2IP = 5;            // Set Timer 1 interrupt priority
    _T2IF = 0;            // Clear Timer 1 interrupt flag
    _T2IE = 1;            // Enable Timer 1 interrupt
    T2CONbits.TON = 0;    // Turn on Timer 2
}

void StartTimer3()
{
    // Configure Timer 2.
    // PR1 and TCKPS are set to call interrupt every 500ms.
    // Period = PR1 * prescaler * Tcy = 58594 * 256 * 33.33ns = 500ms
    T3CON = 0;            // Clear Timer 1 configuration
    T3CONbits.TCKPS = 3;  // Set timer 1 prescaler (0=1:1, 1=1:8, 2=1:64, 3=1:256)
    PR3 = 10000;//24000000/(speed/distance_per_mark);          // Set Timer 1 period (max value is 65535)
    _T3IP = 4;            // Set Timer 1 interrupt priority
    _T3IF = 0;            // Clear Timer 1 interrupt flag
    _T3IE = 1;            // Enable Timer 1 interrupt
    T3CONbits.TON = 1;    // Turn on Timer 1
}

void StartTimer4()
{
    T5CON = 0;
    T4CON = 0;
    T4CONbits.T32 = 1;
    T4CONbits.TCKPS=0;
    _T5IP = 6;
    _T5IF = 0;
    _T5IE = 1;
    T4CONbits.TON = 0;
}