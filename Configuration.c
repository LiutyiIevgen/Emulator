#include "Configuration.h"

extern long _lowEdge;
extern long _highEdge;
extern int Vstart;
extern int R;
extern int Amax;
extern int Vmax;

extern double distancePerMark;
extern long exactStopZone;
extern long slowdown_zone;
extern int RevisionSpeed;
extern int SpeedUpZone;
extern int RevisionExtraEdge;

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
    ReadParameterValue(R_VALUE,&R);
    ReadParameterValue(DISTANCE_PER_MARK,&distancePerMark);
    ReadParameterValue(EXACT_STOP_ZONE,&exactStopZone);
    ReadParameterValue(SLOWDOWN_ZONE,&slowdown_zone);
    ReadParameterValue(REVISION_SPEED,&RevisionSpeed);
    ReadParameterValue(SPEEDUP_ZONE,&SpeedUpZone);
    ReadParameterValue(REVISION_EXTRA_EDGE,&RevisionExtraEdge);
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
        case DISTANCE_PER_MARK:
            ReadParameterValue(DISTANCE_PER_MARK,&distancePerMark);
            break;
        case EXACT_STOP_ZONE:
            ReadParameterValue(EXACT_STOP_ZONE,&exactStopZone);
            break;
        case SLOWDOWN_ZONE:
            ReadParameterValue(SLOWDOWN_ZONE,&slowdown_zone);
            break;
        case REVISION_SPEED:
            ReadParameterValue(REVISION_SPEED,&RevisionSpeed);
            break;
        case SPEEDUP_ZONE:
            ReadParameterValue(SPEEDUP_ZONE,&SpeedUpZone);
            break;
        case REVISION_EXTRA_EDGE:
            ReadParameterValue(REVISION_EXTRA_EDGE,&RevisionExtraEdge);
            break;
    }
}
