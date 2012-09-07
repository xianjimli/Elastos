//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __RSCHOOL_H__
#define __RSCHOOL_H__

#include "_RSchool.h"

CarClass(RSchool)
{
public:
    CARAPI Open();

    CARAPI OnObjectEntering(
        /* [in] */ PInterface pObject);

    CARAPI OnObjectEntered(
        /* [in] */ PInterface pObject);

    CARAPI OnObjectLeaving(
        /* [in] */ PInterface pObject);

    CARAPI OnObjectLeft(
        /* [in] */ PInterface pObject);
};

#endif // __RSCHOOL_H__
