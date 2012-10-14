
#ifndef __CRESOURCESFACTORY_H__
#define __CRESOURCESFACTORY_H__

#include "_CResourcesFactory.h"

CarClass(CResourcesFactory)
{
public:
    CARAPI GetSystem(
        /* [out] */ IResources** res);

    CARAPI UpdateSystemConfiguration(
        /* [in] */ IConfiguration* config,
        /* [in] */ IDisplayMetrics* metrics);
};

#endif //__CRESOURCESFACTORY_H__
