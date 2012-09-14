//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "GCarrier.h"

#include "CGSMCarrier.h"
#include "CCDMACarrier.h"

#include <Elastos.IO.h>

#include <stdlib.h>
#include <time.h>

ECode GCarrier::New(
    /*[out]*/ ICarrier ** ppICarrier)
{
    ECode ec;
	IFileInputStream *is;
	Int32 value;

    // pretend to find a GSM, CDMA or WiFi network
    //

    ec = CFileInputStream::New(String("config.dat"), &is);
    if (FAILED(ec)) return ec;

    ec = is->Read(&value);
	if (FAILED(ec)) {
		is->Release();
		return ec;
	}

    if (value == '1') {
        ec = CGSMCarrier::New(ppICarrier);
    }
    else if (value == '2'){
        ec = CCDMACarrier::New(ppICarrier);
    }
	else if (value == '3'){
        ec = GCarrier::NewViaDynamicBinding(String("PickCarrierWiFi.eco"), ppICarrier);
	}
	else {
	    ec = E_NOT_IMPLEMENTED;
	}

	is->Close();
	is->Release();

    return ec;
}

