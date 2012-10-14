
#ifdef _FRAMEWORK_CORE
#include "content/BroadcastReceiver.h"
#include "os/CBundle.h"
#else
#include "BroadcastReceiver.h"
#endif


BroadcastReceiver::BroadcastReceiver()
{
}

BroadcastReceiver::~BroadcastReceiver()
{
}

ECode BroadcastReceiver::Initialize()
{
    return NOERROR;
}

PInterface BroadcastReceiver::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IBroadcastReceiver*)this;
    }
    else if (riid == EIID_IObject) {
        return (IObject*)this;
    }
    if (riid == EIID_IBroadcastReceiver) {
        return (IBroadcastReceiver*)this;
    }

    return NULL;
}

UInt32 BroadcastReceiver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 BroadcastReceiver::Release()
{
    return ElRefBase::Release();
}

ECode BroadcastReceiver::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IBroadcastReceiver *)this) {
        *pIID = EIID_IBroadcastReceiver;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IObject *)this) {
        *pIID = EIID_IObject;
        return NOERROR;
    }

    return E_INVALID_ARGUMENT;
}

ECode BroadcastReceiver::Aggregate(
    /* [in] */ AggrType aggrType,
    /* [in] */ PInterface pObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode BroadcastReceiver::GetDomain(
    /* [out] */ PInterface *ppObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode BroadcastReceiver::GetClassID(
    /* [out] */ ClassID *pCLSID)
{
    return E_NOT_IMPLEMENTED;
}

ECode BroadcastReceiver::ReceiveBroadcast(
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
ECode BroadcastReceiver::SetResultCode(
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
ECode BroadcastReceiver::GetResultCode(
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
ECode BroadcastReceiver::SetResultData(
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
ECode BroadcastReceiver::GetResultData(
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
ECode BroadcastReceiver::SetResultExtras(
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
ECode BroadcastReceiver::GetResultExtras(
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
ECode BroadcastReceiver::SetResult(
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
ECode BroadcastReceiver::GetAbortBroadcast(
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
ECode BroadcastReceiver::AbortBroadcast()
{
    CheckSynchronousHint();
    mAbortBroadcast = TRUE;
    return NOERROR;
}

/**
 * Clears the flag indicating that this receiver should abort the current
 * broadcast.
 */
ECode BroadcastReceiver::ClearAbortBroadcast()
{
    mAbortBroadcast = FALSE;
    return NOERROR;
}

/**
 * For internal use, sets the hint about whether this BroadcastReceiver is
 * running in ordered mode.
 */
ECode BroadcastReceiver::SetOrderedHint(
    /* [in] */ Boolean isOrdered)
{
    mOrderedHint = isOrdered;
    return NOERROR;
}

/**
 * For internal use, sets the hint about whether this BroadcastReceiver is
 * receiving the initial sticky broadcast value. @hide
 */
ECode BroadcastReceiver::SetInitialStickyHint(
    /* [in] */ Boolean isInitialSticky)
{
   mInitialStickyHint = isInitialSticky;
   return NOERROR;
}

ECode BroadcastReceiver::OnReceive(
    /* [in] */ IContext *pContext,
    /* [in] */ IIntent *pIntent)
{
    return NOERROR;
}

void BroadcastReceiver::CheckSynchronousHint()
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
