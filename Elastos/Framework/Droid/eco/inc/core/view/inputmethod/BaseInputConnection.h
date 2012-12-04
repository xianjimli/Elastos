
#ifndef  __BASEINPUTCONNECTION_H__
#define  __BASEINPUTCONNECTION_H__

#include "ext/frameworkext.h"
#include "view/ElKeyCharacterMap.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>



/**
 * Base class for implementors of the InputConnection interface, taking care
 * of most of the common behavior for providing a connection to an Editable.
 * Implementors of this class will want to be sure to implement
 * {@link #getEditable} to provide access to their own editable object.
 */
class BaseInputConnection
{
private:
    class ComposingText
        : public ElRefBase,
        , public INoCopySpan
    {
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

public:
    BaseInputConnection(
        /* [in] */ ILocalInputMethodManager* mgr,
        /* [in] */ Boolean fullEditor);

    BaseInputConnection(
        /* [in] */ IView* targetView,
        /* [in] */ Boolean fullEditor);

    ~BaseInputConnection();

    static CARAPI RemoveComposingSpans(
        /* [in] */ ISpannable* text);

    static CARAPI SetComposingSpans(
        /* [in] */ ISpannable* text);

    /** @hide */
    static CARAPI SetComposingSpans(
        /* [in] */ ISpannable* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    static CARAPI_(Int32) GetComposingSpanStart(
        /* [in] */ ISpannable* text);

    static CARAPI_(Int32) GetComposingSpanEnd(
        /* [in] */ ISpannable* text);

    /**
     * Return the target of edit operations.  The default implementation
     * returns its own fake editable that is just used for composing text;
     * subclasses that are real text editors should override this and
     * supply their own.
     */
    virtual CARAPI_(AutoPtr<IEditable>) GetEditable();

    /**
     * Default implementation does nothing.
     */
    virtual CARAPI_(Boolean) BeginBatchEdit();

    /**
     * Default implementation does nothing.
     */
    virtual CARAPI_(Boolean) EndBatchEdit();

    /**
     * Default implementation uses
     * {@link MetaKeyKeyListener#clearMetaKeyState(long, int)
     * MetaKeyKeyListener.clearMetaKeyState(long, int)} to clear the state.
     */
    virtual CARAPI_(Boolean) ClearMetaKeyStates(
        /* [in] */ Int32 states);

    /**
     * Default implementation does nothing.
     */
    virtual CARAPI_(Boolean) CommitCompletion(
        /* [in] */ ICompletionInfo* text);

    /**
     * Default implementation replaces any existing composing text with
     * the given text.  In addition, only if dummy mode, a key event is
     * sent for the new text and the current editable buffer cleared.
     */
    virtual CARAPI_(Boolean) CommitText(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 newCursorPosition);

    /**
     * The default implementation performs the deletion around the current
     * selection position of the editable text.
     */
    virtual CARAPI_(Boolean) DeleteSurroundingText(
        /* [in] */ Int32 leftLength,
        /* [in] */ Int32 rightLength);

    /**
     * The default implementation removes the composing state from the
     * current editable text.  In addition, only if dummy mode, a key event is
     * sent for the new text and the current editable buffer cleared.
     */
    virtual CARAPI_(Boolean) FinishComposingText(
        /* [out] */ Boolean* state);

    /**
     * The default implementation uses TextUtils.getCapsMode to get the
     * cursor caps mode for the current selection position in the editable
     * text, unless in dummy mode in which case 0 is always returned.
     */
    virtual CARAPI_(Int32) GetCursorCapsMode(
        /* [in] */ Int32 reqModes);

    /**
     * The default implementation always returns null.
     */
    virtual CARAPI_(AutoPtr<IExtractedText>) GetExtractedText(
        /* [in] */ IExtractedTextRequest* request,
        /* [in] */ Int32 flags);

    /**
     * The default implementation returns the given amount of text from the
     * current cursor position in the buffer.
     */
    virtual CARAPI_(AutoPtr<ICharSequence>) GetTextBeforeCursor(
        /* [in] */ Int32 length,
        /* [in] */ Int32 flags);

    /**
     * The default implementation returns the text currently selected, or null if none is
     * selected.
     */
    virtual CARAPI_(AutoPtr<ICharSequence>) GetSelectedText(
        /* [in] */ Int32 flags);

    /**
     * The default implementation returns the given amount of text from the
     * current cursor position in the buffer.
     */
    virtual CARAPI_(AutoPtr<ICharSequence>) GetTextAfterCursor(
        /* [in] */ Int32 length,
        /* [in] */ Int32 flags);

    /**
     * The default implementation turns this into the enter key.
     */
    virtual CARAPI_(Boolean) PerformEditorAction(
        /* [in] */ Int32 actionCode);

    /**
     * The default implementation does nothing.
     */
    virtual CARAPI_(Boolean) PerformContextMenuAction(
        /* [in] */ Int32 id);

    /**
     * The default implementation does nothing.
     */
    virtual CARAPI_(Boolean) PerformPrivateCommand(
        /* [in] */ String action,
        /* [in] */ IBundle* data);

    /**
     * The default implementation places the given text into the editable,
     * replacing any existing composing text.  The new text is marked as
     * in a composing state with the composing style.
     */
    virtual CARAPI_(Boolean) SetComposingText(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 newCursorPosition);

    virtual CARAPI_(Boolean) SetComposingRegion(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    /**
     * The default implementation changes the selection position in the
     * current editable text.
     */
    virtual CARAPI_(Boolean) SetSelection(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    /**
     * Provides standard implementation for sending a key event to the window
     * attached to the input connection's view.
     */
    virtual CARAPI_(Boolean) SendKeyEvent(
        /* [in] */ IKeyEvent* event);

    /**
     * Updates InputMethodManager with the current fullscreen mode.
     */
    virtual CARAPI_(Boolean) ReportFullscreenMode(
        /* [in] */ Boolean enabled);

protected:
    BaseInputConnection();

    CARAPI Init(
        /* [in] */ ILocalInputMethodManager* mgr,
        /* [in] */ Boolean fullEditor);

    CARAPI Init(
        /* [in] */ IView* targetView,
        /* [in] */ Boolean fullEditor);

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

    AutoPtr<ILocalInputMethodManager> mIMM;
    AutoPtr<IView> mTargetView;
    const Boolean mDummyMode;

    ArrayOf<IInterface*>* mDefaultComposingSpans;

    AutoPtr<IEditable> mEditable;
    AutoPtr<ElKeyCharacterMap> mKeyCharacterMap;
};

#endif  //__BASEINPUTCONNECTION_H__
