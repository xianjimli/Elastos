
#include "ext/frameworkdef.h"
#include "view/accessibility/CAccessibilityManagerHelper.h"
#include "view/accessibility/CAccessibilityManager.h"
#include <elastos/Mutex.h>

ECode CAccessibilityManagerHelper::GetInstance(
    /* [in] */ IContext* context,
    /* [out] */ ILocalAccessibilityManager** localAccessibilityManager)
{
    VALIDATE_NOT_NULL(localAccessibilityManager);

    Mutex::Autolock lock(CAccessibilityManager::sInstanceSync);
    if (CAccessibilityManager::sInstance == NULL) {
        CAccessibilityManager::New(context,
        (ILocalAccessibilityManager**)&CAccessibilityManager::sInstance);
    }
    *localAccessibilityManager = CAccessibilityManager::sInstance;
    return NOERROR;
}
