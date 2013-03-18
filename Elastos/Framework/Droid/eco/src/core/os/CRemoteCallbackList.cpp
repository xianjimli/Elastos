
#include "ext/frameworkdef.h"
#include "os/CRemoteCallbackList.h"


CRemoteCallbackList::CRemoteCallbackList()
    : mCallbacks(5)
    , mBroadcastCount(-1)
    , mKilled(FALSE)
{}

ECode CRemoteCallbackList::constructor()
{
    return NOERROR;
}

//private final class Callback implements IBinder.DeathRecipient {
//    final E mCallback;
//    final Object mCookie;
//
//    Callback(E callback, Object cookie) {
//        mCallback = callback;
//        mCookie = cookie;
//    }
//
//    public void binderDied() {
//        synchronized (mCallbacks) {
//            mCallbacks.remove(mCallback.asBinder());
//        }
//        onCallbackDied(mCallback, mCookie);
//    }
//}

ECode CRemoteCallbackList::Register(
    /* [in] */ IInterface* callback,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return RegisterEx(callback, NULL, result);
}

ECode CRemoteCallbackList::RegisterEx(
    /* [in] */ IInterface* callback,
    /* [in] */ IInterface* cookie,
    /* [out] */ Boolean* result)
{
//    synchronized (mCallbacks) {
//        if (mKilled) {
//            return false;
//        }
//        IBinder binder = callback.asBinder();
//        try {
//            Callback cb = new Callback(callback, cookie);
//            binder.linkToDeath(cb, 0);
//            mCallbacks.put(binder, cb);
//            return true;
//        } catch (RemoteException e) {
//            return false;
//        }
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteCallbackList::Unregister(
    /* [in] */ IInterface* callback,
    /* [out] */ Boolean* result)
{
//    synchronized (mCallbacks) {
//        Callback cb = mCallbacks.remove(callback.asBinder());
//        if (cb != null) {
//            cb.mCallback.asBinder().unlinkToDeath(cb, 0);
//            return true;
//        }
//        return false;
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteCallbackList::Kill()
{
//    synchronized (mCallbacks) {
//        for (Callback cb : mCallbacks.values()) {
//            cb.mCallback.asBinder().unlinkToDeath(cb, 0);
//        }
//        mCallbacks.clear();
//        mKilled = true;
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteCallbackList::OnCallbackDied(
    /* [in] */ IInterface* callback)
{
    return NOERROR;
}

ECode CRemoteCallbackList::OnCallbackDiedEx(
    /* [in] */ IInterface* callback,
    /* [in] */ IInterface* cookie)
{
    return OnCallbackDied(callback);
}

ECode CRemoteCallbackList::BeginBroadcast(
        /* [out] */ Int32* number)
{
//    synchronized (mCallbacks) {
//        if (mBroadcastCount > 0) {
//            throw new IllegalStateException(
//                    "beginBroadcast() called while already in a broadcast");
//        }
//
//        final int N = mBroadcastCount = mCallbacks.size();
//        if (N <= 0) {
//            return 0;
//        }
//        Object[] active = mActiveBroadcast;
//        if (active == null || active.length < N) {
//            mActiveBroadcast = active = new Object[N];
//        }
//        int i=0;
//        for (Callback cb : mCallbacks.values()) {
//            active[i++] = cb;
//        }
//        return i;
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteCallbackList::GetBroadcastItem(
    /* [in] */ Int32 index,
    /* [out] */ IInterface** callback)
{
//    return ((Callback)mActiveBroadcast[index]).mCallback;
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteCallbackList::GetBroadcastCookie(
    /* [in] */ Int32 index,
    /* [out] */ IInterface** cookie)
{
//    return ((Callback)mActiveBroadcast[index]).mCookie;
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteCallbackList::FinishBroadcast()
{
    if (mBroadcastCount < 0) {
        // throw new IllegalStateException(
            // "finishBroadcast() called outside of a broadcast");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    ArrayOf< AutoPtr<IInterface> >* active = mActiveBroadcast;
    if (active != NULL){
        const Int32 N = mBroadcastCount;
        for (Int32 i = 0; i < N; i++){
            (*active)[i] = NULL;
        }
    }
    mBroadcastCount = -1;

    return NOERROR;
}
