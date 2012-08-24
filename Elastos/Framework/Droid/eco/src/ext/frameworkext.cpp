
#include "ext/frameworkext.h"
#include "server/CServiceManager.h"

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
