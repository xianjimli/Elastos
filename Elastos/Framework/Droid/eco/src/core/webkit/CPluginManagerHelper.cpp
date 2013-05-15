
#include "webkit/CPluginManagerHelper.h"
#include "webkit/CPluginManager.h"
ECode CPluginManagerHelper::GetInstance(
    /* [in] */ IContext * context,
    /* [out] */ IPluginManager ** instance)
{
	*instance = CPluginManager::GetInstance(context);
    return NOERROR;
}

