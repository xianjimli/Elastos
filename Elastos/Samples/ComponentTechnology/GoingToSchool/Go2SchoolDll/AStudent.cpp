//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "AStudent.h"
#include <stdio.h>

ECode AStudent::OnAspectAttaching(
    /* [in] */ PInterface pObject)
{
    printf("-> AStudent::OnAspectAttaching()...\n");

    m_pChild = IChild::Probe(pObject);
    if (m_pChild) return NOERROR;
    else return E_NO_INTERFACE;
}

ECode AStudent::OnAspectDetaching(
    /* [in] */ PInterface pObject)
{
    printf("-> AStudent::OnAspectDetaching()...\n");
    return NOERROR;
}

ECode AStudent::Study()
{
    String name;
    m_pChild->GetName(&name);
    name += "'s studying.";
    printf("%s\n", (const char*)name);
    return NOERROR;
}

ECode AStudent::GetID(
    /* [out] */ Int32 * pId)
{
    *pId = m_studentID;
    return NOERROR;
}

ECode AStudent::SetID(
    /* [in] */ Int32 id)
{
    m_studentID = id;
    return NOERROR;
}

