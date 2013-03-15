
#ifndef  __EDITABLEINPUTCONNECTION_H__
#define  __EDITABLEINPUTCONNECTION_H__

#include "view/inputmethod/BaseInputConnection.h"
#include "view/inputmethod/BaseInputConnectionMacro.h"
#include <elastos/AutoPtr.h>

class EditableInputConnection : public BaseInputConnection
{
public:
    CARAPI_(AutoPtr<IEditable>) GetEditable();

    CARAPI_(Boolean) BeginBatchEdit();

    CARAPI_(Boolean) EndBatchEdit();

    CARAPI_(Boolean) ClearMetaKeyStates(
        /* [in] */ Int32 states);

    CARAPI_(Boolean) CommitCompletion(
        /* [in] */ ICompletionInfo* text);

    CARAPI_(Boolean) PerformEditorAction(
        /* [in] */ Int32 actionCode);

    CARAPI_(Boolean) PerformContextMenuAction(
        /* [in] */ Int32 id);

    CARAPI_(AutoPtr<IExtractedText>) GetExtractedText(
        /* [in] */ IExtractedTextRequest* request,
        /* [in] */ Int32 flags);

    CARAPI_(Boolean) PerformPrivateCommand(
        /* [in] */ const String& action,
        /* [in] */ IBundle* data);

    CARAPI_(Boolean) CommitText(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 newCursorPosition);

protected:
    // private static final boolean DEBUG = false;
    // private static final String TAG = "EditableInputConnection";

    AutoPtr<ITextView> mTextView;
};

#endif  //__EDITABLEINPUTCONNECTION_H__
