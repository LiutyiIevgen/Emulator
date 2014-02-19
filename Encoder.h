/* 
 * File:   Encoder.h
 * Author: Æåíÿ
 *
 * Created on 17 ??????? 2014 ?., 10:00
 */

void EncForwardDirectionStroke();
void EncReverseDirectionStroke();
unsigned char EncReadStartSignal();
void EncSpeedControl();
void EncStopControl();
void EncSlowdownControl();
void EncStartControl();
unsigned int EncGetDirection();

