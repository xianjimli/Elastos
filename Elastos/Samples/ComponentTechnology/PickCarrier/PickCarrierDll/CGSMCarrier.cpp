//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "CGSMCarrier.h"
#include <stdio.h>

ECode CGSMCarrier::Connect()
{
    printf("GSM carrier connected!\n");
    return NOERROR;
}

ECode CGSMCarrier::Disconnect()
{
    printf("GSM carrier disconnected!\n");
    return NOERROR;
}

ECode CGSMCarrier::GetName(
    /* [out] */ String * pName)
{
    *pName = "China Mobile";
    return NOERROR;
}

ECode CGSMCarrier::DoSomethingWithGSM()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

