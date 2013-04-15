
#include "ext/frameworkdef.h"
#include "widget/MediaController.h"
#include "widget/CFrameLayoutLayoutParams.h"
#include "view/CWindowManagerLayoutParams.h"
#include "graphics/ElPixelFormat.h"
#include "impl/CPolicyManager.h"
#include "os/CApartment.h"

// using namespace Elastos::Core;


PInterface MediaController::MCTouchListener::Probe(
            /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IViewOnTouchListener) {
        return (IViewOnTouchListener*)this;
    }

    return NULL;
}

UInt32 MediaController::MCTouchListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 MediaController::MCTouchListener::Release()
{
    return ElRefBase::Release();
}

ECode MediaController::MCTouchListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IViewOnTouchListener*)this) {
        *pIID = EIID_IViewOnTouchListener;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode MediaController::MCTouchListener::OnTouch(
    /* [in] */ IView* v,
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);

    Int32 action;
    event->GetAction(&action);
    if (action == MotionEvent_ACTION_DOWN) {
        if (mHost->mShowing) {
            mHost->Hide();
        }
    }
    *res = FALSE;

    return NOERROR;
}


PInterface MediaController::MCClickListener::Probe(
            /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IViewOnClickListener) {
        return (IViewOnTouchListener*)this;
    }

    return NULL;
}

UInt32 MediaController::MCClickListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 MediaController::MCClickListener::Release()
{
    return ElRefBase::Release();
}

ECode MediaController::MCClickListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IViewOnClickListener*)this) {
        *pIID = EIID_IViewOnClickListener;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}


ECode MediaController::PauseListener::OnClick(
    /* [in] */ IView* v)
{
    mHost->DoPauseResume();
    mHost->Show(MediaController::sDefaultTimeout);

    return NOERROR;
}


ECode MediaController::RewListener::OnClick(
    /* [in] */ IView* v)
{
    Int32 pos;
    mHost->mPlayer->GetCurrentPosition(&pos);
    pos -= 5000; // milliseconds
    mHost->mPlayer->SeekTo(pos);
    mHost->SetProgress();

    mHost->Show(MediaController::sDefaultTimeout);

    return NOERROR;
}

ECode MediaController::FfwdListener::OnClick(
    /* [in] */ IView* v)
{
    Int32 pos;
    mHost->mPlayer->GetCurrentPosition(&pos);
    pos += 15000; // milliseconds
    mHost->mPlayer->SeekTo(pos);
    mHost->SetProgress();

    mHost->Show(MediaController::sDefaultTimeout);

    return NOERROR;
}


PInterface MediaController::MCOnSeekBarChangeListener::Probe(
            /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IOnSeekBarChangeListener) {
        return (IOnSeekBarChangeListener*)this;
    }

    return NULL;
}

UInt32 MediaController::MCOnSeekBarChangeListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 MediaController::MCOnSeekBarChangeListener::Release()
{
    return ElRefBase::Release();
}

ECode MediaController::MCOnSeekBarChangeListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IOnSeekBarChangeListener*)this) {
        *pIID = EIID_IOnSeekBarChangeListener;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode MediaController::MCOnSeekBarChangeListener::OnStartTrackingTouch(
    /* [in] */ ISeekBar* seekBar)
{
    mHost->Show(3600000);

    mHost->mDragging = TRUE;

    // By removing these pending progress messages we make sure
    // that a) we won't update the progress while the user adjusts
    // the seekbar and b) once the user is done dragging the thumb
    // we will post one of these messages to the queue again and
    // this ensures that there will be exactly one message queued up.
    //mHandler.removeMessages(SHOW_PROGRESS);
    ECode (STDCALL MediaController::*pHandlerFunc)();
    pHandlerFunc = &MediaController::HandleShowProgress;
    mHost->mApartment->RemoveCppCallbacks(
                (Handle32)mHost, *(Handle32*)&pHandlerFunc);

    return NOERROR;
}

ECode MediaController::MCOnSeekBarChangeListener::OnProgressChanged(
    /* [in] */ ISeekBar* seekBar,
    /* [in] */ Int32 progress,
    /* [in] */ Boolean fromUser)
{
    if (!fromUser) {
        // We're not interested in programmatically generated changes to
        // the progress bar's position.
        return NOERROR;
    }

    Int32 duration;
    mHost->mPlayer->GetDuration(&duration);
    Int32 newposition = (duration * progress) / 1000;
    mHost->mPlayer->SeekTo(newposition);
    if (mHost->mCurrentTime != NULL) {
        AutoPtr<ICharSequence> charS;
        ASSERT_SUCCEEDED(CStringWrapper::New(
                mHost->StringForTime(newposition), (ICharSequence**)&charS));
        mHost->mCurrentTime->SetText(charS);
    }

    return NOERROR;
}

ECode MediaController::MCOnSeekBarChangeListener::OnStopTrackingTouch(
    /* [in] */ ISeekBar* seekBar)
{
    mHost->mDragging = FALSE;
    mHost->SetProgress();
    mHost->UpdatePausePlay();
    mHost->Show(sDefaultTimeout);

    // Ensure that progress is properly updated in the future,
    // the call to show() does not guarantee this because it is a
    // no-op if we are already showing.
    //mHandler.sendEmptyMessage(SHOW_PROGRESS);
    ECode (STDCALL MediaController::*pHandlerShowProgress)();
    pHandlerShowProgress = &MediaController::HandleShowProgress;
    mHost->mApartment->PostCppCallback(
            (Handle32)mHost, *(Handle32*)&pHandlerShowProgress, NULL, 0);

    return NOERROR;
}


const Int32 MediaController::sDefaultTimeout;
const Int32 MediaController::FADE_OUT;
const Int32 MediaController::SHOW_PROGRESS;

MediaController::MediaController()
    : mShowing(FALSE)
    , mDragging(FALSE)
    , mUseFastForward(TRUE)
    , mFromXml(TRUE)
    , mListenersSet(FALSE)
{
    mTouchListener = new MCTouchListener(this);
    mPauseListener = new PauseListener(this);
    mRewListener = new RewListener(this);
    mFfwdListener = new FfwdListener(this);
    mSeekListener = new MCOnSeekBarChangeListener(this);
    //todo: CApartment::GetMyApartment((IApartment**)&mApartment);
    CApartment::GetMainApartment((IApartment**)&mApartment);
    assert(mApartment != NULL);
}

MediaController::~MediaController()
{}

ECode MediaController::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(FrameLayout::Init(context, attrs));
    mRoot = (IView*)this->Probe(EIID_IView);
    mContext = context;
    mUseFastForward = TRUE;
    mFromXml = TRUE;
    return NOERROR;
}

ECode MediaController::Init(
    /* [in] */ IContext* context,
    /* [in] */ Boolean useFastForward)
{
    FAIL_RETURN(FrameLayout::Init(context));
    mContext = context;
    mUseFastForward = useFastForward;
    return InitFloatingWindow();
}

ECode MediaController::OnFinishInflate()
{
    if (mRoot != NULL) {
        return InitControllerView(mRoot);
    }

    return NOERROR;
}

ECode MediaController::InitFloatingWindow()
{
    FAIL_RETURN(mContext->GetSystemService(Context_WINDOW_SERVICE, (IInterface**)&mWindowManager));
    AutoPtr<IPolicyManager> policy;
    FAIL_RETURN(CPolicyManager::AcquireSingleton((IPolicyManager**)&policy));
    FAIL_RETURN(policy->MakeNewWindow(mContext, (IWindow**)&mWindow));
    mWindow->SetWindowManager(mWindowManager, NULL, String(NULL));
    Boolean result;
    mWindow->RequestFeature(Window_FEATURE_NO_TITLE, &result);
    FAIL_RETURN(mWindow->GetDecorView((IView**)&mDecor));
    mDecor->SetOnTouchListener(mTouchListener);
    mWindow->SetContentViewEx((IView*)this->Probe(EIID_IView));
    FAIL_RETURN(mWindow->SetBackgroundDrawableResource(0x0106000d)); //android.R.color.transparent

    // While the media controller is up, the volume control keys should
    // affect the media stream type
    mWindow->SetVolumeControlStream(AudioManager_STREAM_MUSIC);

    SetFocusable(TRUE);
    SetFocusableInTouchMode(TRUE);
    SetDescendantFocusability(ViewGroup_FOCUS_AFTER_DESCENDANTS);
    View::RequestFocus();

    return NOERROR;
}

