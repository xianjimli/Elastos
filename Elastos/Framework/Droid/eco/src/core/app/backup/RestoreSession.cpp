
#include "app/backup/RestoreSession.h"
#include "app/backup/CRestoreObserverWrapper.h"

RestoreSession::RestoreSession(
    /* [in] */ IContext* context,
    /* [in] */ IRestoreSessionStub* binder):
    mContext(context),
    mBinder(binder)
{
}

RestoreSession::~RestoreSession()
{

}

UInt32 RestoreSession::AddRef()
{
	return ElRefBase::AddRef();
}

UInt32 RestoreSession::Release()
{
	return ElRefBase::Release();
}

ECode RestoreSession::GetAvailableRestoreSets(
    /* [in] */ IRestoreObserver* observer,
    /* [out] */ Int32* error)
{
	assert(error != NULL);
	*error = -1;

	AutoPtr<IRestoreObserverStub> obsWrapper;
    CRestoreObserverWrapper::New(mContext, observer, (IRestoreObserverStub**) &obsWrapper);
    assert(obsWrapper != NULL);

//    try {
    mBinder->GetAvailableRestoreSets(obsWrapper, error);
    // } catch (RemoteException e) {
    //     Log.d(TAG, "Can't contact server to get available sets");
    // }
    return NOERROR;
}

ECode RestoreSession::RestoreAll(
    /* [in] */ Int64 token,
    /* [in] */ IRestoreObserver* observer,
    /* [out] */ Int32* error)
{
	assert(error != NULL);
	*error = -1;

    if (mObserver != NULL) {
        //Log.d(TAG, "restoreAll() called during active restore");
        return NOERROR;
    }

    CRestoreObserverWrapper::New(mContext, observer, (IRestoreObserverStub**) &mObserver);
    assert(mObserver != NULL);

    //try {
        mBinder->RestoreAll(token, mObserver, error);
    // } catch (RemoteException e) {
    //     Log.d(TAG, "Can't contact server to restore");
    // }

    return NOERROR;
}

ECode RestoreSession::RestoreCapsule(
    /* [in] */ const String& capsule,
    /* [in] */ IRestoreObserver* observer,
    /* [out] */ Int32* error)
{
	assert(error != NULL);
	*error = -1;

    if (mObserver != NULL) {
        //Log.d(TAG, "restorePackage() called during active restore");
        return NOERROR;
    }

    CRestoreObserverWrapper::New(mContext, observer, (IRestoreObserverStub**) &mObserver);
    assert(mObserver != NULL);

    //try {
        mBinder->RestoreCapsule(capsule, mObserver, error);
    // } catch (RemoteException e) {
    //     Log.d(TAG, "Can't contact server to restore package");
    // }

    return NOERROR;
}

ECode RestoreSession::EndRestoreSession()
{
    // try {
    //     mBinder.endRestoreSession();
    // } catch (RemoteException e) {
    //     Log.d(TAG, "Can't contact server to get available sets");
    // } finally {
    //     mBinder = NULL;
    // }

	mBinder->EndRestoreSession();
	mBinder = NULL;
    return NOERROR;
}
