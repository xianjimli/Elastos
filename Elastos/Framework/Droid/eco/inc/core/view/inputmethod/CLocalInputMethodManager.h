
#ifndef  _CINPUTMETHODMANAGER_H__
#define  _CINPUTMETHODMANAGER_H__

#include "ext/frameworkext.h"
#include "_CLocalInputMethodManager.h"
#include <elastos/AutoPtr.h>
#include <elastos/List.h>
#include "view/IInputConnectionWrapper.h"

CarClass(CLocalInputMethodManager)
{
private:
    class ControlledInputConnectionWrapper:
        public IInputConnectionWrapper
    {
    public:
        ControlledInputConnectionWrapper(
            /* [in] */ IApartment* mainLooper,
            /* [in] */ IInputConnection* conn,
            /* [in] */ CLocalInputMethodManager* host);

        CARAPI_(Boolean) IsActive();

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

    private:
        CLocalInputMethodManager* mHost;
    };

    class _InputMethodClientStub:
        public ElRefBase,
        public IInputMethodClientStub
    {
    public:
        _InputMethodClientStub(
            /* [in] */ CLocalInputMethodManager* host);

        ~_InputMethodClientStub();

        CARAPI SetUsingInputMethod(
            /* [in] */ Boolean state);

        CARAPI OnBindMethod(
            /* [in] */ IInputBindResult* res);

        CARAPI OnUnbindMethod(
            /* [in] */ Int32 sequence);

        CARAPI SetActive(
            /* [in] */ Boolean active);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI GetDescription(
            /* [out] */ String* str);

    private:
        CLocalInputMethodManager* mHost;
    };


public:
    CLocalInputMethodManager();

    //TODO: will delete. Because LocalInputMethodManager is private in java code.
    CARAPI constructor(
        /* [in] */ IInputMethodManagerStub* service,
        /* [in] */ IApartment* looper);

    /**
     * Retrieve the global InputMethodManager instance, creating it if it
     * doesn't already exist.
     * @hide
     */
    static CARAPI_(ILocalInputMethodManager*) GetInstance(
        /* [in] */ IContext* context);

    /**
     * Internally, the input method manager can't be context-dependent, so
     * we have this here for the places that need it.
     * @hide
     */
    static CARAPI_(ILocalInputMethodManager*) GetInstanceEx(
        /* [in] */ IApartment* mainLooper);

    /**
     * Private optimization: retrieve the global InputMethodManager instance,
     * if it exists.
     * @hide
     */
    static CARAPI_(ILocalInputMethodManager*) PeekInstance();

    /** @hide */
    CARAPI GetClient(
        /* [out] */ IInputMethodClientStub** client);

    /** @hide */
    CARAPI GetInputContext(
        /* [out] */ IInputContextStub** context);

    CARAPI GetInputMethodList(
        /* [out] */ IObjectContainer** infos);

    CARAPI GetEnabledInputMethodList(
        /* [out] */ IObjectContainer** list);

    CARAPI ShowStatusIcon(
        /* [in] */ IBinder* imeToken,
        /* [in] */ const String& packageName,
        /* [in] */ Int32 iconId);

    CARAPI HideStatusIcon(
        /* [in] */ IBinder* imeToken);

    /** @hide */
    CARAPI SetFullscreenMode(
        /* [in] */ Boolean fullScreen);

    /**
     * Allows you to discover whether the attached input method is running
     * in fullscreen mode.  Return true if it is fullscreen, entirely covering
     * your UI, else returns false.
     */
    CARAPI IsFullscreenMode(
        /* [out] */ Boolean* isfull);

    /**
     * Return true if the given view is the currently active view for the
     * input method.
     */
    CARAPI IsActive(
        /* [in] */ IView* view,
        /* [out] */ Boolean* active);

    /**
     * Return true if any view is currently active in the input method.
     */
    CARAPI IsActiveEx(
        /* [out] */ Boolean* active);

    /**
     * Return true if the currently served view is accepting full text edits.
     * If false, it has no input connection, so can only handle raw key events.
     */
    CARAPI IsAcceptingText(
        /* [out] */ Boolean* isAccepting);

    /**
     * Reset all of the state associated with being bound to an input method.
     */
    CARAPI ClearBindingLocked();

    /**
     * Called from the FINISH_INPUT_CONNECTION message above.
     * @hide
     */
    CARAPI ReportFinishInputConnection(
        /* [in] */ IInputConnection* ic);

    CARAPI DisplayCompletions(
        /* [in] */ IView* view,
        /* [in] */ ArrayOf<ICompletionInfo*>* completions);

    CARAPI UpdateExtractedText(
        /* [in] */ IView* view,
        /* [in] */ Int32 token,
        /* [in] */ IExtractedText* text);

    /**
     * Synonym for {@link #showSoftInput(View, Int32, ResultReceiver)} without
     * a result receiver: explicitly request that the current input method's
     * soft input area be shown to the user, if needed.
     *
     * @param view The currently focused view, which would like to receive
     * soft keyboard input.
     * @param flags Provides additional operating flags.  Currently may be
     * 0 or have the {@link #SHOW_IMPLICIT} bit set.
     */
    CARAPI ShowSoftInput(
        /* [in] */ IView* view,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* show);

    /**
     * Explicitly request that the current input method's soft input area be
     * shown to the user, if needed.  Call this if the user interacts with
     * your view in such a way that they have expressed they would like to
     * start performing input into it.
     *
     * @param view The currently focused view, which would like to receive
     * soft keyboard input.
     * @param flags Provides additional operating flags.  Currently may be
     * 0 or have the {@link #SHOW_IMPLICIT} bit set.
     * @param resultReceiver If non-NULL, this will be called by the IME when
     * it has processed your request to tell you what it has done.  The result
     * code you receive may be either {@link #RESULT_UNCHANGED_SHOWN},
     * {@link #RESULT_UNCHANGED_HIDDEN}, {@link #RESULT_SHOWN}, or
     * {@link #RESULT_HIDDEN}.
     */
    CARAPI ShowSoftInputEx(
        /* [in] */ IView* view,
        /* [in] */ Int32 flags,
        /* [in] */ IResultReceiver* resultReceiver,
        /* [out] */ Boolean* show);

    /** @hide */
    CARAPI ShowSoftInputUnchecked(
        /* [in] */ Int32 flags,
        /* [in] */ IResultReceiver* resultReceiver);

    /**
     * Synonym for {@link #hideSoftInputFromWindow(IBinder, Int32, ResultReceiver)
     * without a result: request to hide the soft input window from the
     * context of the window that is currently accepting input.
     *
     * @param windowToken The token of the window that is making the request,
     * as returned by {@link View#getWindowToken() View.getWindowToken()}.
     * @param flags Provides additional operating flags.  Currently may be
     * 0 or have the {@link #HIDE_IMPLICIT_ONLY} bit set.
     */
    CARAPI HideSoftInputFromWindow(
        /* [in] */ IBinder* windowToken,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* hide);

    /**
     * Request to hide the soft input window from the context of the window
     * that is currently accepting input.  This should be called as a result
     * of the user doing some actually than fairly explicitly requests to
     * have the input window hidden.
     *
     * @param windowToken The token of the window that is making the request,
     * as returned by {@link View#getWindowToken() View.getWindowToken()}.
     * @param flags Provides additional operating flags.  Currently may be
     * 0 or have the {@link #HIDE_IMPLICIT_ONLY} bit set.
     * @param resultReceiver If non-NULL, this will be called by the IME when
     * it has processed your request to tell you what it has done.  The result
     * code you receive may be either {@link #RESULT_UNCHANGED_SHOWN},
     * {@link #RESULT_UNCHANGED_HIDDEN}, {@link #RESULT_SHOWN}, or
     * {@link #RESULT_HIDDEN}.
     */
    CARAPI HideSoftInputFromWindowEx(
        /* [in] */ IBinder* windowToken,
        /* [in] */ Int32 flags,
        /* [in] */ IResultReceiver* resultReceiver,
        /* [out] */ Boolean* hide);

    /**
     * This method toggles the input method window display.
     * If the input window is already displayed, it gets hidden.
     * If not the input window will be displayed.
     * @param windowToken The token of the window that is making the request,
     * as returned by {@link View#getWindowToken() View.getWindowToken()}.
     * @param showFlags Provides additional operating flags.  May be
     * 0 or have the {@link #SHOW_IMPLICIT},
     * {@link #SHOW_FORCED} bit set.
     * @param hideFlags Provides additional operating flags.  May be
     * 0 or have the {@link #HIDE_IMPLICIT_ONLY},
     * {@link #HIDE_NOT_ALWAYS} bit set.
     **/
    CARAPI ToggleSoftInputFromWindow(
        /* [in] */ IBinder* windowToken,
        /* [in] */ Int32 showFlags,
        /* [in] */ Int32 hideFlags);

    /*
     * This method toggles the input method window display.
     * If the input window is already displayed, it gets hidden.
     * If not the input window will be displayed.
     * @param showFlags Provides additional operating flags.  May be
     * 0 or have the {@link #SHOW_IMPLICIT},
     * {@link #SHOW_FORCED} bit set.
     * @param hideFlags Provides additional operating flags.  May be
     * 0 or have the {@link #HIDE_IMPLICIT_ONLY},
     * {@link #HIDE_NOT_ALWAYS} bit set.
     * @hide
     */
    CARAPI ToggleSoftInput(
        /* [in] */ Int32 showFlags,
        /* [in] */ Int32 hideFlags);

    /**
     * If the input method is currently connected to the given view,
     * restart it with its new contents.  You should call this when the text
     * within your view changes outside of the normal input method or key
     * input flow, such as when an application calls TextView.setText().
     *
     * @param view The view whose text has changed.
     */
    CARAPI RestartInput(
        /* [in] */ IView* view);

    /**
     * When the focused window is dismissed, this method is called to finish the
     * input method started before.
     * @hide
     */
    CARAPI WindowDismissed(
        /* [in] */ IBinder* appWindowToken);

    /**
     * Call this when a view receives focus.
     * @hide
     */
    CARAPI FocusIn(
        /* [in] */ IView* view);
    /**
     * Call this when a view loses focus.
     * @hide
     */
    CARAPI FocusOut(
        /* [in] */ IView* view);

    /**
     * @hide
     */
    CARAPI CheckFocus();

    /**
     * Called by ViewRoot when its window gets input focus.
     * @hide
     */
    CARAPI OnWindowFocus(
        /* [in] */ IView* rootView,
        /* [in] */ IView* focusedView,
        /* [in] */ Int32 softInputMode,
        /* [in] */ Boolean first,
        /* [in] */ Int32 windowFlags);

    /** @hide */
    CARAPI StartGettingWindowFocus(
        /* [in] */ IView* rootView);

    /**
     * Report the current selection range.
     */
    CARAPI UpdateSelection(
        /* [in] */ IView* view,
        /* [in] */ Int32 selStart,
        /* [in] */ Int32 selEnd,
        /* [in] */ Int32 candidatesStart,
        /* [in] */ Int32 candidatesEnd);

    /**
     * Returns true if the current input method wants to watch the location
     * of the input editor's cursor in its window.
     */
    CARAPI IsWatchingCursor(
        /* [in] */ IView* view,
        /* [out] */ Boolean* isWatching);

    /**
     * Report the current cursor location in its window.
     */
    CARAPI UpdateCursor(
        /* [in] */ IView* view,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    /**
     * Call {@link InputMethodSession#appPrivateCommand(String, Bundle)
     * InputMethodSession.appPrivateCommand()} on the current Input Method.
     * @param view Optional View that is sending the command, or NULL if
     * you want to send the command regardless of the view that is attached
     * to the input method.
     * @param action Name of the command to be performed.  This <em>must</em>
     * be a scoped name, i.e. prefixed with a package name you own, so that
     * different developers will not create conflicting commands.
     * @param data Any data to include with the command.
     */
    CARAPI SendAppPrivateCommand(
        /* [in] */ IView* view,
        /* [in] */ const String& action,
        /* [in] */ IBundle* data);

    /**
     * Force switch to a new input method component.  This can only be called
     * from the currently active input method, as validated by the given token.
     * @param token Supplies the identifying token given to an input method
     * when it was started, which allows it to perform this operation on
     * itself.
     * @param id The unique identifier for the new input method to be switched to.
     */
    CARAPI SetInputMethod(
        /* [in] */ IBinder* token,
        /* [in] */ const String& id);

    /**
     * Close/hide the input method's soft input area, so the user no longer
     * sees it or can interact with it.  This can only be called
     * from the currently active input method, as validated by the given token.
     *
     * @param token Supplies the identifying token given to an input method
     * when it was started, which allows it to perform this operation on
     * itself.
     * @param flags Provides additional operating flags.  Currently may be
     * 0 or have the {@link #HIDE_IMPLICIT_ONLY},
     * {@link #HIDE_NOT_ALWAYS} bit set.
     */
    CARAPI HideSoftInputFromInputMethod(
        /* [in] */ IBinder* token,
        /* [in] */ Int32 flags);

    /**
     * Show the input method's soft input area, so the user
     * sees the input method window and can interact with it.
     * This can only be called from the currently active input method,
     * as validated by the given token.
     *
     * @param token Supplies the identifying token given to an input method
     * when it was started, which allows it to perform this operation on
     * itself.
     * @param flags Provides additional operating flags.  Currently may be
     * 0 or have the {@link #SHOW_IMPLICIT} or
     * {@link #SHOW_FORCED} bit set.
     */
    CARAPI ShowSoftInputFromInputMethod(
        /* [in] */ IBinder* token,
        /* [in] */ Int32 flags);

    /**
     * @hide
     */
    CARAPI DispatchKeyEvent(
        /* [in] */ IContext* context,
        /* [in] */ Int32 seq,
        /* [in] */ IKeyEvent* key,
        /* [in] */ IInputMethodCallbackStub* callback);

    CARAPI ShowInputMethodPicker();

private:
    // CARAPI constructor(
    //     /* [in] */ IInputMethodManagerStub* service,
    //     /* [in] */ IApartment* looper);

    /**
     * Reset all of the state associated with a served view being connected
     * to an input method
     */
    CARAPI_(void) ClearConnectionLocked();

    /**
     * Disconnect any existing input connection, clearing the served view.
     */
    CARAPI_(void) FinishInputLocked();

    CARAPI_(void) StartInputInner();

    CARAPI_(void) FocusInLocked(
        /* [in] */ IView* view);

    CARAPI_(void) ScheduleCheckFocusLocked(
        /* [in] */ IView* view);

    CARAPI_(void) CloseCurrentInput();

    /**
     * @hide
     */
    CARAPI_(void) DispatchTrackballEvent(
        /* [in] */ IContext* context,
        /* [in] */ Int32 seq,
        /* [in] */ IMotionEvent* motion,
        /* [in] */ IInputMethodCallbackStub* callback);

    CARAPI_(void) HandleMsgBind(
        /* [in] */ IInputBindResult* res);

    CARAPI_(void) HandleMsgUnBind(
        /* [in] */ Int32 sequence);

    CARAPI_(void) HandleMsgSetActive(
        /* [in] */ Int32 flag);

public:
    /**
     * This is the view that should currently be served by an input method,
     * regardless of the state of setting that up.
     */
    AutoPtr<IView> mServedView;

    // For scheduling work on the main thread.  This also serves as our
    // global lock.
    AutoPtr<IApartment> mH;

    static Mutex sStaticHandlerLock;

private:
    // static final boolean DEBUG = false;
    static CString TAG;

    static AutoPtr<ILocalInputMethodManager> mInstance;

    AutoPtr<IInputMethodManagerStub> mService;
    AutoPtr<IApartment> mMainLooper;

    // Our generic input connection if the current target does not have its own.
    AutoPtr<IInputContextStub> mIInputContext;

    /**
     * True if this input method client is active, initially false.
     */
    Boolean mActive;

    /**
     * Set whenever this client becomes inactive, to know we need to reset
     * state with the IME then next time we receive focus.
     */
    Boolean mHasBeenInactive;

    /**
     * As reported by IME through InputConnection.
     */
    Boolean mFullscreenMode;

    // -----------------------------------------------------------

    /**
     * This is the root view of the overall window that currently has input
     * method focus.
     */
    AutoPtr<IView> mCurRootView;

    /**
     * This is then next view that will be served by the input method, when
     * we get around to updating things.
     */
    AutoPtr<IView> mNextServedView;
    /**
     * True if we should restart input in the next served view, even if the
     * view hasn't actually changed from the current serve view.
     */
    Boolean mNextServedNeedsStart;
    /**
     * This is set when we are in the process of connecting, to determine
     * when we have actually finished.
     */
    Boolean mServedConnecting;
    /**
     * This is non-NULL when we have connected the served view; it holds
     * the attributes that were last retrieved from the served view and given
     * to the input connection.
     */
    AutoPtr<IEditorInfo> mCurrentTextBoxAttribute;
    /**
     * The InputConnection that was last retrieved from the served view.
     */
    AutoPtr<IInputConnection> mServedInputConnection;
    /**
     * The completions that were last provided by the served view.
     */
    ArrayOf<ICompletionInfo*>* mCompletions;

    // Cursor position on the screen.
    AutoPtr<IRect> mTmpCursorRect;
    AutoPtr<IRect> mCursorRect;
    Int32 mCursorSelStart;
    Int32 mCursorSelEnd;
    Int32 mCursorCandStart;
    Int32 mCursorCandEnd;

    // -----------------------------------------------------------

    /**
     * Sequence number of this binding, as returned by the server.
     */
    Int32 mBindSequence;
    /**
     * ID of the method we are bound to.
     */
    String mCurId;
    /**
     * The actual instance of the method to make calls on it.
     */
    AutoPtr<IInputMethodSessionStub> mCurMethod;

    // -----------------------------------------------------------

    static const Int32 MSG_DUMP = 1;
    static const Int32 MSG_BIND = 2;
    static const Int32 MSG_UNBIND = 3;
    static const Int32 MSG_SET_ACTIVE = 4;

    AutoPtr<IInputMethodClientStub> mClient;

    AutoPtr<IInputConnection> mDummyInputConnection;

    static Mutex sStaticInstanceLock;
};


#endif   //_CINPUTMETHODMANAGER_H__
