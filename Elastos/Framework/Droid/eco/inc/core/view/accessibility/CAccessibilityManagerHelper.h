
#ifndef __CACCESSIBILITYMANAGERHELPER_H__
#define __CACCESSIBILITYMANAGERHELPER_H__

#include "_CAccessibilityManagerHelper.h"

CarClass(CAccessibilityManagerHelper)
{
public:
    GetInstance(
        /* [in] */ IContext* context,
        /* [out] */ ILocalAccessibilityManager** localAccessibilityManager);
};

#endif //__CACCESSIBILITYMANAGERHELPER_H__
