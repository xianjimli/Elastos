
#ifndef __COBJECTINTEGERMAP_H__
#define __COBJECTINTEGERMAP_H__

#include "_CObjectIntegerMap.h"
#include "ext/frameworkext.h"
#include <elastos/HashMap.h>
#include <elastos/AutoPtr.h>

CarClass(CObjectIntegerMap)
{
public:
    CObjectIntegerMap();

    ~CObjectIntegerMap();

    CARAPI Get(
        /* [in] */ Int32 key,
        /* [out] */ IInterface** value);

    CARAPI Put(
        /* [in] */ Int32 key,
        /* [in] */ IInterface* value);

private:
    HashMap<Int32, AutoPtr<IInterface> > mMap;
};

#endif //__COBJECTINTEGERMAP_H__
