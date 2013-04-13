
#ifndef __MediaController_h__
#define __MediaController_h__


#include "FrameLayout.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>


class MediaController : public FrameLayout
{
private:
    class MCTouchListener
        :  public ElRefBase
        ,  public IViewOnTouchListener
    {
    public:
        MCTouchListener(
            /* [in] */ MediaController* host)
            : mHost(host)
        {}

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnTouch(
            /* [in] */ IView* v,
            /* [in] */ IMotionEvent* event,
            /* [out] */ Boolean* res);

    private:
        MediaController* mHost;
    };

    class MCClickListener
        : public ElRefBase
        , public IViewOnClickListener
    {
    public:
        MCClickListener(
            /* [in] */ MediaController* host)
            : mHost(host)
        {}

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

    protected:
        MediaController* mHost;
    };

    class PauseListener : public MCClickListener
    {
    public:
        PauseListener(
            /* [in] */ MediaController* host)
            : MCClickListener(host)
        {}

        CARAPI OnClick(
            /* [in] */ IView* v);
    };

    class RewListener : public MCClickListener
    {
    public:
        RewListener(
            /* [in] */ MediaController* host)
            : MCClickListener(host)
        {}

        CARAPI OnClick(
            /* [in] */ IView* v);
    };

    class FfwdListener : public MCClickListener
    {
    public:
        FfwdListener(
            /* [in] */ MediaController* host)
            : MCClickListener(host)
        {}

        CARAPI OnClick(
            /* [in] */ IView* v);
    };

    class MCOnSeekBarChangeListener
        : public ElRefBase
        , public IOnSeekBarChangeListener
    {
    public:
        MCOnSeekBarChangeListener(
            /* [in] */ MediaController* host)
            : mHost(host)
        {}

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnProgressChanged(
            /* [in] */ ISeekBar* seekBar,
            /* [in] */ Int32 progress,
            /* [in] */ Boolean fromUser);

        CARAPI OnStartTrackingTouch(
            /* [in] */ ISeekBar* seekBar);

        CARAPI OnStopTrackingTouch(
            /* [in] */ ISeekBar* seekBar);

    private:
        MediaController* mHost;
    };

public:
    MediaController();

    virtual ~MediaController();

    //@Override
    CARAPI OnFinishInflate();

    virtual CARAPI SetMediaPlayer(
        /* [in] */ IMediaPlayerControl* player);

    /**
     * Set the view that acts as the anchor for the control view.
     * This can for example be a VideoView, or your Activity's main view.
     * @param view The view to which to anchor the controller when it is visible.
     */
    virtual CARAPI SetAnchorView(
        /* [in] */ IView* view);

    /**
     * Show the controller on screen. It will go away
     * automatically after 3 seconds of inactivity.
     */
    virtual CARAPI Show();

    /**
     * Show the controller on screen. It will go away
     * automatically after 'timeout' milliseconds of inactivity.
     * @param timeout The timeout in milliseconds. Use 0 to show
     * the controller until hide() is called.
     */
    virtual CARAPI Show(
        /* [in] */ Int32 timeout);

    virtual CARAPI_(Boolean) IsShowing();

    /**
     * Remove the controller from the screen.
     */
    virtual CARAPI Hide();

    //@Override
    CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

    //@Override
    CARAPI_(Boolean) OnTrackballEvent(
        /* [in] */ IMotionEvent* event);

    //@Override
    CARAPI_(Boolean) DispatchKeyEvent(
        /* [in] */ IKeyEvent* event);

    //@Override
    CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    virtual CARAPI SetPrevNextListeners(
        /* [in] */ IViewOnClickListener* next,
        /* [in] */ IViewOnClickListener* prev);

protected:
    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ Boolean useFastForward = TRUE);

    virtual CARAPI_(AutoPtr<IView>) MakeControllerView();

private:
    CARAPI InitFloatingWindow();

    CARAPI InitControllerView(
        /* [in] */ IView* view);

    CARAPI_(void) DisableUnsupportedButtons();

    CARAPI_(String) StringForTime(
        /* [in] */ Int32 timeMs);

    CARAPI_(Int32) SetProgress();

    CARAPI_(void) UpdatePausePlay();

    CARAPI_(void) DoPauseResume();

    CARAPI_(void) InstallPrevNextListeners();

    CARAPI HandleFadeOut();

    CARAPI HandleShowProgress();

private:
    AutoPtr<IMediaPlayerControl> mPlayer;
    AutoPtr<IContext> mContext;
    AutoPtr<IView> mAnchor;
    AutoPtr<IView> mRoot;
    AutoPtr<ILocalWindowManager> mWindowManager;
    AutoPtr<IWindow> mWindow;
    AutoPtr<IView> mDecor;
    AutoPtr<IProgressBar> mProgress;
    AutoPtr<ITextView> mEndTime;
    AutoPtr<ITextView> mCurrentTime;
    Boolean mShowing;
    Boolean mDragging;

    static const Int32 sDefaultTimeout = 3000;
    static const Int32 FADE_OUT = 1;
    static const Int32 SHOW_PROGRESS = 2;
    Boolean mUseFastForward;
    Boolean mFromXml;
    Boolean mListenersSet;
    AutoPtr<IViewOnClickListener> mNextListener;
    AutoPtr<IViewOnClickListener> mPrevListener;
    //StringBuilder               mFormatBuilder;
    //Formatter                   mFormatter;
    AutoPtr<IImageButton> mPauseButton;
    AutoPtr<IImageButton> mFfwdButton;
    AutoPtr<IImageButton> mRewButton;
    AutoPtr<IImageButton> mNextButton;
    AutoPtr<IImageButton> mPrevButton;

    AutoPtr<MCTouchListener> mTouchListener;
    AutoPtr<PauseListener> mPauseListener;
    AutoPtr<RewListener> mRewListener;
    AutoPtr<FfwdListener> mFfwdListener;

    // There are two scenarios that can trigger the seekbar listener to trigger:
    //
    // The first is the user using the touchpad to adjust the posititon of the
    // seekbar's thumb. In this case onStartTrackingTouch is called followed by
    // a number of onProgressChanged notifications, concluded by onStopTrackingTouch.
    // We're setting the field "mDragging" to true for the duration of the dragging
    // session to avoid jumps in the position in case of ongoing playback.
    //
    // The second scenario involves the user operating the scroll ball, in this
    // case there WON'T BE onStartTrackingTouch/onStopTrackingTouch notifications,
    // we will simply apply the updated position without suspending regular updates.
    AutoPtr<MCOnSeekBarChangeListener> mSeekListener;

    AutoPtr<IApartment> mApartment;
};

#endif //__MediaController_h__
