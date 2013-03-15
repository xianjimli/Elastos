
#ifndef __CSOFTKEYBOARD_H__
#define __CSOFTKEYBOARD_H__

#define __USE_MALLOC

#include "_CSoftKeyboard.h"
#include "InputMethodService.h"
#include <elastos/AutoPtr.h>
#include <elastos/Character.h>
#include <StringBuffer.h>
#include <elastos/List.h>

CarClass(CSoftKeyboard), public InputMethodService
{
public:
    CSoftKeyboard();

    ~CSoftKeyboard();

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI GetAssets(
        /* [out] */ IAssetManager** assetManager);

    CARAPI GetResources(
        /* [out] */ IResources** resources);

    CARAPI GetCapsuleManager(
        /* [out] */ ILocalCapsuleManager** capsuleManager);

    CARAPI GetContentResolver(
        /* [out] */ IContentResolver** resolver);

    CARAPI GetApplicationContext(
        /* [out] */ IContext** ctx);

    CARAPI GetText(
        /* [in] */ Int32 resId,
        /* [out] */ ICharSequence** text);

    CARAPI GetString(
        /* [in] */ Int32 resId,
        /* [out] */ String* str);

    CARAPI SetTheme(
        /* [in] */ Int32 resid);

    CARAPI GetTheme(
        /* [out] */ ITheme** theme);

    CARAPI ObtainStyledAttributes(
        /* [in] */ const ArrayOf<Int32> & attrs,
        /* [out] */ ITypedArray** styles);

    CARAPI ObtainStyledAttributesEx(
        /* [in] */ Int32 resid,
        /* [in] */ const ArrayOf<Int32> & attrs,
        /* [out] */ ITypedArray** styles);

    CARAPI ObtainStyledAttributesEx2(
        /* [in] */ IAttributeSet* set,
        /* [in] */ const ArrayOf<Int32> & attrs,
        /* [out] */ ITypedArray** styles);

    CARAPI ObtainStyledAttributesEx3(
        /* [in] */ IAttributeSet* set,
        /* [in] */ const ArrayOf<Int32>& attrs,
        /* [in] */ Int32 defStyleAttr,
        /* [in] */ Int32 defStyleRes,
        /* [out] */ ITypedArray** styles);

    CARAPI GetClassLoader(
        /* [out] */ IClassLoader** loader);

    CARAPI GetCapsuleName(
        /* [out] */ String* capsuleName);

    CARAPI GetApplicationInfo(
        /* [out] */ IApplicationInfo** info);

    CARAPI StartActivity(
        /* [in] */ IIntent* intent);

    CARAPI SendBroadcast(
        /* [in] */ IIntent* intent);

    CARAPI StartService(
        /* [in] */ IIntent* service,
        /* [out] */ IComponentName** name);

    CARAPI StopService(
        /* [in] */ IIntent* service,
        /* [out] */ Boolean* succeeded);

    CARAPI BindService(
        /* [in] */ IIntent* service,
        /* [in] */ IServiceConnection* conn,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* succeeded);

    CARAPI UnbindService(
        /* [in] */ IServiceConnection* conn);

    CARAPI GetSystemService(
        /* [in] */ CString name,
        /* [out] */ IInterface** object);

    CARAPI CreateCapsuleContext(
        /* [in] */ const String& capsuleName,
        /* [in] */ Int32 flags,
        /* [out] */ IContext** ctx);

    CARAPI CheckCallingPermission(
        /* [in] */ const String& permission,
        /* [out] */ Int32* value);

    CARAPI EnforceCallingOrSelfPermission(
        /* [in] */ CString permission,
        /* [in] */ CString message);

    CARAPI RevokeUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags);

    CARAPI CheckCallingOrSelfPermission(
        /* [in] */ const String& permission,
        /* [out] */ Int32* perm);

    CARAPI GrantUriPermission(
        /* [in] */ const String& toCapsule,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags);

    CARAPI GetBaseContext(
        /* [out] */ IContext** ctx);

    CARAPI Attach(
        /* [in] */ IContext* ctx,
        /* [in] */ IApplicationApartment* apartment,
        /* [in] */ const String& className,
        /* [in] */ IBinder* token,
        /* [in] */ IApplication* application,
        /* [in] */ IActivityManager* activityManager);

    CARAPI Create();

    CARAPI Start(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 startId);

    CARAPI StartCommand(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 startId,
        /* [out] */ Int32* result);

    CARAPI Destroy();

    CARAPI Bind(
        /* [in] */ IIntent* intent,
        /* [out] */ IBinder** binder);

    CARAPI Unbind(
        /* [in] */ IIntent* intent,
        /* [out] */ Boolean* succeeded);

    CARAPI Rebind(
        /* [in] */ IIntent* intent);

    CARAPI GetClassName(
        /* [out] */ String* className);

    CARAPI GetKeyDispatcherState(
        /* [out] */ IDispatcherState** dispatcherState);

    CARAPI OnCreateInputMethodInterface(
        /* [out] */ IAbstractInputMethodImpl** inputMethodImpl);

    CARAPI OnCreateInputMethodSessionInterface(
        /* [out] */ IAbstractInputMethodSessionImpl** abstractInputMethodSessionImpl);

    CARAPI OnTrackballEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* state);

    CARAPI OnInitializeInterface();

    CARAPI GetLayoutInflater(
        /* [out] */ ILayoutInflater** inflater);

    CARAPI GetWindow(
        /* [out] */ IDialog** dialog);

    CARAPI GetMaxWidth(
        /* [out] */ Int32* maxWidth);

    CARAPI GetCurrentInputBinding(
        /* [out] */ IInputBinding** binding);

    CARAPI GetCurrentInputConnection(
        /* [out] */ IInputConnection** inputConnection);

    CARAPI GetCurrentInputStarted(
        /* [out] */ Boolean* state);

    CARAPI GetCurrentInputEditorInfo(
        /* [out] */ IEditorInfo** editorInfo);

    CARAPI UpdateFullscreenMode();

    CARAPI OnConfigureWindow(
        /* [in] */ IWindow* win,
        /* [in] */ Boolean isFullscreen,
        /* [in] */ Boolean isCandidatesOnly);

    CARAPI IsFullscreenMode(
        /* [out] */ Boolean* fullScreenMode);

    CARAPI OnEvaluateFullscreenMode(
        /* [out] */ Boolean* screenMode);

    CARAPI SetExtractViewShown(
        /* [in] */ Boolean shown);

    CARAPI IsExtractViewShown(
        /* [out] */ Boolean* shown);

    CARAPI UpdateInputViewShown();

    CARAPI IsShowInputRequested(
        /* [out] */ Boolean* requested);

    CARAPI IsInputViewShown(
        /* [out] */ Boolean* shown);

    CARAPI OnEvaluateInputViewShown(
        /* [out] */ Boolean* shown);

    CARAPI SetCandidatesViewShown(
        /* [in] */ Boolean shown);

    CARAPI GetCandidatesHiddenVisibility(
        /* [out] */ Int32* visibility);

    CARAPI ShowStatusIcon(
        /* [in] */ Int32 iconResId);

    CARAPI HideStatusIcon();

    CARAPI SwitchInputMethod(
        /* [in] */ const String& id);

    CARAPI SetExtractView(
        /* [in] */ IView* view);

    CARAPI SetCandidatesView(
        /* [in] */ IView* view);

    CARAPI SetInputView(
        /* [in] */ IView* view);

    CARAPI OnCreateExtractTextView(
        /* [out] */ IView** view);

    CARAPI OnCreateCandidatesView(
        /* [out] */ IView** view);

    CARAPI OnCreateInputView(
        /* [out] */ IView** view);

    CARAPI OnStartInputView(
        /* [in] */ IEditorInfo* info,
        /* [in] */ Boolean restarting);

    CARAPI OnFinishInputView(
        /* [in] */ Boolean finishingInput);

    CARAPI OnStartCandidatesView(
        /* [in] */ IEditorInfo* info,
        /* [in] */ Boolean restarting);

    CARAPI OnFinishCandidatesView(
        /* [in] */ Boolean finishingInput);

    CARAPI OnShowInputRequested(
        /* [in] */ Int32 flags,
        /* [in] */ Boolean configChange,
        /* [out] */ Boolean* requested);

    CARAPI ShowWindow(
        /* [in] */ Boolean showInput);

    CARAPI HideWindow();

    CARAPI OnWindowShown();

    CARAPI OnWindowHidden();

    CARAPI OnBindInput();

    CARAPI OnUnbindInput();

    CARAPI OnStartInput(
        /* [in] */ IEditorInfo* attribute,
        /* [in] */ Boolean restarting);

    CARAPI OnFinishInput();

    CARAPI OnDisplayCompletions(
        /* [in] */ ArrayOf<ICompletionInfo *>* completions);

    CARAPI OnUpdateExtractedText(
        /* [in] */ Int32 token,
        /* [in] */ IExtractedText* text);

    CARAPI OnUpdateSelection(
        /* [in] */ Int32 oldSelStart,
        /* [in] */ Int32 oldSelEnd,
        /* [in] */ Int32 newSelStart,
        /* [in] */ Int32 newSelEnd,
        /* [in] */ Int32 candidatesStart,
        /* [in] */ Int32 candidatesEnd);

    CARAPI OnUpdateCursor(
        /* [in] */ IRect* newCursor);

    CARAPI RequestHideSelf(
        /* [in] */ Int32 flags);

    CARAPI OnAppPrivateCommand(
        /* [in] */ const String& action,
        /* [in] */ IBundle* data);

    CARAPI SendDownUpKeyEvents(
        /* [in] */ Int32 keyEventCode);

    CARAPI SendDefaultEditorAction(
        /* [in] */ Boolean fromEnterKey,
        /* [out] */ Boolean* state);

    CARAPI SendKeyChar(
        /* [in] */ Char32 charCode);

    CARAPI OnExtractedSelectionChanged(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI OnExtractedTextClicked();

    CARAPI OnExtractedCursorMovement(
        /* [in] */ Int32 dx,
        /* [in] */ Int32 dy);

    CARAPI OnExtractTextContextMenuItem(
        /* [in] */ Int32 id,
        /* [out] */ Boolean* state);

    CARAPI GetTextForImeAction(
        /* [in] */ Int32 imeOptions,
        /* [out] */ ICharSequence** text);

    CARAPI OnUpdateExtractingVisibility(
        /* [in] */ IEditorInfo* ei);

    CARAPI OnUpdateExtractingViews(
        /* [in] */ IEditorInfo* ei);

    CARAPI OnExtractingInputChanged(
        /* [in] */ IEditorInfo* ei);

    CARAPI OnPress(
        /* [in] */ Int32 primaryCode);

    CARAPI OnRelease(
        /* [in] */ Int32 primaryCode);

    CARAPI OnKey(
        /* [in] */ Int32 primaryCode,
        /* [in] */ ArrayOf<Int32>* keyCodes);

    CARAPI OnText(
        /* [in] */ ICharSequence* text);

    CARAPI SwipeLeft();

    CARAPI SwipeRight();

    CARAPI SwipeDown();

    CARAPI SwipeUp();

    CARAPI OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    CARAPI OnLowMemory();

    CARAPI OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnKeyLongPress(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnKeyMultiple(
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 count,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI constructor();

    CARAPI_(void) SetSuggestions(
        /* [in] */ ArrayOf<ICharSequence*>* suggestions,
        /* [in] */ Boolean completions,
        /* [in] */ Boolean typedWordValid);

    CARAPI_(Boolean) IsWordSeparator(
        /* [in] */ Int32 code);

    CARAPI_(void) PickDefaultCandidate();

    CARAPI PickSuggestionManually(
        /* [in] */ Int32 index);

private:
    /**
     * This translates incoming hard key events in to edit operations on an
     * InputConnection.  It is only needed when using the
     * PROCESS_HARD_KEYS option.
     */
    CARAPI_(Boolean) TranslateKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    /**
     * Helper function to commit any text being composed in to the editor.
     */
    CARAPI_(void) CommitTyped(
        /* [in] */ IInputConnection* inputConnection);

    /**
     * Helper to update the shift state of our keyboard based on the initial
     * editor state.
     */
    CARAPI_(void) UpdateShiftKeyState(
        /* [in] */ IEditorInfo* attr);

    /**
     * Helper to determine if a given character code is alphabetic.
     */
    CARAPI_(Boolean) IsAlphabet(
        /* [in] */ Int32 code);

    /**
     * Helper to send a key down / key up pair to the current editor.
     */
    CARAPI_(void) KeyDownUp(
        /* [in] */ Int32 keyEventCode);

    /**
     * Helper to send a character to the editor as raw key events.
     */
    CARAPI_(void) SendKey(
        /* [in] */ Int32 keyCode);

    /**
     * Update the list of available candidates from the current composing
     * text.  This will need to be filled in by however you are determining
     * candidates.
     */
    CARAPI_(void) UpdateCandidates();

    CARAPI_(void) HandleBackspace();

    CARAPI_(void) HandleShift();

    CARAPI_(void) HandleCharacter(
        /* [in] */ Int32 primaryCode,
        /* [in] */ ArrayOf<Int32>* keyCodes);

    CARAPI_(void) HandleClose();

    CARAPI_(void) CheckToggleCapsLock();

private:
    /**
     * This Boolean indicates the optional example code for performing
     * processing of hard keys in addition to regular text generation
     * from on-screen interaction.  It would be used for input methods that
     * perform language translations (such as converting text entered on
     * a QWERTY keyboard to Chinese), but may not be used for input methods
     * that are primarily intended to be used for on-screen text entry.
     */
    static const Boolean PROCESS_HARD_KEYS = TRUE;

    AutoPtr<IKeyboardView>           mInputView;
    AutoPtr<ICandidateView>          mCandidateView;
    ArrayOf< AutoPtr<ICompletionInfo> >* mCompletions;

    StringBuffer                     mComposing;
    Boolean                          mPredictionOn;
    Boolean                          mCompletionOn;
    Int32                            mLastDisplayWidth;
    Boolean                          mCapsLock;
    Int64                            mLastShiftTime;
    Int64                            mMetaState;

    AutoPtr<IKeyboard>               mSymbolsKeyboard;
    AutoPtr<IKeyboard>               mSymbolsShiftedKeyboard;
    AutoPtr<IKeyboard>               mQwertyKeyboard;
    AutoPtr<IKeyboard>               mCurKeyboard;

    String                           mWordSeparators;
};

#endif // __CSOFTKEYBOARD_H__
