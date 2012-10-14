
#include "ext/frameworkext.h"
#ifdef _FRAMEWORK_CORE
#include "os/CServiceManager.h"
#endif

#define __USE_MALLOC

#include <elastos/Map.h>

namespace Elastos
{
    const RBTreeColorType S_RBTreeRed = FALSE;
    const RBTreeColorType S_RBTreeBlack = TRUE;

    ELAPI GetServiceManager(IServiceManager** serviceManager)
    {
        return CServiceManager::AcquireSingleton(serviceManager);
    }
}
