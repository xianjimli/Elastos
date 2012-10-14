
#include "frameworkext.h"

namespace Elastos
{
    ELAPI GetServiceManager(IServiceManager** serviceManager)
    {
        return CServiceManager::AcquireSingleton(serviceManager);
    }
}
