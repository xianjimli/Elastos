#ifndef __TIMEZONEGETTER_H__
#define __TIMEZONEGETTER_H__

#include "elastos.h"
using namespace Elastos;

class TimeZoneGetter {
public:
    static TimeZoneGetter* GetInstance();
    static void SetInstance(
        /* [in] */ TimeZoneGetter* getter);

    String GetId();

private:
    static TimeZoneGetter* mInstance;

};

#endif //__TIMEZONEGETTER_H__