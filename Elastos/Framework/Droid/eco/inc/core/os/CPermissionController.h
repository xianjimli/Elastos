
#ifndef __CPERMISSIONCONTROLLER_H__
#define __CPERMISSIONCONTROLLER_H__

#include "_CPermissionController.h"

CarClass(CPermissionController)
{
public:
    CARAPI CheckPermission(
        /* [in] */ const String& permission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [out] */ Boolean * pResult);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CPERMISSIONCONTROLLER_H__
