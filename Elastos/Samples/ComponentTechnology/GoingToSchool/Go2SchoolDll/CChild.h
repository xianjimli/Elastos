//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __CCHILD_H__
#define __CCHILD_H__

#include "_CChild.h"

CarClass(CChild)
{
public:
    CARAPI Play();

    CARAPI GetName(
        /* [out] */ String * pName);

    CARAPI constructor(
        /* [in] */ const String& name);

    CARAPI OnAspectAttaching(
        /* [in] */ PInterface pAspect);

    CARAPI OnAspectDetaching(
        /* [in] */ PInterface pAspect);

private:
    String m_childName;
};

#endif // __CCHILD_H__
