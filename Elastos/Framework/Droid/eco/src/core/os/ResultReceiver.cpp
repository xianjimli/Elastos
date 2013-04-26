
#include "os/ResultReceiver.h"
#include "os/CMyResultReceiver.h"
#include "ext/frameworkdef.h"


ResultReceiver::MyRunnable::MyRunnable(
    /* [in] */ Int32 resultCode,
    /* [in] */ IBundle* resultData,
    /* [in] */ ResultReceiver* host)
    : mResultCode(resultCode)
    , mResultData(resultData)
    , mHost(host)
{}

ResultReceiver::MyRunnable::~MyRunnable()
{}

ECode ResultReceiver::MyRunnable::Run()
{
    assert(mHost != NULL);
    mHost->OnReceiveResult(mResultCode, mResultData);
    //delete this object here
    delete this;
    return NOERROR;
}

ResultReceiver::ResultReceiver()
    : mLocal(FALSE)
{}

ResultReceiver::ResultReceiver(
    /* [in] */ IApartment* handler)
    : mLocal(TRUE)
    , mHandler(handler)
{}

ECode ResultReceiver::Init(
    /* [in] */ IApartment* handler)
{
    mLocal = TRUE;
    mHandler = handler;
    return NOERROR;
}

ECode ResultReceiver::Send(
    /* [in] */ Int32 resultCode,
    /* [in] */ IBundle* resultData)
{
    if (mLocal) {
        if (mHandler != NULL) {
        	//TODO
            //return mHandler.post(new MyRunnable(resultCode, resultData));
        }
        else {
            OnReceiveResult(resultCode, resultData);
        }
        return NOERROR;
    }

    if (mReceiver != NULL) {
        mReceiver->Send(resultCode, resultData);
    }

    return NOERROR;
}

ECode ResultReceiver::DescribeContents(
    /* [out] */ Int32* contents)
{
    VALIDATE_NOT_NULL(contents);

	*contents = 0;
    return NOERROR;
}

ECode ResultReceiver::ReadFromParcel(
    /* [in] */ IParcel* src)
{
    mLocal = FALSE;
    mHandler = NULL;
    return src->ReadInterfacePtr((Handle32*)&mReceiver);
}

ECode ResultReceiver::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    Mutex::Autolock lock(GetSelfLock());

    if (mReceiver == NULL) {
        CMyResultReceiver::New((Handle32)this, (IResultReceiver**)&mReceiver);
    }
    return dest->WriteInterfacePtr(mReceiver.Get());
}

ECode ResultReceiver::OnReceiveResult(
    /* [in] */ Int32 resultCode,
    /* [in] */ IBundle* resultData)
{
    return NOERROR;
}
