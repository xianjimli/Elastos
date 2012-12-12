
#include "widget/NumberKeyListener.h"
#include "text/CSpannableStringBuilder.h"
#include "text/Selection.h"
#include <elastos/Math.h>

using namespace Elastos::Core;

Int32 NumberKeyListener::Lookup(
    /* [in] */ IKeyEvent* event, 
    /* [in] */ ISpannable* content)
{
    Int32 res;
    event->GetMatch(GetAcceptedChars(), GetMetaState(content), &res);

    return res;
}

AutoPtr<ICharSequence> NumberKeyListener::Filter(
    /* [in] */ ICharSequence* source, 
    /* [in] */ Int32 start, 
    /* [in] */ Int32 end,
    /* [in] */ ISpanned* dest, 
    /* [in] */ Int32 dstart, 
    /* [in] */ Int32 dend)
{
    ArrayOf<Char16>* accept = GetAcceptedChars();
    Boolean filter = FALSE;

    Int32 i;
    for (i = start; i < end; i++) {
        Char32 c;
        source->GetCharAt(i, &c);
        if (!Ok(accept, c)) {
            break;
        }
    }

    if (i == end) {
        // It was all OK.
        return NULL;
    }

    if (end - start == 1) {
        // It was not OK, and there is only one Char16, so nothing remains.
        return "";
    }

    AutoPtr<ISpannableStringBuilder> filtered;
    CSpannableStringBuilder::New(source, start, end, (ISpannableStringBuilder**)&filtered);
    i -= start;
    end -= start;

    Int32 len = end - start;
    // Only count down to i because the chars before that were all OK.
    for (Int32 j = end - 1; j >= i; j--) {
        Char32 c;
        source->GetCharAt(j, &c);
        if (!Ok(accept, c)) {
            filtered->Delete(j, j + 1);
        }
    }

    return filtered;
}

Boolean NumberKeyListener::Ok(
    /* [in] */ ArrayOf<Char16>* accept, 
    /* [in] */ Char16 c)
{
    for (Int32 i = accept->GetLength() - 1; i >= 0; i--) {
        if ((*accept)[i] == c) {
            return TRUE;
        }
    }

    return FALSE;
}

Boolean NumberKeyListener::OnKeyDown(
    /* [in] */ IView* view, 
    /* [in] */ IEditable* content,
    /* [in] */ Int32 keyCode, 
    /* [in] */ IKeyEvent* event) 
{
    Int32 selStart, selEnd;

    {
        Int32 a = Selection::GetSelectionStart(content);
        Int32 b = Selection::GetSelectionEnd(content);

        selStart = Math::Min(a, b);
        selEnd = Math::Max(a, b);
    }

    if (selStart < 0 || selEnd < 0) {
        selStart = selEnd = 0;
        Selection::SetSelection(content, 0);
    }

    Int32 i = event != NULL ? Lookup(event, content) : 0;

    Int32 count;
    Int32 repeatCount = event != NULL ? (event->GetRepeatCount(&count), count) : 0;
    if (repeatCount == 0) {
        if (i != 0) {
            if (selStart != selEnd) {
                Selection::SetSelection(content, selEnd);
            }

            content->Replace(selStart, selEnd, String.valueOf((Char16) i));

            AdjustMetaAfterKeypress(content);
            return TRUE;
        }
    } else if (i == '0' && repeatCount == 1) {
        // Pretty hackish, it replaces the 0 with the +

        Char32 c;
        content->GetCharAt(selStart - 1, &c);
        if (selStart == selEnd && selEnd > 0 &&
                c == '0') {
            //content->Replace(selStart - 1, selEnd, String::ValueOf('+'));
            AdjustMetaAfterKeypress(content);
            return TRUE;
        }
    }

    AdjustMetaAfterKeypress(content);
    return BaseKeyListener::OnKeyDown(view, content, keyCode, event);
}
