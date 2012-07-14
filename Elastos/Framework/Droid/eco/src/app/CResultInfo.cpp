
#include "app/CResultInfo.h"

ECode CResultInfo::constructor(
    /* [in] */ const String& resultWho,
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* data)
{
    mResultWho = resultWho;
    mRequestCode = requestCode;
    mResultCode = resultCode;
    mData = data;

    return NOERROR;
}

ECode CResultInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    AutoPtr<IInterface> obj;
    source->ReadString(&mResultWho);
    source->ReadInt32(&mRequestCode);
    source->ReadInt32(&mResultCode);
    source->ReadInterfacePtrPtr((Handle32*)&obj);
    mData = obj != NULL ?
            (IIntent*)obj->Probe(EIID_IIntent) : NULL;

    return NOERROR;
}

ECode CResultInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteString(mResultWho);
    dest->WriteInt32(mRequestCode);
    dest->WriteInt32(mResultCode);
    dest->WriteInterfacePtr((IIntent*)mData);

    return NOERROR;
}
