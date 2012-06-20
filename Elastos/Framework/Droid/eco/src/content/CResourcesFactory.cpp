
#include "content/CResourcesFactory.h"
#include "content/CResources.h"

CARAPI CResourcesFactory::GetSystem(
    /* [out] */ IResources** res)
{
    if (!res) {
        return E_INVALID_ARGUMENT;
    }

    Mutex::Autolock lock(CResources::mSyncLock);

    ECode ec = NOERROR;
    if (CResources::mSystem == NULL) {
        ec = CResources::New(res);
        if (FAILED(ec)) {
            *res = NULL;
            return ec;
        }
        CResources::mSystem = *res;

    }
    *res = CResources::mSystem.Get();
    (*res)->AddRef();
    return NOERROR;
}

CARAPI CResourcesFactory::UpdateSystemConfiguration(
    /* [in] */ IConfiguration* config,
    /* [in] */ IDisplayMetrics* metrics)
{
    if (CResources::mSystem != NULL) {
        CResources::mSystem->UpdateConfiguration(config, metrics);
        //Log.i(TAG, "Updated system resources " + mSystem
        //        + ": " + mSystem.getConfiguration());
    }
    return NOERROR;
}
