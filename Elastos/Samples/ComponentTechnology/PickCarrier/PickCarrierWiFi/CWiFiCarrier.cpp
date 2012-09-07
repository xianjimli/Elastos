//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "CWiFiCarrier.h"
#include <stdio.h>

ECode CWiFiCarrier::Connect()
{
    printf("WiFi carrier connected!\n");
    return NOERROR;
}

ECode CWiFiCarrier::Disconnect()
{
    printf("WiFi carrier disconnected!\n");
    return NOERROR;
}

ECode CWiFiCarrier::GetName(
    /* [out] */ String * pName)
{
    *pName = "China Telecom";
    return NOERROR;
}

ECode CWiFiCarrier::DoSomethingWithWiFi()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

