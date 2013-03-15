
#include "ext/frameworkdef.h"
#include "view/inputmethod/CEditorInfo.h"

CEditorInfo::CEditorInfo()
    : mInputType(InputType_TYPE_NULL)
    , mImeOptions(EditorInfo_IME_NULL)
    , mActionId(0)
    , mInitialSelStart(-1)
    , mInitialSelEnd(-1)
    , mInitialCapsMode(0)
    , mFieldId(0)
{}

ECode CEditorInfo::constructor()
{
    return NOERROR;
}

ECode CEditorInfo::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    source->ReadInt32(&mInputType);
    source->ReadInt32(&mImeOptions);
    source->ReadString(&mPrivateImeOptions);
    // res.actionLabel = TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel(source);
    source->ReadInt32(&mActionId);
    source->ReadInt32(&mInitialSelStart);
    source->ReadInt32(&mInitialSelEnd);
    source->ReadInt32(&mInitialCapsMode);
    // res.hintText = TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel(source);
    // res.label = TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel(source);
    source->ReadString(&mCapsuleName);
    source->ReadInt32(&mFieldId);
    source->ReadString(&mFieldName);
    // dest.writeBundle(extras);
    return NOERROR;
}

ECode CEditorInfo::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    dest->WriteInt32(mInputType);
    dest->WriteInt32(mImeOptions);
    dest->WriteString(mPrivateImeOptions);
    // TextUtils.writeToParcel(actionLabel, dest, flags);
    dest->WriteInt32(mActionId);
    dest->WriteInt32(mInitialSelStart);
    dest->WriteInt32(mInitialSelEnd);
    dest->WriteInt32(mInitialCapsMode);
    // TextUtils.writeToParcel(hintText, dest, flags);
    // TextUtils.writeToParcel(label, dest, flags);
    dest->WriteString(mCapsuleName);
    dest->WriteInt32(mFieldId);
    dest->WriteString(mFieldName);
    // dest.writeBundle(extras);
    return NOERROR;
}
ECode CEditorInfo::GetInputType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mInputType;
    return NOERROR;
}

ECode CEditorInfo::GetImeOptions(
    /* [out] */ Int32* options)
{
    VALIDATE_NOT_NULL(options);
    *options = mImeOptions;
    return NOERROR;
}

ECode CEditorInfo::GetActionLabel(
    /* [out] */ ICharSequence** actionLabel)
{
    VALIDATE_NOT_NULL(actionLabel);
    *actionLabel = mActionLabel;
    if (*actionLabel != NULL) (*actionLabel)->AddRef();
    return NOERROR;
}

ECode CEditorInfo::GetActionId(
    /* [out] */ Int32* actionId)
{
    VALIDATE_NOT_NULL(actionId);
    *actionId = mActionId;
    return NOERROR;
}

ECode CEditorInfo::GetHintText(
    /* [out] */ ICharSequence** hintText)
{
    VALIDATE_NOT_NULL(hintText);
    *hintText = mHintText;
    if (*hintText != NULL) (*hintText)->AddRef();
    return NOERROR;
}
