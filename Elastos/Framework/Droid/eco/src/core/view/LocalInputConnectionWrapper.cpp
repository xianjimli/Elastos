
#include "view/LocalInputConnectionWrapper.h"
#include "os/SystemClock.h"

//TODO
// AutoPtr<LocalInputConnectionWrapper::InputContextCallback> LocalInputConnectionWrapper::InputContextCallback::sInstance =
// 		new InputContextCallback();

LocalInputConnectionWrapper* LocalInputConnectionWrapper::InputContextCallback::mHost;
Mutex LocalInputConnectionWrapper::InputContextCallback::mPriLock;
Int32 LocalInputConnectionWrapper::InputContextCallback::sSequenceNumber = 1;
const Int32 LocalInputConnectionWrapper::MAX_WAIT_TIME_MILLIS;



LocalInputConnectionWrapper::InputContextCallback::InputContextCallback():
	mSeq(0),
	mHaveValue(FALSE),
	mCursorCapsMode(0)
{

}

LocalInputConnectionWrapper::InputContextCallback::~InputContextCallback()
{

}

PInterface LocalInputConnectionWrapper::InputContextCallback::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInputContextCallbackStub == riid) {
        return (IInputContextCallbackStub *)this;
    }

    return NULL;
}

UInt32 LocalInputConnectionWrapper::InputContextCallback::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 LocalInputConnectionWrapper::InputContextCallback::Release()
{
    return ElRefBase::Release();
}

ECode LocalInputConnectionWrapper::InputContextCallback::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface*)(IInputContextCallbackStub*)this) {
        *pIID = EIID_IInputContextCallbackStub;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode LocalInputConnectionWrapper::InputContextCallback::SetTextBeforeCursor(
    /* [in] */ ICharSequence* textBeforeCursor,
    /* [in] */ Int32 seq)
{
	Mutex::Autolock lock(mHost->mSelfLock);
    if (seq == mSeq) {
        mTextBeforeCursor = textBeforeCursor;
        mHaveValue = TRUE;

        //TODO
        //notifyAll();
    } else {
        // Log.i(TAG, "Got out-of-sequence callback " + seq + " (expected " + mSeq
        //         + ") in setTextBeforeCursor, ignoring.");
    }

    return NOERROR;
}

ECode LocalInputConnectionWrapper::InputContextCallback::SetTextAfterCursor(
    /* [in] */ ICharSequence* textAfterCursor,
    /* [in] */ Int32 seq)
{
    Mutex::Autolock lock(mHost->mSelfLock);
    if (seq == mSeq) {
        mTextAfterCursor = textAfterCursor;
        mHaveValue = TRUE;

        //TODO
        //notifyAll();
    } else {
        // Log.i(TAG, "Got out-of-sequence callback " + seq + " (expected " + mSeq
        //         + ") in setTextAfterCursor, ignoring.");
    }

    return NOERROR;
}

ECode LocalInputConnectionWrapper::InputContextCallback::SetSelectedText(
    /* [in] */ ICharSequence* selectedText,
    /* [in] */ Int32 seq)
{
    Mutex::Autolock lock(mHost->mSelfLock);
    if (seq == mSeq) {
        mSelectedText = selectedText;
        mHaveValue = TRUE;

        //TODO
        //notifyAll();
    } else {
        // Log.i(TAG, "Got out-of-sequence callback " + seq + " (expected " + mSeq
        //         + ") in setSelectedText, ignoring.");
    }

    return NOERROR;
}

ECode LocalInputConnectionWrapper::InputContextCallback::SetCursorCapsMode(
    /* [in] */ Int32 capsMode,
    /* [in] */ Int32 seq)
{
    Mutex::Autolock lock(mHost->mSelfLock);
    if (seq == mSeq) {
        mCursorCapsMode = capsMode;
        mHaveValue = TRUE;

        //TODO
        //notifyAll();
    } else {
        // Log.i(TAG, "Got out-of-sequence callback " + seq + " (expected " + mSeq
        //         + ") in setCursorCapsMode, ignoring.");
    }

    return NOERROR;
}

ECode LocalInputConnectionWrapper::InputContextCallback::SetExtractedText(
    /* [in] */ IExtractedText* extractedText,
    /* [in] */ Int32 seq)
{
    Mutex::Autolock lock(mHost->mSelfLock);
    if (seq == mSeq) {
        mExtractedText = extractedText;
        mHaveValue = TRUE;

        //TODO
        //notifyAll();
    } else {
        // Log.i(TAG, "Got out-of-sequence callback " + seq + " (expected " + mSeq
        //         + ") in setExtractedText, ignoring.");
    }

    return NOERROR;
}

ECode LocalInputConnectionWrapper::InputContextCallback::GetDescription(
    /* [out] */ String* str)
{
    return E_NOT_IMPLEMENTED;
}

AutoPtr<LocalInputConnectionWrapper::InputContextCallback> LocalInputConnectionWrapper::InputContextCallback::GetInstance(
	/* [in] */ LocalInputConnectionWrapper* host)
{
	mHost = host;
    Mutex::Autolock lock(mPriLock);
    // Return sInstance if it's non-NULL, otherwise construct a new callback
    AutoPtr<InputContextCallback> callback;
    if (sInstance != NULL) {
        callback = sInstance;
        sInstance = NULL;

        // Reset the callback
        callback->mHaveValue = FALSE;
    } else {
        callback = new InputContextCallback();
    }

    // Set the sequence number
    callback->mSeq = sSequenceNumber++;
    return callback;
}

void LocalInputConnectionWrapper::InputContextCallback::WaitForResultLocked()
{
    long startTime = SystemClock::UptimeMillis();
    long endTime = startTime + MAX_WAIT_TIME_MILLIS;

    while (!mHaveValue) {
        long remainingTime = endTime - SystemClock::UptimeMillis();
        if (remainingTime <= 0) {
            // Log.w(TAG, "Timed out waiting on IInputContextCallback");
            return;
        }
        // try {
        // 	//TODO
        //     wait(remainingTime);
        // } catch (InterruptedException e) {
        // }
    }
}

/**
 * Makes the given InputContextCallback available for use in the future.
 */
void LocalInputConnectionWrapper::InputContextCallback::Dispose()
{
    Mutex::Autolock lock(mPriLock);
    // If sInstance is non-NULL, just let this object be garbage-collected
    if (sInstance == NULL) {
        // Allow any objects being held to be gc'ed
        mTextAfterCursor = NULL;
        mTextBeforeCursor = NULL;
        mExtractedText = NULL;
        sInstance = this;
    }
}

LocalInputConnectionWrapper::LocalInputConnectionWrapper(
    /* [in] */ IInputContextStub* inputContext):
    mIInputContext(inputContext)
{
}

LocalInputConnectionWrapper::~LocalInputConnectionWrapper()
{
}

PInterface LocalInputConnectionWrapper::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInputConnection == riid) {
        return (IInputConnection *)this;
    }

    return NULL;
}

