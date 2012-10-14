
#ifndef __LOCATIONPROVIDER_H__
#define __LOCATIONPROVIDER_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>

class LocationProvider : public ElRefBase, ILocalLocationProvider
{
private:
	static const CString TAG;

public:
    LocationProvider (
    	/* [in] */ String name,
    	/* [in] */ ILocationManager* service);

    CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI GetName(
    	/* [out] */ String* name);

    CARAPI MeetsCriteria(
        /* [in] */ ICriteria* criteria,
        /* [out] */ Boolean* result);

private:
	String mName;
    AutoPtr<ILocationManager> mService;
};

#endif //__LOCATIONPROVIDER_H__
