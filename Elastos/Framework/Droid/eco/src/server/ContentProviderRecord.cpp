
#include "server/CActivityManagerService.h"
#include "server/ContentProviderRecord.h"

ContentProviderRecord::ContentProviderRecord(
    /* [in] */ CContentProviderInfo* info,
    /* [in] */ CApplicationInfo* ai) :
    mExternals(0)
{
    sem_init(&mEvent, 0, 0);

    ECode ec = CContentProviderHolder::NewByFriend(
            (CContentProviderHolder**)&mHolder);
    assert(SUCCEEDED(ec));
    mHolder->mInfo = info;
    mAppInfo = ai;
    CComponentName::New(info->mCapsuleName, info->mName, (IComponentName**)&mName);
}

ContentProviderRecord::ContentProviderRecord(
    /* [in] */ ContentProviderRecord* cpr) :
    mExternals(0)
{
    sem_init(&mEvent, 0, 0);

    ECode ec = CContentProviderHolder::NewByFriend(
            (CContentProviderHolder**)&mHolder);
    assert(SUCCEEDED(ec));
    mHolder->mInfo = cpr->mHolder->mInfo;
    mName = cpr->mName;
}

ContentProviderRecord::~ContentProviderRecord()
{
    sem_destroy(&mEvent);
}

Boolean ContentProviderRecord::CanRunHere(
    /* [in] */ ProcessRecord* app)
{
//    return (info.multiprocess || info.processName.equals(app.processName))
//            && (uid == Process.SYSTEM_UID || uid == app.info.uid);
    if (!mHolder->mInfo->mProcessName.Compare(app->mProcessName)) {
        return TRUE;
    }
    return FALSE;
}

void ContentProviderRecord::Lock()
{
    mSyncLock.Lock();
}

void ContentProviderRecord::Unlock()
{
    mSyncLock.Unlock();
}

void ContentProviderRecord::Wait()
{
    sem_wait(&mEvent);
}

void ContentProviderRecord::Notify()
{
    sem_post(&mEvent);
}

ECode ContentProviderRecord::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
//    if (stringName != null) {
//        return stringName;
//    }
//    StringBuilder sb = new StringBuilder(128);
//    sb.append("ContentProviderRecord{");
//    sb.append(Integer.toHexString(System.identityHashCode(this)));
//    sb.append(' ');
//    sb.append(info.name);
//    sb.append('}');
//    return stringName = sb.toString();
}

ECode ContentProviderRecord::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    assert(hashCode != NULL);
    *hashCode = (Int32)this;
    return NOERROR;
}
