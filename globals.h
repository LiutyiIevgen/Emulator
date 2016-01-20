/* 
 * File:   globals.h
 * Author: Æåíÿ
 *
 * Created on 26 ??????? 2014 ?., 11:51
 */
#ifndef GLOBALS_H
#define	GLOBALS_H

#include "Parameters.h"


/*long _lowEdge = -250000; //mm
long _highEdge =  50000; //mm
int Vmax = 10000; //mm/sec
int Vstart = 900;
int Amax = 100;  //mm/sec^2
int R = 5; //mm/sec^3
*/
int _nodeId = 5;
long _lowEdge; //mm
long _highEdge; //mm
int Vmax; //mm/sec
int Vstart;
int Amax;  //mm/sec^2
int R; //mm/sec^3

long halfS;
long halfV;
int halfVstart;

/*
int _nodeId; int buffer1;
float _distancePerMarks; //mm
long _highEdge;
long _lowEdge;
int _synchronization; int buffer2;
int _synchZone; int buffer3;
int _overRiseControl = 1; int buffer4;
int _kolibrovka = 0; int buffer5;
int _overRizeZoneHigh; int buffer6;
int _overRizeZoneLow; int buffer7;

int _overRizeZonePeopleLow;
int _overRizeZonePeopleHigh;
int _overRizeZoneNegabaritLow;
int _overRizeZoneNegabaritHigh;
long _StartSlowdownHigh;
long _StartSlowdownLow;

int _buffer; int buffer8;

long overSpeedIS1[120]; int overSpeedIV1[120];
long overSpeedIS2[120]; int overSpeedIV2[120];
long overSpeedIS3[120]; int overSpeedIV3[120];
long overSpeedIS4[120]; int overSpeedIV4[120];
extern long EncPositionCounter;
int _gisterezisNull; int buffer9;
float _vCoefficient;
float _aCoefficient;
int _buffer2;
long _horizont1;
long _horizont2;
long _horizont3;
int _maxVhorizont;
long _zeroPlatform;*/

//parameters ids
#define HIGH_EDGE 0x2001
#define LOW_EDGE 0x2002
#define V_MAX   0x2003
#define V_START 0x2004
#define A_MAX   0x2005
#define R_VALUE 0x2006

void ReadConfig()
{
    ReadParameterValue(HIGH_EDGE,&_highEdge);
    _highEdge = _highEdge<<8;
    _highEdge /= 256;
    ReadParameterValue(LOW_EDGE,&_lowEdge);
    _lowEdge = _lowEdge<<8;
    _lowEdge /= 256;
    ReadParameterValue(V_MAX,&Vmax);
    ReadParameterValue(V_START,&Vstart);
    ReadParameterValue(A_MAX,&Amax);
    ReadParameterValue(V_MAX,&Vmax);
    ReadParameterValue(R_VALUE,&R);
   /* ReadParameterValue(SYNCHRONIZATION,&_synchronization);
    ReadParameterValue(SYNCH_ZONE,&_synchZone);
    ReadParameterValue(OVER_RIZE_ZONE_HIGH,&_overRizeZoneHigh);
    ReadParameterValue(OVER_RIZE_ZONE_LOW,&_overRizeZoneLow);

    ReadParameterValue(OVER_RIZE_ZONE_PEOPLE_LOW,&_overRizeZonePeopleLow);
    ReadParameterValue(OVER_RIZE_ZONE_PEOPLE_HIGH,&_overRizeZonePeopleHigh);
    ReadParameterValue(OVER_RIZE_ZONE_NEGABARIT_LOW,&_overRizeZoneNegabaritLow);
    ReadParameterValue(OVER_RIZE_ZONE_NEGABARIT_HIGH,&_overRizeZoneNegabaritHigh);
    ReadParameterValue(START_SLOWDOWN_HIGH,&_StartSlowdownHigh);
    ReadParameterValue(START_SLOWDOWN_LOW,&_StartSlowdownLow);

    ReadCodtDomainValue(DEFENCE_DIAGRAM1,overSpeedIS1,overSpeedIV1);
    ReadCodtDomainValue(DEFENCE_DIAGRAM2,overSpeedIS2,overSpeedIV2);
    ReadCodtDomainValue(DEFENCE_DIAGRAM3,overSpeedIS3,overSpeedIV3);
    ReadCodtDomainValue(DEFENCE_DIAGRAM4,overSpeedIS4,overSpeedIV4);
    ReadParameterValue(GISTEREZIS_NULL,&_gisterezisNull);
    ReadParameterValue(V_COEFFICIENT,&_vCoefficient);
    ReadParameterValue(A_COEFFICIENT,&_aCoefficient);
    ReadParameterValue(HORIZONT_1,&_horizont1);
    _horizont1 = _horizont1<<8;
    _horizont1 /= 256;
    ReadParameterValue(HORIZONT_2,&_horizont2);
    _horizont2 = _horizont2<<8;
    _horizont2 /= 256;
    ReadParameterValue(HORIZONT_3,&_horizont3);
    _horizont3 = _horizont3<<8;
    _horizont3 /= 256;
    ReadParameterValue(MAX_V_HORIZONT,&_maxVhorizont);
    ReadParameterValue(ZERO_PLATFORM,&_zeroPlatform);*/
}
void ReadCodtDomainValue( int parameterId, char* overSpeedIS, char* overSpeedIV)
{
    char codtDomain[120];
    long overSpeedS[20];
    int overSpeedV[20];
    ReadParameterValue(parameterId,codtDomain);
    int i=0;
    long* s;
    int* v;
    for(i;i<20;i++)
    {
        s = (codtDomain + i*6); v = (codtDomain + i*6 + 4);
        overSpeedS[i] = *s; overSpeedV[i] = *v;
    }
    //OverSpeedInterpolation(overSpeedS,overSpeedV,overSpeedIS,overSpeedIV);
}


void UpdateConfig(int parameterId)
{
    switch(parameterId)
    {
        case HIGH_EDGE:
            ReadParameterValue(HIGH_EDGE,&_highEdge);
            _highEdge = _highEdge<<8;
            _highEdge /= 256;
            break;
        case LOW_EDGE:
            ReadParameterValue(LOW_EDGE,&_lowEdge);
            _lowEdge = _lowEdge<<8;
            _lowEdge /= 256;
            break;
        case V_MAX:
            ReadParameterValue(V_MAX,&Vmax);
            break;
        case V_START:
            ReadParameterValue(V_START,&Vstart);
            break;
        case A_MAX:
            ReadParameterValue(A_MAX,&Amax);
            break;
        case R_VALUE:
            ReadParameterValue(R_VALUE,&R);
            break;
    }
}
#endif	/* GLOBALS_H */

