//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __ASTUDENT_H__
#define __ASTUDENT_H__

#include "_AStudent.h"

CarClass(AStudent)
{
public:
    CARAPI Study();

    CARAPI GetID(
        /* [out] */ Int32 * pId);

    CARAPI SetID(
        /* [in] */ Int32 id);

    CARAPI OnAspectAttaching(
        /* [in] */ PInterface pObject);

    CARAPI OnAspectDetaching(
        /* [in] */ PInterface pObject);
    
private:
    Int32 m_studentID;
    IChild* m_pChild;
};

#endif // __ASTUDENT_H__
