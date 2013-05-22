
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
    source->ReadInt64(&mId);
    source->ReadInt32(&mPosition);
    String strText, strLable;
    source->ReadString(&strText);
    source->ReadString(&strLable);

    mText = NULL;
    mLabel = NULL;

    if (!strText.IsNull()) {
        CStringWrapper::New(strText, (ICharSequence**)&mText);
    }

    if (!strLable.IsNull()) {
        CStringWrapper::New(strLable, (ICharSequence**)&mLabel);
    }

    return NOERROR;
}

ECode CCompletionInfo::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    dest->WriteInt64(mId);
    dest->WriteInt32(mPosition);
    String strText, strLable;
    if (mText)
        mText->ToString(&strText);
    if (mLabel)
        mLabel->ToString(&strLable);
    dest->WriteString(strText);
    dest->WriteString(strLable);

    return NOERROR;
}
