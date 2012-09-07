//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __CANIMAL_H__
#define __CANIMAL_H__


#include "_CAnimal.h"

CarClass(CAnimal)
{
public:
    CARAPI GetKind(
        /* [out] */ String * pName);

    CARAPI GetOwner(
        /* [out] */ String * pName);

    CARAPI Says(
        /* [out] */ String * pSound);

private:
    // TODO: Add your private member variables here.
};

#endif // __CANIMAL_H__
