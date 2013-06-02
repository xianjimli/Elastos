
#include "ext/frameworkdef.h"
#include "os/CPermissionController.h"

ECode CPermissionController::CheckPermission(
    /* [in] */ CString permission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [out] */ Boolean* result)
{
    Int32 checkResult;
    ECode ec = mActivityManagerService->CheckPermission(permission, pid, uid, &checkResult);

    if (checkResult == CapsuleManager_PERMISSION_GRANTED) {
        *result = TRUE;
    }
    else {
        *result = FALSE;
    }

    return ec;
}

ECode CPermissionController::constructor(
    /* [in] */ IActivityManager* activityManagerService)
{
    VALIDATE_NOT_NULL(activityManagerService);

    mActivityManagerService = activityManagerService;
    return NOERROR;
}

