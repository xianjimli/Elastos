
#ifndef __CACCESSIBILITYMANAGERCLIENT_H__
#define __CACCESSIBILITYMANAGERCLIENT_H__

#include "_CAccessibilityManagerClient.h"
#include "view/accessibility/CAccessibilityManager.h"
#include <elastos/AutoPtr.h>

class CAccessibilityManager;

CarClass(CAccessibilityManagerClient)
{
public:
    CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    CARAPI constructor(
        /* [in] */ ILocalAccessibilityManager* host);

private:
    AutoPtr<CAccessibilityManager> mHost;
};

#endif //__CACCESSIBILITYMANAGERCLIENT_H__
