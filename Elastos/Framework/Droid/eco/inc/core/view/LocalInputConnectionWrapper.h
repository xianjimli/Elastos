
#ifndef  __LOCALINPUTCONNECTIONWRAPPER_H__
#define  __LOCALINPUTCONNECTIONWRAPPER_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>

using namespace Elastos::Core::Threading;

class LocalInputConnectionWrapper
    : public ElRefBase
    , public IInputConnection
{
    friend class CInputContextCallback;

public:
    LocalInputConnectionWrapper(
        /* [in] */ IInputContext* inputContext);

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
    AutoPtr<IInputContext> mIInputContext;
};

#endif //__LOCALINPUTCONNECTIONWRAPPER_H__
