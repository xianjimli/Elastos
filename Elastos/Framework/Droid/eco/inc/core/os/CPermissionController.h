
#ifndef __CPERMISSIONCONTROLLER_H__
#define __CPERMISSIONCONTROLLER_H__

#include "_CPermissionController.h"
#include <elastos/AutoPtr.h>


CarClass(CPermissionController)
{
public:
    CARAPI CheckPermission(
        /* [in] */ CString permission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [out] */ Boolean* result);

    CARAPI constructor(
        /* [in] */ IActivityManager* activityManagerService);

private:
    // TODO: Add your private member variables here.
    IActivityManager* mActivityManagerService;
};

#endif // __CPERMISSIONCONTROLLER_H__
