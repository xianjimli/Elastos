
#ifndef __CONTENTPROVIDERRECORD_H__
#define __CONTENTPROVIDERRECORD_H__

#include "ext/frameworkdef.h"
#include "server/ProcessRecord.h"
#include <elastos/AutoPtr.h>
#include <elastos/Set.h>
#include <elastos/etl_hash_fun.h>
#include <semaphore.h>
#include <elastos/Mutex.h>

using namespace Elastos::Core::Threading;

class ProcessRecord;

class ContentProviderRecord
{
public:
    ContentProviderRecord(
        /* [in] */ IContentProviderInfo* info,
        /* [in] */ IApplicationInfo* ai);

    ContentProviderRecord(
        /* [in] */ ContentProviderRecord* cpr);

    ~ContentProviderRecord();

    CARAPI_(Boolean) CanRunHere(
        /* [in] */ ProcessRecord* app);

    CARAPI_(void) Lock();

    CARAPI_(void) Unlock();

    CARAPI_(void) Wait();

    CARAPI_(void) Notify();

    CARAPI GetDescription(
        /* [out] */ String* description);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

public:
    // All attached clients
    Set<ProcessRecord*> mClients;
    AutoPtr<IApplicationInfo> mAppInfo;
    AutoPtr<IComponentName> mName;
    // number of non-framework processes supported by this provider
    Int32 mExternals;
    ProcessRecord* mApp; // if non-null, hosting application
    ProcessRecord* mLaunchingApp; // if non-null, waiting for this app to be launched.
    AutoPtr<IContentProviderHolder> mHolder;

private:
    Mutex mSyncLock;
    sem_t mEvent;
};

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash<ContentProviderRecord*>
{
    size_t operator()(ContentProviderRecord* s) const
    {
        Int32 hashCode;
        assert(s != NULL);
        s->GetHashCode(&hashCode);
        return (size_t)hashCode;
    }
};

_ELASTOS_NAMESPACE_END

#endif //__CONTENTPROVIDERRECORD_H__
