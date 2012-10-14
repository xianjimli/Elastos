
#ifndef __CCONTENTPROVIDERHOLDER_H__
#define __CCONTENTPROVIDERHOLDER_H__

#include "_CContentProviderHolder.h"
#include <elastos/AutoPtr.h>
#include "capsule/CContentProviderInfo.h"

CarClass(CContentProviderHolder)
{
public:
    CARAPI GetContentProvider(
        /* [out] */ IContentProvider** provider);

    CARAPI SetContentProvider(
        /* [in] */ IContentProvider* provider);

    CARAPI GetContentProviderInfo(
        /* [out] */ IContentProviderInfo** info);

    CARAPI SetContentProviderInfo(
        /* [in] */ IContentProviderInfo* info);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

public:
    AutoPtr<CContentProviderInfo> mInfo;
    AutoPtr<IContentProvider> mProvider;
};

#endif //__CCONTENTPROVIDERHOLDER_H__
