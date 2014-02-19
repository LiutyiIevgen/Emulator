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
    PR1 = 40000;//24000000/(speed/distance_per_mark);          // Set Timer 1 period (max value is 65535)
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
    PR2 = 937;//24000000/(speed/distance_per_mark);          // Set Timer 1 period (max value is 65535)
    _T2IP = 5;            // Set Timer 1 interrupt priority
    _T2IF = 0;            // Clear Timer 1 interrupt flag
    _T2IE = 1;            // Enable Timer 1 interrupt
    T2CONbits.TON = 0;    // Turn on Timer 1
}
