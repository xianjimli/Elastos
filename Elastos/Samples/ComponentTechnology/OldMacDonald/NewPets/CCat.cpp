//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "CCat.h"

ECode CCat::GetKind(
    /* [out] */ String * pName)
{
    *pName = "cat";
    return NOERROR;
}

ECode CCat::GetOwner(
    /* [out] */ String * pName)
{
    String name;
    Super::GetOwner(&name);
    *pName = "Old ";
    *pName += name;
    return NOERROR;
}

ECode CCat::Says(
    /* [out] */ String * pSound)
{
    *pSound = "meow";
    return NOERROR;
}

ECode CCat::constructor()
{
    Super::constructor();
    return NOERROR;
}

