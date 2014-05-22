/* 
 * File:   Encoder.h
 * Author: Æåíÿ
 *
 * Created on 17 ??????? 2014 ?., 10:00
 */

/**
 * @author Liutyi Ievgen
 * @todo make encoder signals wtich simulates a forward movement
 */
void EncForwardDirectionStroke();
/**
 * @author Liutyi Ievgen
 * @todo make encoder signals wtich simulates a reverse movement movement
 */
void EncReverseDirectionStroke();
void InitCounter();
/**
 * @author Liutyi Ievgen
 * @return start switch state
 * @todo read a state of start switch
 */
unsigned char EncReadStartSignal();
/**
 * @author Liutyi Ievgen
 * @return OverZero switch state
 * @todo read a state of OverZero switch
 */
unsigned char EncReadOverZeroSignal();
/**
 * @author Liutyi Ievgen
 * @return HandMode switch state
 * @todo read a state of HandMode switch
 */
unsigned char EncReadHandModeSignal();
void EncReadDirectionSignal();
/**
 * @author Liutyi Ievgen
 * @todo change speed to necessary meaning
 */
void EncSpeedControl();
/**
 * @author Liutyi Ievgen
 * @todo stoped a movement in boundary points
 */
void EncStopControl();
/**
 * @author Liutyi Ievgen
 * @todo define a start of slowdown
 */
void EncSlowdownControl();
/**
 * @author Liutyi Ievgen
 * @todo define a direction and start a movement
 */
void EncStartControl();
/**
 * @author Liutyi Ievgen
 * @return diretction of movement
 * @todo get a diretction of movement
 */
unsigned int EncGetDirection();
unsigned int EncGetStop();
unsigned int EncGetStart();
void TrySetOverRise();
void ParseTPDO1(unsigned int, unsigned char*);
void ParseTPDO3(unsigned int, unsigned char*);
void AnalyzePrevV();
void SetSpeedByJoystick();

