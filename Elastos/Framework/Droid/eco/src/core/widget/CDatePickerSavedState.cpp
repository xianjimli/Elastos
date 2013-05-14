
#include "widget/CDatePickerSavedState.h"

CDatePickerSavedState::CDatePickerSavedState()
    : mYear(0)
    , mMonth(0)
    , mDay(0)
{}

Int32 CDatePickerSavedState::GetYear()
{
    return mYear;
}

Int32 CDatePickerSavedState::GetMonth()
{
    return mMonth;
}

Int32 CDatePickerSavedState::GetDay()
{
    return mDay;
}

ECode CDatePickerSavedState::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    ViewBaseSavedState::ReadFromParcel(source);
    source->ReadInt32(&mYear);
    source->ReadInt32(&mMonth);
    source->ReadInt32(&mDay);
    return NOERROR;
}

ECode CDatePickerSavedState::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    ViewBaseSavedState::WriteToParcel(dest);
    dest->WriteInt32(mYear);
    dest->WriteInt32(mMonth);
    dest->WriteInt32(mDay);
    return NOERROR;
}

ECode CDatePickerSavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    AutoPtr<IParcelable> p = ViewBaseSavedState::GetSuperState();
    if (p != NULL) {
        *superState = p;
        (*superState)->AddRef();
    }
    else {
        (*superState) = NULL;
    }

    return NOERROR;
}

ECode CDatePickerSavedState::constructor(
    /* [in] */ IParcelable* superState,
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 day)
{
    ViewBaseSavedState::Init(superState);
    mYear = year;
    mMonth = month;
    mDay = day;
    return NOERROR;
}
