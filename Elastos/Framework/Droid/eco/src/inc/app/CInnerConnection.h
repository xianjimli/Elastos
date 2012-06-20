
#ifndef __CINNERCONNECTION_H__
#define __CINNERCONNECTION_H__

#include "_CInnerConnection.h"
#include "app/LoadedCap.h"

CarClass(CInnerConnection)
{
public:
    CARAPI Init(
        /* [in] */ LoadedCap::ServiceDispatcher* sd);

    CARAPI Connected(
        /* [in] */ IComponentName* name,
        /* [in] */ IBinder* service);

    CARAPI GetDescription(
        /* [out] */ String* description);

public:
    LoadedCap::ServiceDispatcher* mDispatcher;
};

#endif //__CINNERCONNECTION_H__
