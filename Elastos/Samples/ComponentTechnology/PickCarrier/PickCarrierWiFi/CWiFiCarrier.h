//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __CWIFICARRIER_H__
#define __CWIFICARRIER_H__


#include "_CWiFiCarrier.h"

CarClass(CWiFiCarrier)
{
public:
    CARAPI Connect();

    CARAPI Disconnect();

    CARAPI GetName(
        /* [out] */ String * pName);

    CARAPI DoSomethingWithWiFi();

private:
    // TODO: Add your private member variables here.
};

#endif // __CWIFICARRIER_H__
