
#include "statusbar/CStatusBarNotification.h"

ECode CStatusBarNotification::GetDescription(
    /* [out] */ String* str)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStatusBarNotification::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadString(&mCap);
    source->ReadInt32(&mId);
    source->ReadString(&mTag);
    source->ReadInt32(&mUid);
    source->ReadInt32(&mInitialPid);
    AutoPtr<IInterface> obj;
    source->ReadInterfacePtr((Handle32*)&obj);
    mNotification = INotification::Probe(obj);
    return NOERROR;
}

ECode CStatusBarNotification::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteString(mCap);
    dest->WriteInt32(mId);
    dest->WriteString(mTag);
    dest->WriteInt32(mUid);
    dest->WriteInt32(mInitialPid);
    dest->WriteInterfacePtr(mNotification);
    return NOERROR;
}

ECode CStatusBarNotification::constructor()
{
    return NOERROR;
}

ECode CStatusBarNotification::constructor(
    /* [in] */ const String& cap,
    /* [in] */ Int32 id,
    /* [in] */ const String& tag,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 initialPid,
    /* [in] */ INotification* notification)
{
    if (mCap.IsNull() || notification == NULL) return E_NULL_POINTER_EXCEPTION;

    mCap = cap;
    mId = id;
    mTag = tag;
    mUid = uid;
    mInitialPid = initialPid;
    mNotification = notification;
    return NOERROR;
}

ECode CStatusBarNotification::constructor(
    /* [in] */ IParcel* source)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