ECode MediaController::SetMediaPlayer(
    /* [in] */ IMediaPlayerControl* player)
{
    mPlayer = player;
    UpdatePausePlay();

    return NOERROR;
}

ECode MediaController::SetAnchorView(
    /* [in] */ IView* view)
{
    mAnchor = view;

    AutoPtr<IFrameLayoutLayoutParams> frameParams;
    FAIL_RETURN(CFrameLayoutLayoutParams::New(
            ViewGroupLayoutParams_MATCH_PARENT,
            ViewGroupLayoutParams_MATCH_PARENT,
            (IFrameLayoutLayoutParams**)&frameParams));

    RemoveAllViews();
    AutoPtr<IView> v = MakeControllerView();
    return AddView(v, frameParams);
}

AutoPtr<IView> MediaController::MakeControllerView()
{
    AutoPtr<ILayoutInflater> inflate;
    ASSERT_SUCCEEDED(mContext->GetSystemService(
            Context_LAYOUT_INFLATER_SERVICE, (IInterface**)&inflate));
    ASSERT_SUCCEEDED(inflate->Inflate(0x01090041, NULL, (IView**)&mRoot)); //com.android.internal.R.layout.media_controller

    ASSERT_SUCCEEDED(InitControllerView(mRoot));

    return mRoot;
}

ECode MediaController::InitControllerView(
    /* [in] */ IView* view)
{
    FAIL_RETURN(view->FindViewById(0x010201f3, (IView**)&mPauseButton)); //com.android.internal.R.id.pause
    if (mPauseButton != NULL) {
        Boolean result;
        mPauseButton->RequestFocus(&result);
        mPauseButton->SetOnClickListener(mPauseListener);
    }

    FAIL_RETURN(view->FindViewById(0x010201f4, (IView**)&mFfwdButton)); //com.android.internal.R.id.ffwd
    if (mFfwdButton != NULL) {
        mFfwdButton->SetOnClickListener(mFfwdListener);
        if (!mFromXml) {
            mFfwdButton->SetVisibility(mUseFastForward ? View_VISIBLE : View_GONE);
        }
    }

    FAIL_RETURN(view->FindViewById(0x010201f2, (IView**)&mRewButton)); //com.android.internal.R.id.rew
    if (mRewButton != NULL) {
        mRewButton->SetOnClickListener(mRewListener);
        if (!mFromXml) {
            mRewButton->SetVisibility(mUseFastForward ? View_VISIBLE : View_GONE);
        }
    }

    // By default these are hidden. They will be enabled when setPrevNextListeners() is called
    FAIL_RETURN(view->FindViewById(0x010201f5, (IView**)&mNextButton)); //com.android.internal.R.id.next
    if (mNextButton != NULL && !mFromXml && !mListenersSet) {
        mNextButton->SetVisibility(View_GONE);
    }
    FAIL_RETURN(view->FindViewById(0x010201f1, (IView**)&mPrevButton)); //com.android.internal.R.id.prev
    if (mPrevButton != NULL && !mFromXml && !mListenersSet) {
        mPrevButton->SetVisibility(View_GONE);
    }

    FAIL_RETURN(view->FindViewById(0x010201f7, (IView**)&mProgress)); //com.android.internal.R.id.mediacontroller_progress
    if (mProgress != NULL) {
        ISeekBar* pSeekBar = (ISeekBar*)mProgress->Probe(EIID_ISeekBar);
        if (pSeekBar) {
            pSeekBar->SetOnSeekBarChangeListener(mSeekListener);
        }
        mProgress->SetMax(1000);
    }

    FAIL_RETURN(view->FindViewById(0x0102005b, (IView**)&mEndTime)); //com.android.internal.R.id.time
    FAIL_RETURN(view->FindViewById(0x010201f6, (IView**)&mCurrentTime)); //com.android.internal.R.id.time_current
    //mFormatBuilder = new StringBuilder();
    //mFormatter = new Formatter(mFormatBuilder, Locale.getDefault());

    InstallPrevNextListeners();

    return NOERROR;
}

