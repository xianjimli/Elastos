
#include "os/ResultReceiver.h"
#include "ext/frameworkdef.h"


ResultReceiver::MyRunnable::MyRunnable(
    /* [in] */ Int32 resultCode,
    /* [in] */ IBundle* resultData,
    /* [in] */ ResultReceiver* host)
    : mResultCode(resultCode)
    , mResultData(resultData)
    , mHost(host)
{}

ECode ResultReceiver::MyRunnable::Run()
{
    assert(mHost != NULL);
    mHost->OnReceiveResult(mResultCode, mResultData);
    return NOERROR;
}

ResultReceiver::MyResultReceiver::MyResultReceiver(
    /* [in] */ ResultReceiver* host)
    : mHost(host)
{}

ECode ResultReceiver::MyResultReceiver::Send(
    /* [in] */ Int32 resultCode,
    /* [in] */ IBundle* resultData)
{
    if (mHost->mHandler != NULL) {
        //mHandler.post(new MyRunnable(resultCode, resultData, mHost));
        AutoPtr<IRunnable> runnable = new MyRunnable(resultCode, resultData, mHost);
        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInterfacePtr(runnable);

        assert(0);
        //TODO
        // mHandler->PostCppCallbackDelayed(
        //     (Handle32)mMyRunnable.Get(), 0, params, 0);
    }
    else {
        mHost->OnReceiveResult(resultCode, resultData);
    }

    return NOERROR;
}

PInterface ResultReceiver::MyResultReceiver::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IResultReceiver == riid) {
        return (IResultReceiver *)this;
    }

    return NULL;
}

UInt32 ResultReceiver::MyResultReceiver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ResultReceiver::MyResultReceiver::Release()
{
    return ElRefBase::Release();
}

ECode ResultReceiver::MyResultReceiver::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IResultReceiver*)this) {
        *pIID = EIID_IResultReceiver;
    }

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
    /* [in] */ IParcel *dest)
{
    mLocal = FALSE;
    mHandler = NULL;

    //TODO
    //mReceiver = IResultReceiver.Stub.asInterface(in.readStrongBinder());

    return E_NOT_IMPLEMENTED;
}

ECode ResultReceiver::WriteToParcel(
    /* [in] */ IParcel* out)
{
    assert(0);
	//TODO
    // synchronized (this) {
    //     if (mReceiver == null) {
    //         mReceiver = new MyResultReceiver();
    //     }
    //     out.writeStrongBinder(mReceiver.asBinder());
    // }

    return E_NOT_IMPLEMENTED;
}

ECode ResultReceiver::OnReceiveResult(
    /* [in] */ Int32 resultCode,
    /* [in] */ IBundle* resultData)
{
    return NOERROR;
}
