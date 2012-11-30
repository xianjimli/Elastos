
#include "os/CResultReceiver.h"
#include "ext/frameworkdef.h"


CResultReceiver::MyRunnable::MyRunnable(
    /* [in] */ Int32 resultCode,
    /* [in] */ IBundle* resultData,
    /* [in] */ CResultReceiver* host):
    mResultCode(resultCode),
    mResultData(resultData),
    mHost(host)
{

}

ECode CResultReceiver::MyRunnable::Run()
{
	assert(mHost != NULL);
    return mHost->OnReceiveResult(mResultCode, mResultData);
}

PInterface CResultReceiver::MyRunnable::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IRunnable == riid) {
        return (IRunnable *)this;
    }

    return NULL;
}

UInt32 CResultReceiver::MyRunnable::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CResultReceiver::MyRunnable::Release()
{
    return ElRefBase::Release();
}

ECode CResultReceiver::MyRunnable::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IRunnable*)this) {
        *pIID = EIID_IRunnable;
    }

    return NOERROR;
}

CResultReceiver::MyResultReceiver::MyResultReceiver(
    /* [in] */ CResultReceiver* host):
    mHost(host)
{

}

CResultReceiver::MyResultReceiver::~MyResultReceiver()
{
    if (mMyRunnable != NULL) {
        mMyRunnable->Release();
    }
}

ECode CResultReceiver::MyResultReceiver::Send(
    /* [in] */ Int32 resultCode,
    /* [in] */ IBundle* resultData)
{
    if (mHost->mHandler != NULL) {
        //mHandler.post(new MyRunnable(resultCode, resultData, mHost));
        mMyRunnable = new MyRunnable(resultCode, resultData, mHost);
        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInterfacePtr(mMyRunnable->Probe(EIID_IRunnable));

        //TODO
        // mHandler->PostCppCallbackDelayed(
        //     (Handle32)mMyRunnable.Get(), 0, params, 0);
    } else {
        return mHost->OnReceiveResult(resultCode, resultData);
    }

    return NOERROR;
}

PInterface CResultReceiver::MyResultReceiver::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IResultReceiverStub == riid) {
        return (IResultReceiverStub *)this;
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

    if (pObject == (IInterface*)(IResultReceiverStub*)this) {
        *pIID = EIID_IResultReceiverStub;
    }

    return NOERROR;
}

CResultReceiver::CResultReceiver()
{

}

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
        } else {
            return OnReceiveResult(resultCode, resultData);
        }
    }

    if (mReceiver != NULL) {
        return mReceiver->Send(resultCode, resultData);
    }

    return NOERROR;
}

ECode CResultReceiver::DescribeContents(
    /* [out] */ Int32* contents)
{
	*contents = 0;
    return NOERROR;
}

ECode CResultReceiver::ReadFromParcel(
    /* [in] */ IParcel *dest)
{
    return NOERROR;
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

ECode CResultReceiver::OnReceiveResult(
    /* [in] */ Int32 resultCode,
    /* [in] */ IBundle* resultData)
{
	return NOERROR;
}

ECode CResultReceiver::constructor(
    /* [in] */ IParcel* in)
{
    mLocal = FALSE;
    mHandler = NULL;

	//TODO
    //mReceiver = IResultReceiver.Stub.asInterface(in.readStrongBinder());

    return E_NOT_IMPLEMENTED;
}
