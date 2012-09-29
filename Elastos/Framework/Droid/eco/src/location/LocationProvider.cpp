
#include "location/LocationProvider.h"

const CString LocationProvider::TAG = "LocationProvider";

LocationProvider::LocationProvider(
    /* [in] */ String name,
    /* [in] */ ILocationManager* service)
{
	assert(!name.Contains(String(LocationProvider_BAD_CHARS_REGEX)));
//    if (name.matches(BAD_CHARS_REGEX)) {
//        throw new IllegalArgumentException("name " + name +
//            " contains an illegal character");
//    }

    mName = name;
    mService = service;
}

PInterface LocationProvider::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_ILocationProvider) {
        return (ILocationProvider*)this;
    }

    return NULL;
}

UInt32 LocationProvider::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 LocationProvider::Release()
{
    return ElRefBase::Release();
}

ECode LocationProvider::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode LocationProvider::GetName(
	/* [out] */ String* name)
{
	*name = mName;

	return NOERROR;
}

ECode LocationProvider::MeetsCriteria(
    /* [in] */ ICriteria* criteria,
    /* [out] */ Boolean* result)
{
//	try {
    ECode ec = mService->ProviderMeetsCriteria(mName, criteria, result);
    if (ec == E_REMOTE_EXCEPTION) {
    	*result = FALSE;
    }

    return ec;
//    } catch (RemoteException e) {
//        Log.e(TAG, "meetsCriteria: RemoteException", e);
//        return false;
//    }
}
