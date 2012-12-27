
#include "CActivityOne.h"
#include "Uri.h"
#include <elastos/AutoPtr.h>
#include <stdio.h>

CActivityOne::CMySurfaceHolderCallback::CMySurfaceHolderCallback(
    /* [in] */ CActivityOne* host) :
    mHost(host)
{
}

PInterface CActivityOne::CMySurfaceHolderCallback::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(ISurfaceHolderCallback*)this;
    }
    else if (riid == EIID_ISurfaceHolderCallback) {
        return (ISurfaceHolderCallback*)this;
    }

    return NULL;
}

UInt32 CActivityOne::CMySurfaceHolderCallback::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CActivityOne::CMySurfaceHolderCallback::Release()
{
    return ElRefBase::Release();
}

ECode CActivityOne::CMySurfaceHolderCallback::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(ISurfaceHolderCallback*)this) {
        *pIID = EIID_ISurfaceHolderCallback;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}


ECode CActivityOne::CMySurfaceHolderCallback::SurfaceCreated(
    /* [in] */ ISurfaceHolder* holder)
{
    printf("File=[%s], Line = [%d], FUNC = [%s]\n", __FILE__, __LINE__, __FUNCTION__);
    assert(0);
    return NOERROR;
}

ECode CActivityOne::CMySurfaceHolderCallback::SurfaceChanged(
    /* [in] */ ISurfaceHolder* holder,
    /* [in] */ Int32 format,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    printf("File=[%s], Line = [%d], FUNC = [%s]\n", __FILE__, __LINE__, __FUNCTION__);
    assert(0);
    return NOERROR;
}

ECode CActivityOne::CMySurfaceHolderCallback::SurfaceDestroyed(
    /* [in] */ ISurfaceHolder* holder)
{
    printf("File=[%s], Line = [%d], FUNC = [%s]\n", __FILE__, __LINE__, __FUNCTION__);
    assert(0);
    return NOERROR;
}

ECode CActivityOne::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    //SetContentView(0x7f030000);
    ECode ec = CSurfaceView::New((IContext*)this->Probe(EIID_IContext),
            (ISurfaceView**) &mSurfaceView);
    printf("File=[%s], Line = [%d], FUNC = [%s], ec=[0x%08x]\n", __FILE__, __LINE__, __FUNCTION__, ec);
    assert(mSurfaceView != NULL);

    mSurfaceView->GetHolder((ISurfaceHolder**) &mSurfaceHolder);
    assert(mSurfaceHolder != NULL);

    mSurfaceHolderCallback = new CMySurfaceHolderCallback(this);
    assert(mSurfaceHolderCallback != NULL);
    mSurfaceHolder->AddCallback(mSurfaceHolderCallback);

    SetContentView(mSurfaceView);

    return NOERROR;
}

ECode CActivityOne::OnStart()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityOne::OnResume()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityOne::OnPause()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityOne::OnStop()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityOne::OnDestroy()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityOne::OnActivityResult(
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent *data)
{
    return NOERROR;
}
