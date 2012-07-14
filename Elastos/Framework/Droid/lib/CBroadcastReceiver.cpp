
#include "CBroadcastReceiver.h"

CBroadcastReceiver::CBroadcastReceiver()
{
}

CBroadcastReceiver::~CBroadcastReceiver()
{
}

ECode CBroadcastReceiver::Initialize()
{
    return NOERROR;
}

PInterface CBroadcastReceiver::Probe(
    /* [in] */ REIID riid)
{
   if (riid == EIID_IBroadcastReceiver) {
        return (IBroadcastReceiver *)this;
   }
   else {
       return CBaseObject::Probe(riid);
   }
}

UInt32 CBroadcastReceiver::AddRef()
{
    Int32 nRef = atomic_inc(&mRef);
    return (UInt32)nRef;
}

UInt32 CBroadcastReceiver::Release()
{
    Int32 nRef;

    nRef = atomic_dec(&mRef);
    if (nRef == 0) {
        delete this;
    }
    return (UInt32)nRef;
}

ECode CBroadcastReceiver::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IBroadcastReceiver *)this) {
        *pIID = EIID_IBroadcastReceiver;
        return NOERROR;
    }
    else {
        return CBaseObject::GetInterfaceID(pObject, pIID);
    }
}

ECode CBroadcastReceiver::GetClassID(
    /* [out] */ ClassID *pCLSID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CBroadcastReceiver::ReceiveBroadcast(
    /* [in] */ IContext *pContext,
    /* [in] */ IIntent *pIntent)
{
    ECode ec = NOERROR;

    ec = OnReceive(pContext, pIntent);

    return ec;
}

/**
 * Change the current result code of this broadcast; only works with
 * broadcasts sent through
 * {@link Context#sendOrderedBroadcast(Intent, String)
 * Context.sendOrderedBroadcast}.  Often uses the
 * Activity {@link android.app.Activity#RESULT_CANCELED} and
 * {@link android.app.Activity#RESULT_OK} constants, though the
 * actual meaning of this value is ultimately up to the broadcaster.
 *
 * <p><strong>This method does not work with non-ordered broadcasts such
 * as those sent with {@link Context#sendBroadcast(Intent)
 * Context.sendBroadcast}</strong></p>
 *
 * @param code The new result code.
 *
 * @see #setResult(int, String, Bundle)
 */
ECode CBroadcastReceiver::SetResultCode(
    /* [in] */ Int32 code)
{
    CheckSynchronousHint();
    mResultCode = code;
    return NOERROR;
}

/**
 * Retrieve the current result code, as set by the previous receiver.
 *
 * @return int The current result code.
 */
ECode CBroadcastReceiver::GetResultCode(
    /* [out] */ Int32* code)
{
    if (code == NULL) return E_INVALID_ARGUMENT;

    *code = mResultCode;
    return NOERROR;
}

/**
 * Change the current result data of this broadcast; only works with
 * broadcasts sent through
 * {@link Context#sendOrderedBroadcast(Intent, String)
 * Context.sendOrderedBroadcast}.  This is an arbitrary
 * string whose interpretation is up to the broadcaster.
 *
 * <p><strong>This method does not work with non-ordered broadcasts such
 * as those sent with {@link Context#sendBroadcast(Intent)
 * Context.sendBroadcast}</strong></p>
 *
 * @param data The new result data; may be null.
 *
 * @see #setResult(int, String, Bundle)
 */
ECode CBroadcastReceiver::SetResultData(
    /* [in] */ const String& data)
{
    CheckSynchronousHint();
    mResultData = data;
    return NOERROR;
}

/**
 * Retrieve the current result data, as set by the previous receiver.
 * Often this is null.
 *
 * @return String The current result data; may be null.
 */
ECode CBroadcastReceiver::GetResultData(
    /* [out] */ String* data)
{
    if (data == NULL) return E_INVALID_ARGUMENT;

    *data = mResultData;
    return NOERROR;
}

/**
 * Change the current result extras of this broadcast; only works with
 * broadcasts sent through
 * {@link Context#sendOrderedBroadcast(Intent, String)
 * Context.sendOrderedBroadcast}.  This is a Bundle
 * holding arbitrary data, whose interpretation is up to the
 * broadcaster.  Can be set to null.  Calling this method completely
 * replaces the current map (if any).
 *
 * <p><strong>This method does not work with non-ordered broadcasts such
 * as those sent with {@link Context#sendBroadcast(Intent)
 * Context.sendBroadcast}</strong></p>
 *
 * @param extras The new extra data map; may be null.
 *
 * @see #setResult(int, String, Bundle)
 */
ECode CBroadcastReceiver::SetResultExtras(
    /* [in] */ IBundle* extras)
{
    CheckSynchronousHint();
    mResultExtras = extras;
    return NOERROR;
}

/**
 * Retrieve the current result extra data, as set by the previous receiver.
 * Any changes you make to the returned Map will be propagated to the next
 * receiver.
 *
 * @param makeMap If true then a new empty Map will be made for you if the
 *                current Map is null; if false you should be prepared to
 *                receive a null Map.
 *
 * @return Map The current extras map.
 */
ECode CBroadcastReceiver::GetResultExtras(
    /* [in] */ Boolean makeMap,
    /* [out] */ IBundle** extras)
{
    if (extras == NULL) return E_INVALID_ARGUMENT;

    if (makeMap && mResultExtras == NULL) {
        CBundle::New((IBundle**)&mResultExtras);
    }
    *extras = mResultExtras;
    if (*extras != NULL) (*extras)->AddRef();
    return NOERROR;

}

/**
 * Change all of the result data returned from this broadcasts; only works
 * with broadcasts sent through
 * {@link Context#sendOrderedBroadcast(Intent, String)
 * Context.sendOrderedBroadcast}.  All current result data is replaced
 * by the value given to this method.
 *
 * <p><strong>This method does not work with non-ordered broadcasts such
 * as those sent with {@link Context#sendBroadcast(Intent)
 * Context.sendBroadcast}</strong></p>
 *
 * @param code The new result code.  Often uses the
 * Activity {@link android.app.Activity#RESULT_CANCELED} and
 * {@link android.app.Activity#RESULT_OK} constants, though the
 * actual meaning of this value is ultimately up to the broadcaster.
 * @param data The new result data.  This is an arbitrary
 * string whose interpretation is up to the broadcaster; may be null.
 * @param extras The new extra data map.  This is a Bundle
 * holding arbitrary data, whose interpretation is up to the
 * broadcaster.  Can be set to null.  This completely
 * replaces the current map (if any).
 */
ECode CBroadcastReceiver::SetResult(
    /* [in] */ Int32 code,
    /* [in] */ const String& data,
    /* [in] */ IBundle* extras)
{
    CheckSynchronousHint();
    mResultCode = code;
    mResultData = data;
    mResultExtras = extras;
    return NOERROR;
}

/**
 * Returns the flag indicating whether or not this receiver should
 * abort the current broadcast.
 *
 * @return True if the broadcast should be aborted.
 */
ECode CBroadcastReceiver::GetAbortBroadcast(
    /* [out] */ Boolean* aborted)
{
    if (aborted == NULL) return E_INVALID_ARGUMENT;

    *aborted = mAbortBroadcast;
    return NOERROR;
}

/**
 * Sets the flag indicating that this receiver should abort the
 * current broadcast; only works with broadcasts sent through
 * {@link Context#sendOrderedBroadcast(Intent, String)
 * Context.sendOrderedBroadcast}.  This will prevent
 * any other broadcast receivers from receiving the broadcast. It will still
 * call {@link #onReceive} of the BroadcastReceiver that the caller of
 * {@link Context#sendOrderedBroadcast(Intent, String)
 * Context.sendOrderedBroadcast} passed in.
 *
 * <p><strong>This method does not work with non-ordered broadcasts such
 * as those sent with {@link Context#sendBroadcast(Intent)
 * Context.sendBroadcast}</strong></p>
 */
ECode CBroadcastReceiver::AbortBroadcast()
{
    CheckSynchronousHint();
    mAbortBroadcast = TRUE;
    return NOERROR;
}

/**
 * Clears the flag indicating that this receiver should abort the current
 * broadcast.
 */
ECode CBroadcastReceiver::ClearAbortBroadcast()
{
    mAbortBroadcast = FALSE;
    return NOERROR;
}

/**
 * For internal use, sets the hint about whether this BroadcastReceiver is
 * running in ordered mode.
 */
ECode CBroadcastReceiver::SetOrderedHint(
    /* [in] */ Boolean isOrdered)
{
    mOrderedHint = isOrdered;
    return NOERROR;
}

/**
 * For internal use, sets the hint about whether this BroadcastReceiver is
 * receiving the initial sticky broadcast value. @hide
 */
ECode CBroadcastReceiver::SetInitialStickyHint(
    /* [in] */ Boolean isInitialSticky)
{
   mInitialStickyHint = isInitialSticky;
   return NOERROR;
}

ECode CBroadcastReceiver::OnReceive(
    /* [in] */ IContext *pContext,
    /* [in] */ IIntent *pIntent)
{
    return NOERROR;
}

void CBroadcastReceiver::CheckSynchronousHint()
{
    // Note that we don't assert when receiving the initial sticky value,
    // since that may have come from an ordered broadcast.  We'll catch
    // them later when the real broadcast happens again.
    if (mOrderedHint || mInitialStickyHint) {
        return;
    }
//    RuntimeException e = new RuntimeException(
//            "BroadcastReceiver trying to return result during a non-ordered broadcast");
//    e.fillInStackTrace();
//    Log.e("BroadcastReceiver", e.getMessage(), e);
}
