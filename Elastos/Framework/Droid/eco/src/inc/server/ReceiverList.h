
#ifndef __RECEIVERLIST_H__
#define __RECEIVERLIST_H__

#include "ext/frameworkext.h"
#include <elastos/List.h>
#include <elastos/AutoPtr.h>
#include <elastos/etl_hash_fun.h>
#include "server/BroadcastFilter.h"
#include "server/ProcessRecord.h"
#include "server/BroadcastRecord.h"

class BroadcastFilter;
class ProcessRecord;
class BroadcastRecord;

class ReceiverList : public List<BroadcastFilter*>
{
public:
    ReceiverList();

public:
    AutoPtr<IActivityManager> mOwner;
    AutoPtr<IIntentReceiver> mReceiver;
    ProcessRecord* mApp;
    Int32 mPid;
    Int32 mUid;
    BroadcastRecord* mCurBroadcast;
};

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash<AutoPtr<IIntentReceiver> >
{
    size_t operator()(AutoPtr<IIntentReceiver> s) const
    {
        Int32 hashCode;
        assert(s != NULL);
        s->GetHashCode(&hashCode);
        return (size_t)hashCode;
    }
};

_ELASTOS_NAMESPACE_END

#endif //__RECEIVERLIST_H__
