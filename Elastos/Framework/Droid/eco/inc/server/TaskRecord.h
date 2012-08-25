
#ifndef __TASKRECORD_H__
#define __TASKRECORD_H__

#include "content/CIntent.h"
#include "capsule/CActivityInfo.h"
#include "content/CComponentName.h"
#include <elastos/AutoPtr.h>

class TaskRecord
{
public:
    TaskRecord(
        /* [in] */ Int32 taskId,
        /* [in] */ CActivityInfo* info,
        /* [in] */ IIntent* intent,
        /* [in] */ Boolean clearOnBackground);

    CARAPI_(void) TouchActiveTime();

    CARAPI_(Millisecond64) GetInactiveDuration();

    CARAPI_(void) SetIntent(
        /* [in] */ IIntent* intent,
        /* [in] */ IActivityInfo* info);

    CARAPI GetDescription(
        /* [out] */ String* description);

public:
    Int32 mTaskId;       // Unique identifier for this task.
    String mAffinity;  // The affinity name for this task, or null.
    Boolean mClearOnBackground; // As per the original activity.
    AutoPtr<IIntent> mIntent;          // The original intent that started the task.
    AutoPtr<IIntent> mAffinityIntent;  // Intent of affinity-moved activity that started this task.
    AutoPtr<IComponentName> mOrigActivity; // The non-alias activity component of the intent.
    AutoPtr<IComponentName> mRealActivity; // The actual activity component that started the task.
    Int32 mNumActivities;      // Current number of activities in this task.
    Boolean mRootWasReset;   // True if the intent at the root of the task had

    String mStringName;      // caching of toString() result.
};

#endif //__TASKRECORD_H__