ECode MediaController::Show()
{
    return Show(sDefaultTimeout);
}

void MediaController::DisableUnsupportedButtons()
{
    //try {
    Boolean canPause;
    if (mPauseButton != NULL && (mPlayer->CanPause(&canPause), !canPause)) {
        mPauseButton->SetEnabled(FALSE);
    }

    Boolean canSeekBackward;
    if (mRewButton != NULL && (mPlayer->CanSeekBackward(&canSeekBackward), !canSeekBackward)) {
        mRewButton->SetEnabled(FALSE);
    }

    Boolean canSeekForward;
    if (mFfwdButton != NULL && (mPlayer->CanSeekForward(&canSeekForward), !canSeekForward)) {
        mFfwdButton->SetEnabled(FALSE);
    }
    //} catch (IncompatibleClassChangeError ex) {
        // We were given an old version of the interface, that doesn't have
        // the canPause/canSeekXYZ methods. This is OK, it just means we
        // assume the media can be paused and seeked, and so we don't disable
        // the buttons.
    //}
}

ECode MediaController::Show(
    /* [in] */ Int32 timeout)
{
    if (!mShowing && mAnchor != NULL) {
        SetProgress();
        if (mPauseButton != NULL) {
            Boolean result;
            mPauseButton->RequestFocus(&result);
        }
        DisableUnsupportedButtons();

        Int32 x, y;
        FAIL_RETURN(mAnchor->GetLocationOnScreen(&x, &y));

        AutoPtr<IWindowManagerLayoutParams> p;
        FAIL_RETURN(CWindowManagerLayoutParams::New((IWindowManagerLayoutParams**)&p));
        p->SetGravity(Gravity_TOP);
        Int32 width, height;
        mAnchor->GetWidth(&width);
        p->SetWidth(width);
        p->SetHeight(ViewGroupLayoutParams_WRAP_CONTENT);
        p->SetX(0);
        mAnchor->GetHeight(&height);
        p->SetY(y + height - ViewGroupLayoutParams_WRAP_CONTENT);
        p->SetFormat(ElPixelFormat::TRANSLUCENT);
        p->SetType(WindowManagerLayoutParams_TYPE_APPLICATION_PANEL);
        Int32 flags;
        p->GetFlags(&flags);
        p->SetFlags(flags | WindowManagerLayoutParams_FLAG_ALT_FOCUSABLE_IM);
        p->SetToken(NULL);
        p->SetWindowAnimations(0); // android.R.style.DropDownAnimationDown;
        FAIL_RETURN(mWindowManager->AddViewEx5(mDecor, p));
        mShowing = TRUE;
    }
    UpdatePausePlay();

    // cause the progress bar to be updated even if mShowing
    // was already true.  This happens, for example, if we're
    // paused with the progress bar showing the user hits play.
//    mHandler.sendEmptyMessage(SHOW_PROGRESS);

//    Message msg = mHandler.obtainMessage(FADE_OUT);
//    if (timeout != 0) {
//        mHandler.removeMessages(FADE_OUT);
//        mHandler.sendMessageDelayed(msg, timeout);
//    }
    ECode (STDCALL MediaController::*pHandlerShowProgress)();
    pHandlerShowProgress = &MediaController::HandleShowProgress;
    mApartment->PostCppCallback(
            (Handle32)this, *(Handle32*)&pHandlerShowProgress, NULL, 0);

    if (timeout != 0) {
        ECode (STDCALL MediaController::*pHandlerFadeOut)();
        pHandlerFadeOut = &MediaController::HandleFadeOut;

        mApartment->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFadeOut);
        mApartment->PostCppCallbackDelayed(
            (Handle32)this, *(Handle32*)&pHandlerFadeOut,
            NULL, 0, timeout);
    }

    return NOERROR;
}

