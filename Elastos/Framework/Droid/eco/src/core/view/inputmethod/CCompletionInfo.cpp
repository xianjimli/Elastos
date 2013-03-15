
#include "ext/frameworkext.h"
#include "view/inputmethod/CCompletionInfo.h"


CCompletionInfo::CCompletionInfo()
    : mId(0)
    , mPosition(0)
{}

ECode CCompletionInfo::constructor()
{
    return NOERROR;
}

ECode CCompletionInfo::constructor(
    /* [in] */ Int64 id,
    /* [in] */ Int32 index,
    /* [in] */ ICharSequence* text)
{
    mId = id;
    mPosition = index;
    mText = text;
    mLabel = NULL;
    return NOERROR;
}

ECode CCompletionInfo::constructor(
    /* [in] */ Int64 id,
    /* [in] */ Int32 index,
    /* [in] */ ICharSequence* text,
    /* [in] */ ICharSequence* label)
{
    mId = id;
    mPosition = index;
    mText = text;
    mLabel = label;
    return NOERROR;
}

ECode CCompletionInfo::GetId(
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mId;
    return NOERROR;
}

ECode CCompletionInfo::GetPosition(
    /* [out] */ Int32* pos)
{
    VALIDATE_NOT_NULL(pos);
    *pos = mPosition;
    return NOERROR;
}

ECode CCompletionInfo::GetText(
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text);
    *text = mText;
    if (*text != NULL) {
        (*text)->AddRef();
    }

    return NOERROR;
}

ECode CCompletionInfo::GetLabel(
    /* [out] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    *label = mLabel;
    if (*label != NULL) {
        (*label)->AddRef();
    }

    return NOERROR;
}

ECode CCompletionInfo::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CCompletionInfo::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}
