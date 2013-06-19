
#ifndef __BROADCASTFILTER_H__
#define __BROADCASTFILTER_H__

#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>
#include "server/ReceiverList.h"

using namespace Elastos;

extern "C" const ClassID ECLSID_BroadcastFilter;

class ReceiverList;

class BroadcastFilter : public ElRefBase, public IObject
{
public:
    BroadcastFilter();

    BroadcastFilter(
        /* [in] */ IIntentFilter* intentFilter,
        /* [in] */ ReceiverList* receiverList,
        /* [in] */ const String& requiredPermission);

    ~BroadcastFilter();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI Aggregate(
        /* [in] */ AggregateType type,
        /* [in] */ PInterface pObj);

    CARAPI GetDomain(
        /* [out] */ PInterface *ppObj);

    CARAPI GetClassID(
        /* [out] */ ClassID *pCLSID);

    CARAPI_(Int32) GetPriority();

    CARAPI GetDescription(
        /* [out] */ String* description);

public:
    AutoPtr<IIntentFilter> mFilter;
    ReceiverList* mReceiverList;
    String mRequiredPermission;
};

#endif //__BROADCASTFILTER_H__
