
#include "os/CResultReceiver.h"
#include "ext/frameworkdef.h"


CResultReceiver::MyRunnable::MyRunnable(
    /* [in] */ Int32 resultCode,
    /* [in] */ IBundle* resultData,
    /* [in] */ CResultReceiver* host)
    : mResultCode(resultCode)
    , mResultData(resultData)
    , mHost(host)
{}

ECode CResultReceiver::MyRunnable::Run()
{
    assert(mHost != NULL);
    mHost->OnReceiveResult(mResultCode, mResultData);
    return NOERROR;
}


CResultReceiver::MyResultReceiver::MyResultReceiver(
    /* [in] */ CResultReceiver* host)
    : mHost(host)
{}

ECode CResultReceiver::MyResultReceiver::Send(
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

PInterface CResultReceiver::MyResultReceiver::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IResultReceiver == riid) {
        return (IResultReceiver *)this;
    }

    return NULL;
}

UInt32 CResultReceiver::MyResultReceiver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CResultReceiver::MyResultReceiver::Release()
{
    return ElRefBase::Release();
}

ECode CResultReceiver::MyResultReceiver::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IResultReceiver*)this) {
        *pIID = EIID_IResultReceiver;
    }

    return NOERROR;
}

CResultReceiver::CResultReceiver()
    : mLocal(FALSE)
{}

ECode CResultReceiver::constructor(
    /* [in] */ IApartment* handler)
{
    mLocal = TRUE;
    mHandler = handler;

    return NOERROR;
}

ECode CResultReceiver::Send(
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

ECode CResultReceiver::DescribeContents(
    /* [out] */ Int32* contents)
{
    VALIDATE_NOT_NULL(contents);

	*contents = 0;
    return NOERROR;
}

ECode CResultReceiver::ReadFromParcel(
    /* [in] */ IParcel *dest)
{
    mLocal = FALSE;
    mHandler = NULL;

    //TODO
    //mReceiver = IResultReceiver.Stub.asInterface(in.readStrongBinder());

    return E_NOT_IMPLEMENTED;
}

ECode CResultReceiver::WriteToParcel(
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

void CResultReceiver::OnReceiveResult(
    /* [in] */ Int32 resultCode,
    /* [in] */ IBundle* resultData)
{}

