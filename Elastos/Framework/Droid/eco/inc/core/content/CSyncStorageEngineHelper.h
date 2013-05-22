
#ifndef __CSYNCSTORAGEENGINEHELPER_H__
#define __CSYNCSTORAGEENGINEHELPER_H__

#include "_CSyncStorageEngineHelper.h"
#include <elastos/AutoPtr.h>

CarClass(CSyncStorageEngineHelper)
{
public:
    CARAPI NewTestInstance(
        /* [in] */ IContext* context,
        /* [out] */ ISyncStorageEngine** newEngine);

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI GetSingleton(
        /* [out] */ ISyncStorageEngine** syncStoregeEngine);

    CARAPI Equals(
        /* [in] */ IBundle* bundleOne,
        /* [in] */ IBundle* bundleTwo,
        /* [out] */ Boolean* isEqual);

private:
    // TODO: Add your private member variables here.
};

#endif // __CSYNCSTORAGEENGINEHELPER_H__
