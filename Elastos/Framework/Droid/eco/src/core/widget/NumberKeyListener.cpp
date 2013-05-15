
#include "widget/NumberKeyListener.h"
#include "text/CSpannableStringBuilder.h"
#include "text/Selection.h"
#include <elastos/Math.h>
#include <elastos/AutoFree.h>

using namespace Elastos::Core;

PInterface NumberKeyListener::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IInputFilter*)this;
    }
    else if (riid == EIID_IInputFilter) {
        return (IInputFilter*)this;
    }

    return NULL;
}

UInt32 NumberKeyListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 NumberKeyListener::Release()
{
    return ElRefBase::Release();
}

ECode NumberKeyListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

Int32 NumberKeyListener::Lookup(
    /* [in] */ IKeyEvent* event,
    /* [in] */ ISpannable* content)
{
    Char16 res;
    ArrayOf<Char32>* cs = GetAcceptedChars();

    AutoFree< ArrayOf<Char16> > char16Array
            = ArrayOf<Char16>::Alloc(cs->GetLength());
    for (Int32 i = 0; i < cs->GetLength(); i++) {
        (*char16Array)[i] = (*cs)[i];
    }
    event->GetMatchEx(*char16Array, GetMetaState(content), &res);

    return res;
}

ECode NumberKeyListener::Filter(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ISpanned* dest,
    /* [in] */ Int32 dstart,
    /* [in] */ Int32 dend,
    /*[out] */ ICharSequence** cs)
{
    VALIDATE_NOT_NULL(cs);

    ArrayOf<Char32>* accept = GetAcceptedChars();

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
        *cs = NULL;
        return NOERROR;
    }

    if (end - start == 1) {
        // It was not OK, and there is only one char, so nothing remains.
        return CStringWrapper::New(String(""), cs);
    }

    AutoPtr<ISpannableStringBuilder> filtered;
    CSpannableStringBuilder::New(
            source, start, end, (ISpannableStringBuilder**)&filtered);
    i -= start;
    end -= start;

    // Only count down to i because the chars before that were all OK.
    for (Int32 j = end - 1; j >= i; j--) {
        Char32 c;
        source->GetCharAt(j, &c);
        if (!Ok(accept, c)) {
            AutoPtr<IEditable> editable;
            filtered->Delete(j, j + 1, (IEditable**)&editable);
        }
    }

    *cs = (ICharSequence*)filtered->Probe(EIID_ICharSequence);
    (*cs)->AddRef();
    return NOERROR;
}

Boolean NumberKeyListener::Ok(
    /* [in] */ ArrayOf<Char32>* accept,
    /* [in] */ Char32 c)
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
            AutoPtr<IEditable> editable;
            String s = String::FromInt32(i);
            AutoPtr<ICharSequence> cs;
            CStringWrapper::New(s, (ICharSequence**)&cs);
            content->ReplaceEx(selStart, selEnd, cs, (IEditable**)&editable);

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
