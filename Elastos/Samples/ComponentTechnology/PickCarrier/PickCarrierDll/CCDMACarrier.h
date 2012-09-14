//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __CCDMACARRIER_H__
#define __CCDMACARRIER_H__


#include "_CCDMACarrier.h"

CarClass(CCDMACarrier)
{
public:
    CARAPI Connect();

    CARAPI Disconnect();

    CARAPI GetName(
        /* [out] */ String * pName);

    CARAPI DoSomethingWithCDMA();

private:
    // TODO: Add your private member variables here.
};

#endif // __CCDMACARRIER_H__
