//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __CGSMCARRIER_H__
#define __CGSMCARRIER_H__


#include "_CGSMCarrier.h"

CarClass(CGSMCarrier)
{
public:
    CARAPI Connect();

    CARAPI Disconnect();

    CARAPI GetName(
        /* [out] */ String * pName);

    CARAPI DoSomethingWithGSM();

private:
    // TODO: Add your private member variables here.
};

#endif // __CGSMCARRIER_H__
