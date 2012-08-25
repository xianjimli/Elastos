
#ifndef __CSERVICERESTARTER_H__
#define __CSERVICERESTARTER_H__

#include "_CServiceRestarter.h"
#include "server/CServiceRecord.h"

CarClass(CServiceRestarter)
{
public:
    CARAPI Run();

public:
    CARAPI_(void) SetService(
        /* [in] */ CServiceRecord* service);
};

#endif //__CSERVICERESTARTER_H__
