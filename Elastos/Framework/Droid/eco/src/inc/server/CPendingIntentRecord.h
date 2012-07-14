
#ifndef __CPENDINGINTENTRECORD_H__
#define __CPENDINGINTENTRECORD_H__

#include "_CPendingIntentRecord.h"
#include "server/CActivityManagerService.h"
#include "server/Key.h"
#include <elastos/etl_hash_fun.h>
#include <elastos/etl_function.h>

CarClass(CPendingIntentRecord)
{
public:
    CPendingIntentRecord();

    CARAPI Send(
        /* [in] */ Int32 code,
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ IIntentReceiver* finishedReceiver,
        /* [out] */ Int32* result);

    CARAPI SendIntent(
        /* [in] */ IContext* context,
        /* [in] */ Int32 code,
        /* [in] */ IIntent* intent,
        /* [in] */ IOnFinished* onFinished,
        /* [in] */ IHandler* handler);

public:
    CARAPI Initialize(
        /* [in] */ CActivityManagerService* owner,
        /* [in] */ Key* k,
        /* [in] */ Int32 u);

    CARAPI_(Int32) SendInner(
        /* [in] */ Int32 code,
        /* [in] */ IIntent* intent,
        /* [in] */ const String& resolvedType,
        /* [in] */ IIntentReceiver* finishedReceiver,
        /* [in] */ IBinder* resultTo,
        /* [in] */ const String& resultWho,
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 flagsMask,
        /* [in] */ Int32 flagsValues);

public:
    AutoPtr<CActivityManagerService> mOwner;
    Key* mKey;
    Int32 mUid;
    Boolean mSent;
    Boolean mCanceled;

    String mStringName;
};

#endif //__CPENDINGINTENTRECORD_H__
