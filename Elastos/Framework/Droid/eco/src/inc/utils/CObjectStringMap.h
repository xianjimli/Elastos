
#ifndef __COBJECTSTRINGMAP_H__
#define __COBJECTSTRINGMAP_H__

#include "ext/frameworkext.h"
#include <elastos/HashMap.h>
#include <elastos/AutoPtr.h>
#include "_CObjectStringMap.h"

using namespace Elastos;

CarClass(CObjectStringMap)
{
public:
    CObjectStringMap();

    ~CObjectStringMap();

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

#endif //__COBJECTSTRINGMAP_H__
