
#ifndef __ACTIVITYMANAGERNATIVE_H__
#define __ACTIVITYMANAGERNATIVE_H__

#include "ext/frameworkext.h"

class ActivityManagerNative
{
public:
    static CARAPI GetDefault(
        /* [out] */ IActivityManager** service);    
};

#endif //__ACTIVITYMANAGERNATIVE_H__
