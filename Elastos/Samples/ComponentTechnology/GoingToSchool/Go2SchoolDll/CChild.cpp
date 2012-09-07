//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "CChild.h"
#include <stdio.h>

ECode CChild::OnAspectAttaching(
    /* [in] */ PInterface pAspect)
{
    printf("-> CChild::OnAspectAttaching()...\n");
    return NOERROR;
}

ECode CChild::OnAspectDetaching(
    /* [in] */ PInterface pAspect)
{
    printf("-> CChild::OnAspectDetaching()...\n");
    return NOERROR;
}

ECode CChild::Play()
{
    String name;
    this->GetName(&name);
    name += "'s playing.";
    printf("%s\n", (const char*)name);
    return NOERROR;
}

ECode CChild::GetName(
    /* [out] */ String * pName)
{
    if (pName == NULL) {
        return E_INVALID_ARGUMENT;
    }
    *pName = m_childName;
    return NOERROR;
}

ECode CChild::constructor(
    /* [in] */ const String& name)
{
    m_childName = name;
    return NOERROR;
}