Boolean MediaController::IsShowing()
{
    return mShowing;
}

ECode MediaController::Hide()
{
    if (mAnchor == NULL)
        return NOERROR;

    if (mShowing) {
        //try {
            //mHandler.removeMessages(SHOW_PROGRESS);
        ECode (STDCALL MediaController::*pHandlerFunc)();
        pHandlerFunc = &MediaController::HandleShowProgress;
        mApartment->RemoveCppCallbacks(
                    (Handle32)this, *(Handle32*)&pHandlerFunc);
        mWindowManager->RemoveView(mDecor);
        //} catch (IllegalArgumentException ex) {
        //    Log.w("MediaController", "already removed");
        //}
        mShowing = FALSE;
    }

    return NOERROR;
}

String MediaController::StringForTime(
    /* [in] */ Int32 timeMs)
{
    Int32 totalSeconds = timeMs / 1000;

    Int32 seconds = totalSeconds % 60;
    Int32 minutes = (totalSeconds / 60) % 60;
    Int32 hours   = totalSeconds / 3600;

    /*mFormatBuilder.setLength(0);
    if (hours > 0) {
        return mFormatter.format("%d:%02d:%02d", hours, minutes, seconds).toString();
    } else {
        return mFormatter.format("%02d:%02d", minutes, seconds).toString();
    }*/

    String result;
    if (hours > 0) {
        result.Append("%d", hours);
        result += ":";
        result.Append("%2d", minutes);
        result += ":";
        result.Append("%2d", seconds);
    }
    else {
        result.Append("%2d", minutes);
        result += ":";
        result.Append("%2d", seconds);
    }

    return result;
}

Int32 MediaController::SetProgress()
{
    if (mPlayer == NULL || mDragging) {
        return 0;
    }
    Int32 position, duration;
    mPlayer->GetCurrentPosition(&position);
    mPlayer->GetDuration(&duration);
    if (mProgress != NULL) {
        if (duration > 0) {
            // use long to avoid overflow
            Int32 pos = 1000L * position / duration;
            mProgress->SetProgress(pos);
        }
        Int32 percent;
        mPlayer->GetBufferPercentage(&percent);
        mProgress->SetSecondaryProgress(percent * 10);
    }

    if (mEndTime != NULL) {
        AutoPtr<ICharSequence> charS;
        ASSERT_SUCCEEDED(CStringWrapper::New(
                StringForTime(duration), (ICharSequence**)&charS));
        mEndTime->SetText(charS);
    }
    if (mCurrentTime != NULL) {
        AutoPtr<ICharSequence> charS;
        ASSERT_SUCCEEDED(CStringWrapper::New(
                StringForTime(position), (ICharSequence**)&charS));
        mCurrentTime->SetText(charS);
    }

    return position;
}

Boolean MediaController::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    Show(sDefaultTimeout);
    return TRUE;
}

Boolean MediaController::OnTrackballEvent(
    /* [in] */ IMotionEvent* event)
{
    Show(sDefaultTimeout);
    return FALSE;
}

Boolean MediaController::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    Int32 keyCode, repeatCount;
    Boolean isDown;
    event->GetKeyCode(&keyCode);
    event->GetRepeatCount(&repeatCount);
    event->IsDown(&isDown);
    if (repeatCount == 0 && isDown && (
            keyCode ==  KeyEvent_KEYCODE_HEADSETHOOK ||
            keyCode ==  KeyEvent_KEYCODE_MEDIA_PLAY_PAUSE ||
            keyCode ==  KeyEvent_KEYCODE_SPACE)) {
        DoPauseResume();
        Show(sDefaultTimeout);
        if (mPauseButton != NULL) {
            Boolean result;
            mPauseButton->RequestFocus(&result);
        }
        return TRUE;
    }
    else if (keyCode ==  KeyEvent_KEYCODE_MEDIA_STOP) {
        Boolean isPlaying;
        mPlayer->IsPlaying(&isPlaying);
        if (isPlaying) {
            mPlayer->Pause();
            UpdatePausePlay();
        }
        return TRUE;
    }
    else if (keyCode == KeyEvent_KEYCODE_VOLUME_DOWN ||
            keyCode == KeyEvent_KEYCODE_VOLUME_UP) {
        // don't show the controls for volume adjustment
        return FrameLayout::DispatchKeyEvent(event);
    }
    else if (keyCode == KeyEvent_KEYCODE_BACK ||
             keyCode == KeyEvent_KEYCODE_MENU) {
        Hide();

        return TRUE;
    }
    else {
        Show(sDefaultTimeout);
    }

    return FrameLayout::DispatchKeyEvent(event);
}

