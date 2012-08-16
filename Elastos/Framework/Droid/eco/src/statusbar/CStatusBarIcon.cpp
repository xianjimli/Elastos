
#include "statusbar/CStatusBarIcon.h"

CStatusBarIcon::CStatusBarIcon()
    : mVisible(TRUE)
{}

ECode CStatusBarIcon::Clone(
    /* [out] */ IStatusBarIcon** icon)
{
    CStatusBarIcon* _icon;
    CStatusBarIcon::NewByFriend(mIconCapsule, mIconId, mIconLevel, &_icon);
    _icon->mVisible = mVisible;
    _icon->mNumber = mNumber;

    *icon = (IStatusBarIcon*)_icon;
    return NOERROR;
}

ECode CStatusBarIcon::GetDescription(
    /* [out] */ String* str)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStatusBarIcon::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadString(&mIconCapsule);
    source->ReadInt32(&mIconId);
    source->ReadInt32(&mIconLevel);
    source->ReadBoolean(&mVisible);
    source->ReadInt32(&mNumber);
    return NOERROR;
}

ECode CStatusBarIcon::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteString(mIconCapsule);
    dest->WriteInt32(mIconId);
    dest->WriteInt32(mIconLevel);
    dest->WriteBoolean(mVisible);
    dest->WriteInt32(mNumber);
    return NOERROR;
}

ECode CStatusBarIcon::constructor()
{
    return NOERROR;
}

ECode CStatusBarIcon::constructor(
    /* [in] */ const String& iconCapsule,
    /* [in] */ Int32 iconId,
    /* [in] */ Int32 iconLevel)
{
    mIconCapsule = iconCapsule;
    mIconId = iconId;
    mIconLevel = iconLevel;

    return NOERROR;
}

ECode CStatusBarIcon::constructor(
    /* [in] */ const String& iconCapsule,
    /* [in] */ Int32 iconId,
    /* [in] */ Int32 iconLevel,
    /* [in] */ Int32 number)
{
    mIconCapsule = iconCapsule;
    mIconId = iconId;
    mIconLevel = iconLevel;
    mNumber = number;

    return NOERROR;
}

ECode CStatusBarIcon::constructor(
    /* [in] */ IParcel* source)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

