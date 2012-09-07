//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "CCDMACarrier.h"
#include <stdio.h>

ECode CCDMACarrier::Connect()
{
    printf("CDMA carrier connected!\n");
    return NOERROR;
}

ECode CCDMACarrier::Disconnect()
{
    printf("CDMA carrier disconnected!\n");
    return NOERROR;
}

ECode CCDMACarrier::GetName(
    /* [out] */ String * pName)
{
    *pName = "China Unicom";
    return NOERROR;
}

ECode CCDMACarrier::DoSomethingWithCDMA()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