UInt32 LocalInputConnectionWrapper::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 LocalInputConnectionWrapper::Release()
{
    return ElRefBase::Release();
}

ECode LocalInputConnectionWrapper::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface*)(IInputConnection*)this) {
        *pIID = EIID_IInputConnection;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode LocalInputConnectionWrapper::GetTextAfterCursor(
    /* [in] */ Int32 n,
    /* [in] */ Int32 flags,
    /* [out] */ ICharSequence** text)
{
	assert(text != NULL);
	*text = NULL;
    AutoPtr<InputContextCallback> callback = InputContextCallback::GetInstance(this);
    mIInputContext->GetTextAfterCursor(n, flags, callback->mSeq, callback);
    {
    	Mutex::Autolock lock(callback->mPriLock);
        callback->WaitForResultLocked();
        if (callback->mHaveValue) {
            *text = callback->mTextAfterCursor;
        }
    }

    callback->Dispose();
    // catch (RemoteException e) {
    //     return NULL;
    // }
    if (*text != NULL) {
    	(*text)->AddRef();
    }

    return NOERROR;
}

ECode LocalInputConnectionWrapper::GetTextBeforeCursor(
    /* [in] */ Int32 n,
    /* [in] */ Int32 flags,
    /* [out] */ ICharSequence** text)
{
	assert(text != NULL);
	*text = NULL;
    AutoPtr<InputContextCallback> callback = InputContextCallback::GetInstance(this);
    mIInputContext->GetTextBeforeCursor(n, flags, callback->mSeq, callback);
    {
    	Mutex::Autolock lock(callback->mPriLock);
        callback->WaitForResultLocked();
        if (callback->mHaveValue) {
            *text = callback->mTextBeforeCursor;
        }
    }

    callback->Dispose();
    // } catch (RemoteException e) {
    //     return NULL;
    // }
    if (*text != NULL) {
    	(*text)->AddRef();
    }

    return NOERROR;
}

ECode LocalInputConnectionWrapper::GetSelectedText(
    /* [in] */ Int32 flags,
    /* [out] */ ICharSequence** text)
{
	assert(text != NULL);
	*text = NULL;
    AutoPtr<InputContextCallback> callback = InputContextCallback::GetInstance(this);
    mIInputContext->GetSelectedText(flags, callback->mSeq, callback);
    {
    	Mutex::Autolock lock(callback->mPriLock);
        callback->WaitForResultLocked();
        if (callback->mHaveValue) {
            *text = callback->mSelectedText;
        }
    }

    callback->Dispose();
    // } catch (RemoteException e) {
    //     return NULL;
    // }
    if (*text != NULL) {
    	(*text)->AddRef();
    }

    return NOERROR;
}

ECode LocalInputConnectionWrapper::GetCursorCapsMode(
    /* [in] */ Int32 reqModes,
    /* [out] */ Int32* capsMode)
{
	assert(capsMode != NULL);
	*capsMode = 0;

    AutoPtr<InputContextCallback> callback = InputContextCallback::GetInstance(this);
    mIInputContext->GetCursorCapsMode(reqModes, callback->mSeq, callback);
    {
    	Mutex::Autolock lock(callback->mPriLock);
        callback->WaitForResultLocked();
        if (callback->mHaveValue) {
            *capsMode = callback->mCursorCapsMode;
        }
    }

    callback->Dispose();
    // } catch (RemoteException e) {
    //     return 0;
    // }
    return NOERROR;
}

ECode LocalInputConnectionWrapper::GetExtractedText(
    /* [in] */ IExtractedTextRequest* request,
    /* [in] */ Int32 flags,
    /* [out] */ IExtractedText** extractedText)
{
	assert(extractedText != NULL);
	*extractedText = NULL;

    AutoPtr<InputContextCallback> callback = InputContextCallback::GetInstance(this);
    mIInputContext->GetExtractedText(request, flags, callback->mSeq, callback);
    {
    	Mutex::Autolock lock(callback->mPriLock);
        callback->WaitForResultLocked();
        if (callback->mHaveValue) {
            *extractedText = callback->mExtractedText;
        }
    }

    callback->Dispose();
    // } catch (RemoteException e) {
    //     return NULL;
    // }

    if (*extractedText != NULL) {
    	(*extractedText)->AddRef();
    }

    return NOERROR;
}

ECode LocalInputConnectionWrapper::CommitText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 newCursorPosition,
    /* [out] */ Boolean* flag)
{
	assert(flag != NULL);
	*flag = FALSE;

    mIInputContext->CommitText(text, newCursorPosition);
    *flag = TRUE;
    return NOERROR;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::CommitCompletion(
    /* [in] */ ICompletionInfo* text,
    /* [out] */ Boolean* flag)
{
	assert(flag != NULL);
	*flag = FALSE;

    mIInputContext->CommitCompletion(text);
    *flag = TRUE;
    return NOERROR;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::SetSelection(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ Boolean* flag)
{
	assert(flag != NULL);
	*flag = FALSE;

    mIInputContext->SetSelection(start, end);
    *flag = TRUE;
    return NOERROR;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::PerformEditorAction(
    /* [in] */ Int32 editorAction,
    /* [out] */ Boolean* flag)
{
	assert(flag != NULL);
	*flag = FALSE;

    mIInputContext->PerformEditorAction(editorAction);
    *flag = TRUE;
    return NOERROR;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::PerformContextMenuAction(
    /* [in] */ Int32 id,
    /* [out] */ Boolean* flag)
{
	assert(flag != NULL);
	*flag = FALSE;

    mIInputContext->PerformContextMenuAction(id);
    *flag = TRUE;
    return NOERROR;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::SetComposingRegion(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ Boolean* flag)
{
	assert(flag != NULL);
	*flag = FALSE;

    mIInputContext->SetComposingRegion(start, end);
    *flag = TRUE;
    return NOERROR;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::SetComposingText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 newCursorPosition,
    /* [out] */ Boolean* flag)
{
	assert(flag != NULL);
	*flag = FALSE;

    mIInputContext->SetComposingText(text, newCursorPosition);
    *flag = TRUE;
    return NOERROR;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::FinishComposingText(
    /* [out] */ Boolean* flag)
{
	assert(flag != NULL);
	*flag = FALSE;

    mIInputContext->FinishComposingText();
    *flag = TRUE;
    return NOERROR;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::BeginBatchEdit(
    /* [out] */ Boolean* flag)
{
	assert(flag != NULL);
	*flag = FALSE;

    mIInputContext->BeginBatchEdit();
    *flag = TRUE;
    return NOERROR;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::EndBatchEdit(
    /* [out] */ Boolean* flag)
{
	assert(flag != NULL);
	*flag = FALSE;

    mIInputContext->EndBatchEdit();
    *flag = TRUE;
    return NOERROR;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::SendKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* flag)
{
	assert(flag != NULL);
	*flag = FALSE;

    mIInputContext->SendKeyEvent(event);
    *flag = TRUE;
    return NOERROR;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::ClearMetaKeyStates(
    /* [in] */ Int32 states,
    /* [out] */ Boolean* flag)
{
	assert(flag != NULL);
	*flag = FALSE;

    mIInputContext->ClearMetaKeyStates(states);
    *flag = TRUE;
    return NOERROR;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::DeleteSurroundingText(
    /* [in] */ Int32 leftLength,
    /* [in] */ Int32 rightLength,
    /* [out] */ Boolean* flag)
{
	assert(flag != NULL);
	*flag = FALSE;

    mIInputContext->DeleteSurroundingText(leftLength, rightLength);
    *flag = TRUE;
    return NOERROR;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::ReportFullscreenMode(
    /* [in] */ Boolean enabled,
    /* [out] */ Boolean* flag)
{
	assert(flag != NULL);
	*flag = FALSE;

    mIInputContext->ReportFullscreenMode(enabled);
    *flag = TRUE;
    return NOERROR;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode LocalInputConnectionWrapper::PerformPrivateCommand(
    /* [in] */ const String& action,
    /* [in] */ IBundle* data,
    /* [out] */ Boolean* flag)
{
	assert(flag != NULL);
	*flag = FALSE;

    mIInputContext->PerformPrivateCommand(action, data);
    *flag = TRUE;
    return NOERROR;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}