Void MediaController::UpdatePausePlay()
{
    if (mRoot == NULL || mPauseButton == NULL)
        return;

    Boolean isPlaying;
    mPlayer->IsPlaying(&isPlaying);
    if (isPlaying) {
        mPauseButton->SetImageResource(0x01080023); //com.android.internal.R.drawable.ic_media_pause
    }
    else {
        mPauseButton->SetImageResource(0x01080024); //com.android.internal.R.drawable.ic_media_play
    }
}

Void MediaController::DoPauseResume()
{
    Boolean isPlaying;
    mPlayer->IsPlaying(&isPlaying);
    if (isPlaying) {
        mPlayer->Pause();
    }
    else {
        mPlayer->Start();
    }
    UpdatePausePlay();
}

ECode MediaController::SetEnabled(
    /* [in] */ Boolean enabled)
{
    if (mPauseButton != NULL) {
        mPauseButton->SetEnabled(enabled);
    }
    if (mFfwdButton != NULL) {
        mFfwdButton->SetEnabled(enabled);
    }
    if (mRewButton != NULL) {
        mRewButton->SetEnabled(enabled);
    }
    if (mNextButton != NULL) {
        mNextButton->SetEnabled(enabled && mNextListener != NULL);
    }
    if (mPrevButton != NULL) {
        mPrevButton->SetEnabled(enabled && mPrevListener != NULL);
    }
    if (mProgress != NULL) {
        mProgress->SetEnabled(enabled);
    }
    DisableUnsupportedButtons();
    return FrameLayout::SetEnabled(enabled);
}

void MediaController::InstallPrevNextListeners()
{
    if (mNextButton != NULL) {
        mNextButton->SetOnClickListener(mNextListener);
        mNextButton->SetEnabled(mNextListener != NULL);
    }

    if (mPrevButton != NULL) {
        mPrevButton->SetOnClickListener(mPrevListener);
        mPrevButton->SetEnabled(mPrevListener != NULL);
    }
}

ECode MediaController::SetPrevNextListeners(
    /* [in] */ IViewOnClickListener* next,
    /* [in] */ IViewOnClickListener* prev)
{
    mNextListener = next;
    mPrevListener = prev;
    mListenersSet = TRUE;

    if (mRoot != NULL) {
        InstallPrevNextListeners();

        if (mNextButton != NULL && !mFromXml) {
            mNextButton->SetVisibility(View_VISIBLE);
        }
        if (mPrevButton != NULL && !mFromXml) {
            mPrevButton->SetVisibility(View_VISIBLE);
        }
    }

    return NOERROR;
}

ECode MediaController::HandleFadeOut()
{
    Hide();
    return NOERROR;
}

ECode MediaController::HandleShowProgress()
{
//    pos = setProgress();
//    if (!mDragging && mShowing && mPlayer.isPlaying()) {
//        msg = obtainMessage(SHOW_PROGRESS);
//        sendMessageDelayed(msg, 1000 - (pos % 1000));
//    }

    Int32 pos = SetProgress();
    Boolean isPlaying;
    if (!mDragging && mShowing && (mPlayer->IsPlaying(&isPlaying), isPlaying)) {
        ECode (STDCALL MediaController::*pHandlerFunc)();
        pHandlerFunc = &MediaController::HandleShowProgress;

        mApartment->PostCppCallbackDelayed(
            (Handle32)this, *(Handle32*)&pHandlerFunc,
            NULL, 0, 1000 - (pos % 1000));
    }
    return NOERROR;
}
