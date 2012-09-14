//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "CPig.h"

ECode CPig::GetKind(
    /* [out] */ String * pName)
{
    *pName = "pig";
    return NOERROR;
}

ECode CPig::GetOwner(
    /* [out] */ String * pName)
{
    String name;
    Super::GetOwner(&name);
    *pName = "Old ";
    *pName += name;
    return NOERROR;
}

ECode CPig::Says(
    /* [out] */ String * pSound)
{
    *pSound = "oink";
    return NOERROR;
}

ECode CPig::constructor()
{
    Super::constructor();
    return NOERROR;
}

