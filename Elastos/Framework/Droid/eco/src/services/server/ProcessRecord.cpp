
#include "server/ProcessRecord.h"

ProcessRecord::ProcessRecord(
    /* [in] */ BatteryStatsImpl::Uid::Proc* batteryStats,
    /* [in] */ IApplicationApartment* appApartment,
    /* [in] */ IApplicationInfo* info,
    /* [in] */ const String& processName) :
    mPid(0),
    mPubProviders(5),
    mConProviders(5),
    mPersistent(FALSE),
    mCrashing(FALSE),
    mNotResponding(FALSE),
    mRemoved(FALSE),
    mDebugging(FALSE)
{
    mBatteryStats = batteryStats;
    mInfo = info;
    mProcessName = processName;
    mAppApartment = appApartment;
}

ProcessRecord::~ProcessRecord()
{
    mPubProviders.Clear();
    mConProviders.Clear();
}

void ProcessRecord::SetPid(
    /* [in] */ Int32 pid)
{
    mPid = pid;
    mShortStringName = NULL;
    mStringName = NULL;
}

/*
 *  Return true if package has been added false if not
 */
Boolean ProcessRecord::AddCapsule(
    /* [in] */ const String& cap)
{
//    Set<String>::Iterator it =
//            Find(mCapList.Begin(), mCapList.End(), cap);
//    if (it == mCapList.End()) return FALSE;
//    mCapList.Insert(cap);
    return TRUE;
}

void ProcessRecord::ResetCapsuleList()
{
}

ECode ProcessRecord::GetShortDescription(
    /* [out] */ String* description)
{
    if (description == NULL) return E_INVALID_ARGUMENT;

    if (!mShortStringName.IsNull()) {
        *description = mShortStringName;
        return NOERROR;
    }
    StringBuffer sb;
    GetShortDescription(sb);
    mShortStringName = (const char*)sb;
    *description = mShortStringName;
    return NOERROR;
}

void ProcessRecord::GetShortDescription(
    /* [in] */ StringBuffer& sb)
{
//    sb.append(Integer.toHexString(System.identityHashCode(this)));
    Int32 uid;
    mInfo->GetUid(&uid);
    sb = sb + " " + mPid + ":" + mProcessName + "/" + uid;
}

ECode ProcessRecord::GetDescription(
    /* [out] */ String* description)
{
    if (description == NULL) return E_INVALID_ARGUMENT;

    if (!mStringName.IsNull()) {
        *description = mStringName;
        return NOERROR;
    }
    StringBuffer sb;
    sb += "ProcessRecord{";
    GetShortDescription(sb);
    sb += "}";
    mStringName = (const char*)sb;
    *description = mStringName;
    return NOERROR;
}
