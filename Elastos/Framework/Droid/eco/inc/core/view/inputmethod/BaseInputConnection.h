
#ifndef  __BASEINPUTCONNECTION_H__
#define  __BASEINPUTCONNECTION_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>
#include "view/ElKeyCharacterMap.h"

class ComposingText:
    public ElRefBase,
    public INoCopySpan {
public:
    ComposingText();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);
};

/**
 * Base class for implementors of the InputConnection interface, taking care
 * of most of the common behavior for providing a connection to an Editable.
 * Implementors of this class will want to be sure to implement
 * {@link #getEditable} to provide access to their own editable object.
 */
class BaseInputConnection {
    BaseInputConnection(
        /* [in] */ IInputMethodManager* mgr,
        /* [in] */ Boolean fullEditor);

    BaseInputConnection(
        /* [in] */ IView* targetView,
        /* [in] */ Boolean fullEditor);

    ~BaseInputConnection();

public:
    static CARAPI RemoveComposingSpans(
        /* [in] */ ISpannable* text);

    static CARAPI SetComposingSpans(
        /* [in] */ ISpannable* text);

    /** @hide */
    static CARAPI SetComposingSpans(
        /* [in] */ ISpannable* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    static CARAPI GetComposingSpanStart(
        /* [in] */ ISpannable* text,
        /* [out] */ Int32* start);

    static CARAPI GetComposingSpanEnd(
        /* [in] */ ISpannable* text,
        /* [out] */ Int32* end);

    /**
     * Return the target of edit operations.  The default implementation
     * returns its own fake editable that is just used for composing text;
     * subclasses that are real text editors should override this and
     * supply their own.
     */
    CARAPI GetEditable(
        /* [out] */ IEditable** editable);

    /**
     * Default implementation does nothing.
     */
    CARAPI BeginBatchEdit(
        /* [out] */ Boolean* state);

    /**
     * Default implementation does nothing.
     */
    CARAPI EndBatchEdit(
        /* [out] */ Boolean* state);

    /**
     * Default implementation uses
     * {@link MetaKeyKeyListener#clearMetaKeyState(long, int)
     * MetaKeyKeyListener.clearMetaKeyState(long, int)} to clear the state.
     */
    CARAPI ClearMetaKeyStates(
        /* [in] */ Int32 states,
        /* [out] */ Boolean* clear);

    /**
     * Default implementation does nothing.
     */
    CARAPI CommitCompletion(
        /* [in] */ ICompletionInfo* text,
        /* [out] */ Boolean* state);

    /**
     * Default implementation replaces any existing composing text with
     * the given text.  In addition, only if dummy mode, a key event is
     * sent for the new text and the current editable buffer cleared.
     */
    CARAPI CommitText(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 newCursorPosition,
        /* [out] */ Boolean* state);

    /**
     * The default implementation performs the deletion around the current
     * selection position of the editable text.
     */
    CARAPI DeleteSurroundingText(
        /* [in] */ Int32 leftLength,
        /* [in] */ Int32 rightLength,
        /* [out] */ Boolean* state);

    /**
     * The default implementation removes the composing state from the
     * current editable text.  In addition, only if dummy mode, a key event is
     * sent for the new text and the current editable buffer cleared.
     */
    CARAPI FinishComposingText(
        /* [out] */ Boolean* state);

    /**
     * The default implementation uses TextUtils.getCapsMode to get the
     * cursor caps mode for the current selection position in the editable
     * text, unless in dummy mode in which case 0 is always returned.
     */
    CARAPI GetCursorCapsMode(
        /* [in] */ Int32 reqModes,
        /* [out] */ Int32* capsMode);

    /**
     * The default implementation always returns null.
     */
    CARAPI GetExtractedText(
        /* [in] */ IExtractedTextRequest* request,
        /* [in] */ Int32 flags,
        /* [out] */ IExtractedText** text);

    /**
     * The default implementation returns the given amount of text from the
     * current cursor position in the buffer.
     */
    CARAPI GetTextBeforeCursor(
        /* [in] */ Int32 length,
        /* [in] */ Int32 flags,
        /* [out] */ ICharSequence** text);

    /**
     * The default implementation returns the text currently selected, or null if none is
     * selected.
     */
    CARAPI GetSelectedText(
        /* [in] */ Int32 flags,
        /* [out] */ ICharSequence** text);

    /**
     * The default implementation returns the given amount of text from the
     * current cursor position in the buffer.
     */
    CARAPI GetTextAfterCursor(
        /* [in] */ Int32 length,
        /* [in] */ Int32 flags,
        /* [out] */ ICharSequence** text);

    /**
     * The default implementation turns this into the enter key.
     */
    CARAPI PerformEditorAction(
        /* [in] */ Int32 actionCode,
        /* [out] */ Boolean* state);

    /**
     * The default implementation does nothing.
     */
    CARAPI PerformContextMenuAction(
        /* [in] */ Int32 id,
        /* [out] */ Boolean* state);

    /**
     * The default implementation does nothing.
     */
    CARAPI PerformPrivateCommand(
        /* [in] */ String action,
        /* [in] */ IBundle* data,
        /* [out] */ Boolean* state);

    /**
     * The default implementation places the given text into the editable,
     * replacing any existing composing text.  The new text is marked as
     * in a composing state with the composing style.
     */
    CARAPI SetComposingText(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 newCursorPosition,
        /* [out] */ Boolean* state);

    CARAPI SetComposingRegion(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ Boolean* state);

    /**
     * The default implementation changes the selection position in the
     * current editable text.
     */
    CARAPI SetSelection(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ Boolean* state);

    /**
     * Provides standard implementation for sending a key event to the window
     * attached to the input connection's view.
     */
    CARAPI SendKeyEvent(
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* state);

    /**
     * Updates InputMethodManager with the current fullscreen mode.
     */
    CARAPI ReportFullscreenMode(
        /* [in] */ Boolean enabled,
        /* [out] */ Boolean* state);

private:
    CARAPI_(void) SendCurrentText();

    CARAPI_(void) EnsureDefaultComposingSpans();

    CARAPI_(void) ReplaceText(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 newCursorPosition,
        /* [in] */ Boolean composing);

private:
    static const Boolean DEBUG = FALSE;
    static const CString TAG;
    static const AutoPtr<IInterface> COMPOSING;

    AutoPtr<IInputMethodManager> mIMM;
    AutoPtr<IView> mTargetView;
    const Boolean mDummyMode;

    ArrayOf<IInterface*>* mDefaultComposingSpans;

    AutoPtr<IEditable> mEditable;
    AutoPtr<ElKeyCharacterMap> mKeyCharacterMap;
};

#endif  //__BASEINPUTCONNECTION_H__
