
#include "ext/frameworkdef.h"
#include "view/inputmethod/CInputBinding.h"


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
    binding->GetConnectionToken((IBinder**)&mConnectionToken);
    binding->GetUid(&mUid);
    binding->GetPid(&mPid);

    return NOERROR;
}

ECode CInputBinding::constructor()
{
    mConnection = NULL;
    return NOERROR;
}

ECode CInputBinding::GetConnection(
    /* [out] */ IInputConnection** inputConnection)
{
	VALIDATE_NOT_NULL(inputConnection);

    *inputConnection = mConnection;
    if (*inputConnection != NULL) {
        (*inputConnection)->AddRef();
    }

    return NOERROR;
}

ECode CInputBinding::GetConnectionToken(
    /* [out] */ IBinder** binder)
{
    VALIDATE_NOT_NULL(binder);

    *binder = mConnectionToken;
    if (*binder != NULL) {
        (*binder)->AddRef();
    }

    return NOERROR;
}

ECode CInputBinding::GetUid(
    /* [out] */ Int32* uid)
{
    VALIDATE_NOT_NULL(uid);

    *uid = mUid;
    return NOERROR;
}

ECode CInputBinding::GetPid(
    /* [out] */ Int32* pid)
{
    VALIDATE_NOT_NULL(pid);

    *pid = mPid;
    return NOERROR;
}

ECode CInputBinding::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    AutoPtr<IInterface> obj;
    source->ReadInterfacePtr((Handle32*)&obj);
    assert(obj != NULL);
    mConnectionToken = IBinder::Probe(obj.Get());
    source->ReadInt32(&mUid);
    source->ReadInt32(&mPid);
    return NOERROR;
}

ECode CInputBinding::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    dest->WriteInterfacePtr((IInterface*)mConnectionToken.Get());
    dest->WriteInt32(mUid);
    dest->WriteInt32(mPid);
    return NOERROR;
}

ECode CInputBinding::DescribeContents(
    /* [out] */ Int32* contents)
{
    VALIDATE_NOT_NULL(contents);

    *contents = 0;
    return NOERROR;
}

ECode CInputBinding::ToString(
    /* [out] */ String* str)
{
    assert(0);
    return NOERROR;
}