//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __CCAT_H__
#define __CCAT_H__


#include "_CCat.h"

CarClass(CCat)
{
public:
    CARAPI GetKind(
        /* [out] */ String * pName);

    CARAPI GetOwner(
        /* [out] */ String * pName);

    CARAPI Says(
        /* [out] */ String * pSound);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CCAT_H__
