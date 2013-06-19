
#include "content/CInnerReceiver.h"
#include "app/ActivityManagerNative.h"
#include <StringBuffer.h>
#include <Slogger.h>

using namespace Elastos::Utility::Logging;

ECode CInnerReceiver::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}

ECode CInnerReceiver::PerformReceive(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 resultCode,
    /* [in] */ const String& data,
    /* [in] */ IBundle* extras,
    /* [in] */ Boolean ordered,
    /* [in] */ Boolean sticky)
{
    if (CApplicationApartment::DEBUG_BROADCAST) {
        Int32 seq = -1;
        intent->GetInt32Extra(String("seq"), -1, &seq);
        String action;
        intent->GetAction(&action);
        Slogger::I(CApplicationApartment::TAG,
                StringBuffer("Receiving broadcast ") + action +
                " seq=" + seq + " to " + (Int32)mDispatcher);
    }
    if ((LoadedCap::ReceiverDispatcher*)mDispatcher != NULL) {
        return ((LoadedCap::ReceiverDispatcher*)mDispatcher)->PerformReceive(
            intent, resultCode, data, extras, ordered, sticky);
    }
    else {
        // The activity manager dispatched a broadcast to a registered
        // receiver in this process, but before it could be delivered the
        // receiver was unregistered.  Acknowledge the broadcast on its
        // behalf so that the system's broadcast sequence can continue.
        if (CApplicationApartment::DEBUG_BROADCAST) {
            Slogger::I(CApplicationApartment::TAG,
                "Finishing broadcast to unregistered receiver");
        }
        AutoPtr<IActivityManager> mgr;
        ActivityManagerNative::GetDefault((IActivityManager**)&mgr);
        //try {
        ECode ec = mgr->FinishReceiver((IBinder*)(IIntentReceiver*)this,
                resultCode, data, extras, FALSE);
        if (FAILED(ec)) {
            Slogger::W(CApplicationApartment::TAG,
                    "Couldn't finish broadcast to unregistered receiver");
        }
        // } catch () {
        //
        // }
    }
    return NOERROR;
}

ECode CInnerReceiver::GetHashCode(
    /* [out] */ Int32 * hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    *hashCode = (Int32)this;
    return NOERROR;
}

ECode CInnerReceiver::constructor(
    /* [in] */ Handle32 rd,
    /* [in] */ Boolean strong)
{
    mDispatcher = rd;
    return NOERROR;
}

