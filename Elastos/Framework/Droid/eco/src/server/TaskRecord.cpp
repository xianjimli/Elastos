
#include "server/TaskRecord.h"

TaskRecord::TaskRecord(
    /* [in] */ Int32 taskId,
    /* [in] */ IActivityInfo* info,
    /* [in] */ IIntent* intent,
    /* [in] */ Boolean clearOnBackground)
{
    mTaskId = taskId;
    info->GetTaskAffinity(&mAffinity);
    mClearOnBackground = clearOnBackground;
    SetIntent(intent, info);
}

void TaskRecord::TouchActiveTime()
{
    //lastActiveTime = android.os.SystemClock.elapsedRealtime();
}

Millisecond64 TaskRecord::GetInactiveDuration()
{
//    return android.os.SystemClock.elapsedRealtime() - lastActiveTime;
    return 0;
}

void TaskRecord::SetIntent(
    /* [in] */ IIntent* intent,
    /* [in] */ IActivityInfo* info)
{
    mStringName = NULL;

    String target;
    info->GetTargetActivity(&target);
    if (target.IsNull()) {
        mIntent = intent;
        if (intent != NULL) {
            intent->GetComponent((IComponentName**)&mRealActivity);
        }
        mOrigActivity = NULL;
    }
    else {
        String cname;
        info->GetCapsuleName(&cname);
        AutoPtr<IComponentName> targetComponent;
        CComponentName::New(cname, target,
                (IComponentName**)&targetComponent);
        if (intent != NULL) {
            AutoPtr<IIntent> targetIntent;
            CIntent::New(intent, (IIntent**)&targetIntent);
            targetIntent->SetComponent(targetComponent);
            mIntent = targetIntent;
            mRealActivity = targetComponent;
            intent->GetComponent((IComponentName**)&mOrigActivity);
        }
        else {
            mIntent = NULL;
            mRealActivity = targetComponent;
            String name;
            info->GetName(&name);
            CComponentName::New(cname, name, (IComponentName**)&mOrigActivity);
        }
    }

    Int32 flags;
    if (mIntent != NULL && (mIntent->GetFlags(&flags),
            flags & Intent_FLAG_ACTIVITY_RESET_TASK_IF_NEEDED) != 0) {
        // Once we are set to an Intent with this flag, we count this
        // task as having a true root activity.
        mRootWasReset = TRUE;
    }
}

ECode TaskRecord::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}
