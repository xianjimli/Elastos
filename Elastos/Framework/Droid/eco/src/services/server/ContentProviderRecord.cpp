
#include "server/CActivityManagerService.h"
#include "server/ContentProviderRecord.h"

ContentProviderRecord::ContentProviderRecord(
    /* [in] */ IContentProviderInfo* info,
    /* [in] */ IApplicationInfo* ai) :
    mExternals(0)
{
    sem_init(&mEvent, 0, 0);

    ECode ec = CContentProviderHolder::New(
            (IContentProviderHolder**)&mHolder);
    assert(SUCCEEDED(ec));
    mHolder->SetContentProviderInfo(info);
    mAppInfo = ai;
    String cname, name;
    info->GetCapsuleName(&cname);
    info->GetName(&name);
    CComponentName::New(cname, name, (IComponentName**)&mName);
}

ContentProviderRecord::ContentProviderRecord(
    /* [in] */ ContentProviderRecord* cpr) :
    mExternals(0)
{
    sem_init(&mEvent, 0, 0);

    ECode ec = CContentProviderHolder::New(
            (IContentProviderHolder**)&mHolder);
    assert(SUCCEEDED(ec));
    AutoPtr<IContentProviderInfo> info;
    cpr->mHolder->GetContentProviderInfo((IContentProviderInfo**)&info);
    mHolder->SetContentProviderInfo(info);
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
    AutoPtr<IContentProviderInfo> info;
    mHolder->GetContentProviderInfo((IContentProviderInfo**)&info);
    String pname;
    info->GetProcessName(&pname);
    if (!pname.Compare(app->mProcessName)) {
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
