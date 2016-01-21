/* 
 * File:   Configuration.h
 * Author: Æåíÿ
 *
 * Created on 17 ??????? 2014 ?., 12:23
 */

//#define LOW_SEN_POS -250000; //mm !!!!!!!Checkout if we need this
//#define HIGH_SEN_POS 50000; //mm

#define HIGH_EDGE 0x2001
#define LOW_EDGE 0x2002
#define V_MAX   0x2003
#define V_START 0x2004
#define A_MAX   0x2005
#define R_VALUE 0x2006
#define DISTANCE_PER_MARK 0x2007//0.5; // mm/imp
#define EXACT_STOP_ZONE 0x2008//100; //mm
#define SLOWDOWN_ZONE 0x2009//75000; //mm
#define REVISION_SPEED 0x200A//2000;// mm/sec
#define SPEEDUP_ZONE 0x200B  //5000 //mm
#define REVISION_EXTRA_EDGE 0x200C//10000

/*extern int halfVstart;
extern long halfS;
extern long halfV;
*/
void ReadConfig();

void ReadCodtDomainValue( int parameterId, char* overSpeedIS, char* overSpeedIV);

void UpdateConfig(int parameterId);