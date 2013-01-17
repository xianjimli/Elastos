
#ifndef __CPARCELABLEOBJECTSTRINGMAP_H__
#define __CPARCELABLEOBJECTSTRINGMAP_H__

#include "ext/frameworkext.h"
#include "_CParcelableObjectStringMap.h"
#include <elastos/HashMap.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

CarClass(CParcelableObjectStringMap)
{
public:
    CParcelableObjectStringMap();

    ~CParcelableObjectStringMap();

    CARAPI Get(
        /* [in] */ const String& key,
        /* [out] */ IInterface** value);

    CARAPI Put(
        /* [in] */ const String& key,
        /* [in] */ IInterface* value);

    CARAPI GetSize(
        /* [out] */ Int32* size);

    CARAPI GetAllItems(
        /* [out] */ ArrayOf<String>** keys,
        /* [out] */ IObjectContainer** values);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

private:
    HashMap<String, AutoPtr<IInterface> > mMap;
};

#endif //__CPARCELABLEOBJECTSTRINGMAP_H__
