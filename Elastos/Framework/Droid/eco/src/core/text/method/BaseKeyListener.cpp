
#include "text/method/BaseKeyListener.h"
#include "text/Selection.h"
#include "text/TextUtils.h"
#include <elastos/Math.h>

using namespace Elastos::Core;

const AutoPtr<IInterface> BaseKeyListener::OLD_SEL_START = MetaKeyKeyListener::NewNoCopySpan();

 /**
 * Performs the action that happens when you press the DEL key in
 * a TextView.  If there is a selection, deletes the selection;
 * otherwise, DEL alone deletes the character before the cursor,
 * if any;
 * ALT+DEL deletes everything on the line the cursor is on.
 *
 * @return TRUE if anything was deleted; FALSE otherwise.
 */
Boolean BaseKeyListener::Backspace(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    Int32 selStart, selEnd;
    Boolean result = TRUE;

    {
        Int32 a = Selection::GetSelectionStart(content);
        Int32 b = Selection::GetSelectionEnd(content);

        selStart = Math::Min(a, b);
        selEnd = Math::Max(a, b);
    }

    if (selStart != selEnd) {
        AutoPtr<IEditable> editable;
        content->Delete(selStart, selEnd, (IEditable**)&editable);
    }
    else if (AltBackspace(view, content, keyCode, event)) {
        result = TRUE;
    }
    else {
        Int32 to = TextUtils::GetOffsetBefore(content, selEnd);

        if (to != selEnd) {
            AutoPtr<IEditable> editable;
            content->Delete(
                Math::Min(to, selEnd), Math::Max(to, selEnd),
                (IEditable**)&editable);
        }
        else {
            result = FALSE;
        }
    }

    if (result)
        AdjustMetaAfterKeypress(content);

    return result;
}

Boolean BaseKeyListener::AltBackspace(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (GetMetaState(content, META_ALT_ON) != 1) {
        return FALSE;
    }

    if (!(ITextView::Probe(view))) {
        return FALSE;
    }

    AutoPtr<ILayout> layout;
    ITextView::Probe(view)->GetLayout((ILayout**)&layout);

    if (layout == NULL) {
        return FALSE;
    }

    Int32 l, start, end;
    layout->GetLineForOffset(Selection::GetSelectionStart(content), &l);
    layout->GetLineStart(l, &start);
    layout->GetLineEnd(l, &end);

    if (end == start) {
        return FALSE;
    }

    AutoPtr<IEditable> editable;
    content->Delete(start, end, (IEditable**)&editable);
    return TRUE;
}

Int32 BaseKeyListener::MakeTextContentType(
    /* [in] */ Capitalize caps,
    /* [in] */ Boolean autoText)
{
    Int32 contentType = InputType_TYPE_CLASS_TEXT;
    switch (caps) {
        case Capitalize_CHARACTERS:
            contentType |= InputType_TYPE_TEXT_FLAG_CAP_CHARACTERS;
            break;
        case Capitalize_WORDS:
            contentType |= InputType_TYPE_TEXT_FLAG_CAP_WORDS;
            break;
        case Capitalize_SENTENCES:
            contentType |= InputType_TYPE_TEXT_FLAG_CAP_SENTENCES;
            break;
    }
    if (autoText) {
        contentType |= InputType_TYPE_TEXT_FLAG_AUTO_CORRECT;
    }
    return contentType;
}

Boolean BaseKeyListener::OnKeyDown(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (keyCode == KeyEvent_KEYCODE_DEL) {
        Backspace(view, content, keyCode, event);
        return TRUE;
    }

    return MetaKeyKeyListener::OnKeyDown(view, content, keyCode, event);
}

/**
 * Base implementation handles ACTION_MULTIPLE KEYCODE_UNKNOWN by inserting
 * the event's text into the content.
 */
Boolean BaseKeyListener::OnKeyOther(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ IKeyEvent* event)
{
    Int32 action, keyCode;
    event->GetAction(&action);
    event->GetKeyCode(&keyCode);
    if (action != KeyEvent_ACTION_MULTIPLE
        || keyCode != KeyEvent_KEYCODE_UNKNOWN) {
        // Not something we are interested in.
        return FALSE;
    }

    Int32 selStart, selEnd;

    {
        Int32 a = Selection::GetSelectionStart(content);
        Int32 b = Selection::GetSelectionEnd(content);

        selStart = Math::Min(a, b);
        selEnd = Math::Max(a, b);
    }

    String str;
    event->GetCharacters(&str);
    if (str.IsNull()) {
        return FALSE;
    }

    AutoPtr<ICharSequence> text;
    CStringWrapper::New(str, (ICharSequence**)&text);
    AutoPtr<IEditable> editable;
    content->ReplaceEx(selStart, selEnd, text, (IEditable**)&editable);
    return TRUE;
}
