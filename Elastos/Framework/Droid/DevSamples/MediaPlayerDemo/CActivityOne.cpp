
#include "CActivityOne.h"
#include "Uri.h"
#include <elastos/AutoPtr.h>
#include <stdio.h>

CActivityOne::PlayListener::PlayListener(
    /* [in] */ CActivityOne* host)
    : mHost(host)
{}

PInterface CActivityOne::PlayListener::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IViewOnTouchListener*)this;
    }
    else if (riid == EIID_IViewOnTouchListener) {
        return (IViewOnTouchListener*)this;
    }
    else if (riid == EIID_IViewOnKeyListener) {
        return (IViewOnKeyListener*)this;
    }

    return NULL;
}

UInt32 CActivityOne::PlayListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CActivityOne::PlayListener::Release()
{
    return ElRefBase::Release();
}

ECode CActivityOne::PlayListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IViewOnTouchListener*)this) {
        *pIID = EIID_IRunnable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode CActivityOne::PlayListener::OnClick(
    /* [in] */ IView* v)
{
    ECode ec;
    ec = mHost->mMediaPlayer->SetDataSourceEx2(String("/data/data/com.elastos.runtime/elastos/MediaPlayerDemo/data/test.mp3"));
    assert(SUCCEEDED(ec));
    ec = mHost->mMediaPlayer->Prepare();
    assert(SUCCEEDED(ec));
    ec = mHost->mMediaPlayer->Start();
    assert(SUCCEEDED(ec));

    return NOERROR;
}


CActivityOne::PauseListener::PauseListener(
    /* [in] */ CActivityOne* host)
    : mHost(host)
{}

PInterface CActivityOne::PauseListener::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IViewOnTouchListener*)this;
    }
    else if (riid == EIID_IViewOnTouchListener) {
        return (IViewOnTouchListener*)this;
    }
    else if (riid == EIID_IViewOnKeyListener) {
        return (IViewOnKeyListener*)this;
    }

    return NULL;
}

UInt32 CActivityOne::PauseListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CActivityOne::PauseListener::Release()
{
    return ElRefBase::Release();
}

ECode CActivityOne::PauseListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IViewOnTouchListener*)this) {
        *pIID = EIID_IRunnable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode CActivityOne::PauseListener::OnClick(
    /* [in] */ IView* v)
{
    if(mHost->mMediaPlayer != NULL){
        mHost->mMediaPlayer->Pause();
    }
    return NOERROR;
}


CActivityOne::StopListener::StopListener(
    /* [in] */ CActivityOne* host)
    : mHost(host)
{}

PInterface CActivityOne::StopListener::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IViewOnTouchListener*)this;
    }
    else if (riid == EIID_IViewOnTouchListener) {
        return (IViewOnTouchListener*)this;
    }
    else if (riid == EIID_IViewOnKeyListener) {
        return (IViewOnKeyListener*)this;
    }

    return NULL;
}

UInt32 CActivityOne::StopListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CActivityOne::StopListener::Release()
{
    return ElRefBase::Release();
}

ECode CActivityOne::StopListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IViewOnTouchListener*)this) {
        *pIID = EIID_IRunnable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode CActivityOne::StopListener::OnClick(
    /* [in] */ IView* v)
{
    if(mHost->mMediaPlayer != NULL){
        mHost->mMediaPlayer->Stop();
    }
    return NOERROR;
}


ECode CActivityOne::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    SetContentView(0x7f030000);

    assert(CMediaPlayer::New((IMediaPlayer**)&mMediaPlayer) == NOERROR);

    AutoPtr<IView> view;
    view = FindViewById(0x7f050000);
    assert(view != NULL);
    view->SetOnClickListener(new PlayListener(this));
    mPlayButton = IButton::Probe(view);
    assert(mPlayButton != NULL);
    view = FindViewById(0x7f050001);
    view->SetOnClickListener(new PauseListener(this));
    mPauseButton = IButton::Probe(view);
    assert(mPauseButton != NULL);
    view = FindViewById(0x7f050002);
    view->SetOnClickListener(new StopListener(this));
    mStopButton = IButton::Probe(view);
    assert(mStopButton != NULL);

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
