
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
    return NOERROR;
}

ECode CActivityOne::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    SetContentView(0x7f030000);

    AutoPtr<IView> view;
    view = FindViewById(0x7f050000);
    assert(view != NULL);
    mVideoView = IVideoView::Probe(view);
    /***
     * 将播放器关联上一个音频或者视频文件
     * videoView.setVideoURI(Uri uri)
     * videoView.setVideoPath(String path)
     * 以上两个方法都可以。
     */
    mVideoView->SetVideoPath(String("data/data/com.elastos.runtime/elastos/VideoViewDemo/data/test.mp3"));
    /**
     * w为其提供一个控制器，控制其暂停、播放……等功能
     */
    AutoPtr<IMediaController> mc;
    CMediaController::New((IContext*)this->Probe(EIID_IContext), (IMediaController**)&mc);
    mVideoView->SetMediaController(mc);
    /**
     * 视频或者音频到结尾时触发的方法
     */
    // videoView.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
    //     @Override
    //     public void onCompletion(MediaPlayer mp) {
    //         Log.i("通知", "完成");
    //     }
    // });

    // videoView.setOnErrorListener(new MediaPlayer.OnErrorListener() {

    //     @Override
    //     public boolean onError(MediaPlayer mp, int what, int extra) {
    //         Log.i("通知", "播放中出现错误");
    //         return false;
    //     }
    // });
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
