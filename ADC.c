#include "ADC.h"
#include <xc.h>

void InitADC()
{
  TRISB  = 0xFFFF;          //Port B is input
  ADPCFG = 0xFFE0;          //10th channel is sampled and coverted
  ADCON1 = 0;               //ADC off, output_format=INTEGER
                            //Manual start of convesion
                            //Manual start of sampling
  ADCHS  = 0x0000;          //Connect RB0 on AN0 as CH0 input
  ADCSSL = 0;               //No scan
  ADCON3 = 0x1003;          //ADCS=3 (min TAD for 10MHz is 3*TCY=300ns)
  ADCON2 = 0;               //Interrupt upon completion of one sample/convert
}

unsigned char GetAnalogSignal(unsigned char num)
{
  ADCHS  = 0x0000 + num;
  ADCON1bits.ADON = 1;           //ADC on
  int rez = 0;
  int i;

  ADCON1bits.SAMP = 1;        //Start sampling (SAMP=1)
  for(i=0;i<100;i++);        //Wait for 100ms (sampling ...)
  ADCON1bits.SAMP = 0;        //Clear SAMP bit (trigger conversion)
  while (ADCON1bits.DONE == 0);
  rez = ADCBUF0 >> 4;       //Output result on port D
  ADCON1bits.ADON = 0;           //ADC off
  return rez;
}
