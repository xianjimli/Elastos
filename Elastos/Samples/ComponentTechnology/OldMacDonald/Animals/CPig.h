//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __CPIG_H__
#define __CPIG_H__


#include "_CPig.h"

CarClass(CPig)
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

#endif // __CPIG_H__
