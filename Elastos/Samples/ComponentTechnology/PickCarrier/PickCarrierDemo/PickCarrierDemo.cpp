//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <PickCarrier.h>
#include <Elastos.IO.h>
#include <stdio.h>

using namespace Elastos;

ECode ChangeConfig(Byte value)
{
	IFileOutputStream *os = NULL;
	ECode ec;

	ec = CFileOutputStream::New(String("/data/data/com.elastos.runtime/elastos/config.dat"), false, &os);
	if (FAILED(ec)) return ec;

	ec = os->Write(value);
	if (FAILED(ec)) {
		os->Release();
		return ec;
	}

	os->Close();
	os->Release();

	return NOERROR;
}

int main (int argc, char *argv[])
{
    ICarrier* pCarrier = NULL;
	String name;
	ECode ec = NOERROR;

	ec = ChangeConfig('1');
	if (FAILED(ec)) goto Exit;

	ec = GCarrier::New(&pCarrier);
    if (FAILED(ec)) {
        printf("Sorry, no GSM carrier available!, ec: 0x%08x\n", ec);
        return ec;
    }

    pCarrier->GetName(&name);
    printf("%s\n", (const char*)name);

    pCarrier->Connect();

    // do something...
    ;;;;

    pCarrier->Disconnect();
    pCarrier->Release();
    printf("\n");

	ec = ChangeConfig('2');
	if (FAILED(ec)) goto Exit;

	ec = GCarrier::New(&pCarrier);
    if (FAILED(ec)) {
        printf("Sorry, no CDMA carrier available!, ec: 0x%08x\n", ec);
        return ec;
    }

    pCarrier->GetName(&name);
    printf("%s\n", (const char*)name);

    pCarrier->Connect();

    // do something...
    ;;;;

    pCarrier->Disconnect();
    pCarrier->Release();
    printf("\n");

	ec = ChangeConfig('3');
	if (FAILED(ec)) goto Exit;

	ec = GCarrier::New(&pCarrier);
    if (FAILED(ec)) {
        printf("Sorry, no WiFi carrier available!, ec: 0x%08x\n", ec);
        return ec;
    }

    pCarrier->GetName(&name);
    printf("%s\n", (const char*)name);

    pCarrier->Connect();

    // do something...
    ;;;;

    pCarrier->Disconnect();
    pCarrier->Release();

	ec = NOERROR;

Exit:
    return ec;
}
