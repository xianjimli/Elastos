
#ifndef  _LOCALINPUTCONNECTIONWRAPPER_H__
#define  _LOCALINPUTCONNECTIONWRAPPER_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>

using namespace Elastos::Core::Threading;

class LocalInputConnectionWrapper:
    public ElRefBase,
    public IInputConnection
{
protected:
    class InputContextCallback:
        public ElRefBase,
        public IInputContextCallbackStub
    {
    public:
        InputContextCallback();

        ~InputContextCallback();

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI SetTextBeforeCursor(
            /* [in] */ ICharSequence* textBeforeCursor,
            /* [in] */ Int32 seq);

        CARAPI SetTextAfterCursor(
            /* [in] */ ICharSequence* textAfterCursor,
            /* [in] */ Int32 seq);

        CARAPI SetSelectedText(
            /* [in] */ ICharSequence* selectedText,
            /* [in] */ Int32 seq);

        CARAPI SetCursorCapsMode(
            /* [in] */ Int32 capsMode,
            /* [in] */ Int32 seq);

        CARAPI SetExtractedText(
            /* [in] */ IExtractedText* extractedText,
            /* [in] */ Int32 seq);

        CARAPI GetDescription(
            /* [out] */ String* str);

       /**
         * Returns an InputContextCallback object that is guaranteed not to be in use by
         * any other thread.  The returned object's 'have value' flag is cleared and its expected
         * sequence number is set to a new integer.  We use a sequence number so that replies that
         * occur after a timeout has expired are not interpreted as replies to a later request.
         */
        static CARAPI_(AutoPtr<InputContextCallback>) GetInstance(
            /* [in] */ LocalInputConnectionWrapper* host);

        /**
         * Waits for a result for up to {@link #MAX_WAIT_TIME_MILLIS} milliseconds.
         *
         * <p>The caller must be synchronized on this callback object.
         */
        CARAPI_(void) WaitForResultLocked();

        /**
         * Makes the given InputContextCallback available for use in the future.
         */
        CARAPI_(void) Dispose();

    public:
        Int32 mSeq;
        Boolean mHaveValue;
        AutoPtr<ICharSequence> mTextBeforeCursor;
        AutoPtr<ICharSequence> mTextAfterCursor;
        AutoPtr<ICharSequence> mSelectedText;
        AutoPtr<IExtractedText> mExtractedText;
        Int32 mCursorCapsMode;
        static Mutex mPriLock;

    private:
        //static CString TAG = "LocalInputConnectionWrapper.ICC";
        // A 'pool' of one InputContextCallback.  Each ICW request will attempt to gain
        // exclusive access to this object.
        static AutoPtr<InputContextCallback> sInstance;
        static Int32 sSequenceNumber;
        static LocalInputConnectionWrapper* mHost;
    };

public:
    LocalInputConnectionWrapper(
        /* [in] */ IInputContextStub* inputContext);

    ~LocalInputConnectionWrapper();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI GetTextAfterCursor(
        /* [in] */ Int32 n,
        /* [in] */ Int32 flags,
        /* [out] */ ICharSequence** text);

    CARAPI GetTextBeforeCursor(
        /* [in] */ Int32 n,
        /* [in] */ Int32 flags,
        /* [out] */ ICharSequence** text);

    CARAPI GetSelectedText(
        /* [in] */ Int32 flags,
        /* [out] */ ICharSequence** text);

    CARAPI GetCursorCapsMode(
        /* [in] */ Int32 reqModes,
        /* [out] */ Int32* capsMode);

    CARAPI GetExtractedText(
        /* [in] */ IExtractedTextRequest* request,
        /* [in] */ Int32 flags,
        /* [out] */ IExtractedText** extractedText);

    CARAPI CommitText(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 newCursorPosition,
        /* [out] */ Boolean* flag);

    CARAPI CommitCompletion(
        /* [in] */ ICompletionInfo* text,
        /* [out] */ Boolean* flag);

    CARAPI SetSelection(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ Boolean* flag);

    CARAPI PerformEditorAction(
        /* [in] */ Int32 editorAction,
        /* [out] */ Boolean* flag);

    CARAPI PerformContextMenuAction(
        /* [in] */ Int32 id,
        /* [out] */ Boolean* flag);

    CARAPI SetComposingRegion(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ Boolean* flag);

    CARAPI SetComposingText(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 newCursorPosition,
        /* [out] */ Boolean* flag);

    CARAPI FinishComposingText(
        /* [out] */ Boolean* flag);

    CARAPI BeginBatchEdit(
        /* [out] */ Boolean* flag);

    CARAPI EndBatchEdit(
        /* [out] */ Boolean* flag);

    CARAPI SendKeyEvent(
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* flag);

    CARAPI ClearMetaKeyStates(
        /* [in] */ Int32 states,
        /* [out] */ Boolean* flag);

    CARAPI DeleteSurroundingText(
        /* [in] */ Int32 leftLength,
        /* [in] */ Int32 rightLength,
        /* [out] */ Boolean* flag);

    CARAPI ReportFullscreenMode(
        /* [in] */ Boolean enabled,
        /* [out] */ Boolean* flag);

    CARAPI PerformPrivateCommand(
        /* [in] */ const String& action,
        /* [in] */ IBundle* data,
        /* [out] */ Boolean* flag);

public:
    Mutex mSelfLock;

private:
    static const Int32 MAX_WAIT_TIME_MILLIS = 2000;
    AutoPtr<IInputContextStub> mIInputContext;
};

#endif //_LOCALINPUTCONNECTIONWRAPPER_H__
