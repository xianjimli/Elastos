
#include "view/inputmethod/CInputBinding.h"

CInputBinding::CInputBinding()
{}

ECode CInputBinding::constructor(
    /* [in] */ IInputConnection* conn,
    /* [in] */ IBinder* connToken,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 pid)
{
    mConnection = conn;
    mConnectionToken = connToken;
    mUid = uid;
    mPid = pid;

    return NOERROR;
}

ECode CInputBinding::constructor(
    /* [in] */ IInputConnection* conn,
    /* [in] */ IInputBinding* binding)
{
    mConnection = conn;

    assert(binding != NULL);
    binding->GetConnectionToken((IBinder**) &mConnectionToken);
    binding->GetUid(&mUid);
    binding->GetPid(&mPid);

    return NOERROR;
}

ECode CInputBinding::constructor(
    /* [in] */ IParcel* source)
{
    assert(0);

    mConnection = NULL;
    // mConnectionToken = source.readStrongBinder();
    // mUid = source.readInt();
    // mPid = source.readInt();

    return NOERROR;
}

ECode CInputBinding::GetConnection(
    /* [out] */ IInputConnection** inputConnection)
{
	assert(inputConnection != NULL);
    *inputConnection = mConnection;
    if (*inputConnection != NULL) {
        (*inputConnection)->AddRef();
    }

    return NOERROR;
}

ECode CInputBinding::GetConnectionToken(
    /* [out] */ IBinder** binder)
{
    assert(binder != NULL);
    *binder = mConnectionToken;
    if (*binder != NULL) {
        (*binder)->AddRef();
    }

    return NOERROR;
}

ECode CInputBinding::GetUid(
    /* [out] */ Int32* uid)
{
    assert(uid != NULL);
    *uid = mUid;

    return NOERROR;
}

ECode CInputBinding::GetPid(
    /* [out] */ Int32* pid) {
    assert(pid != NULL);
    *pid = mPid;

    return NOERROR;
}

ECode CInputBinding::ReadFromParcel(
    /* [in] */ IParcel *source)
{
	assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CInputBinding::WriteToParcel(
    /* [in] */ IParcel *dest)
{
	assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CInputBinding::DescribeContents(
    /* [out] */ Int32* contents)
{
    assert(contents != NULL);
    *contents = 0;

    return NOERROR;
}

ECode CInputBinding::ToString(
    /* [out] */ String* str)
{
    assert(0);

    return NOERROR;
}