
#include "ext/frameworkdef.h"
#include "statusbar/CStatusBarIconList.h"

CStatusBarIconList::CStatusBarIconList()
    : mIcons(NULL)
{}

CStatusBarIconList::~CStatusBarIconList()
{
    if (mIcons != NULL) {
        for (Int32 i = 0; i < mIcons->GetLength(); ++i) {
            (*mIcons)[i] = NULL;
        }
        ArrayOf< AutoPtr<IStatusBarIcon> >::Free(mIcons);
    }
}

ECode CStatusBarIconList::GetDescription(
    /* [out] */ String* str)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CStatusBarIconList::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadArrayOfString((Handle32*)&mSlots);
    Int32 N;
    source->ReadInt32(&N);
    if (N < 0) {
        mIcons = NULL;
    }
    else {
        mIcons = ArrayOf< AutoPtr<IStatusBarIcon> >::Alloc(N);
        for (Int32 i = 0; i < N; i++) {
            AutoPtr<IInterface> obj;
            source->ReadInterfacePtr((Handle32*)&obj);
            (*mIcons)[i] = obj != NULL ? NULL : IStatusBarIcon::Probe(obj);
        }
    }
    return NOERROR;
}

ECode CStatusBarIconList::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteArrayOfString(*mSlots);
    if (mIcons == NULL) {
        dest->WriteInt32(-1);
    }
    else {
        const Int32 N = mIcons->GetLength();
        dest->WriteInt32(N);
        for (Int32 i = 0; i < N; i++) {
            IStatusBarIcon* ic = (*mIcons)[i];
            dest->WriteInterfacePtr(ic);
        }
    }
    return NOERROR;
}

ECode CStatusBarIconList::DefineSlots(
    /* [in] */ const ArrayOf<String>& slots)
{
    const Int32 N = slots.GetLength();
    mSlots = ArrayOf<String>::Alloc(N);
    for (Int32 i = 0; i < N; i++) {
        (*mSlots)[i] = slots[i];
    }
    mIcons = ArrayOf< AutoPtr<IStatusBarIcon> >::Alloc(N);

    return NOERROR;
}

ECode CStatusBarIconList::GetSlotIndex(
    /* [in] */ const String& slot,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    const Int32 N = mSlots->GetLength();
    for (Int32 i = 0; i < N; i++) {
        if (slot.Equals((*mSlots)[i])) {
            *index = i;
            return NOERROR;
        }
    }
    *index = -1;
    return NOERROR;
}

ECode CStatusBarIconList::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);

    *size = mSlots->GetLength();

    return NOERROR;
}

ECode CStatusBarIconList::SetIcon(
    /* [in] */ Int32 index,
    /* [in] */ IStatusBarIcon* icon)
{
    icon->Clone((IStatusBarIcon**)&(*mIcons)[index]);
    return NOERROR;
}

ECode CStatusBarIconList::RemoveIcon(
    /* [in] */ Int32 index)
{
    (*mIcons)[index] = NULL;
    return NOERROR;
}

ECode CStatusBarIconList::GetSlot(
    /* [in] */ Int32 index,
    /* [out] */ String* slot)
{
    VALIDATE_NOT_NULL(slot);

    *slot = (*mSlots)[index];
    return NOERROR;
}

ECode CStatusBarIconList::GetIcon(
    /* [in] */ Int32 index,
    /* [out] */ IStatusBarIcon** icon)
{
    VALIDATE_NOT_NULL(icon);

    *icon = (*mIcons)[index];
    if (*icon != NULL) (*icon)->AddRef();

    return NOERROR;
}

ECode CStatusBarIconList::GetViewIndex(
    /* [in] */ Int32 index,
    /* [out] */ Int32* viewIndex)
{
    VALIDATE_NOT_NULL(viewIndex);

    Int32 count = 0;
    for (Int32 i = 0; i < index; i++) {
        if ((*mIcons)[i] != NULL) {
            count++;
        }
    }
    *viewIndex = count;
    return NOERROR;
}

ECode CStatusBarIconList::CopyFrom(
    /* [in] */ IStatusBarIconList* list)
{
    CStatusBarIconList* that = (CStatusBarIconList*)list;

    if (that->mSlots == NULL) {
        mSlots = NULL;
        mIcons = NULL;
    }
    else {
        const Int32 N = that->mSlots->GetLength();
        mSlots = ArrayOf<String>::Alloc(N);
        mIcons = ArrayOf< AutoPtr<IStatusBarIcon> >::Alloc(N);
        for (Int32 i = 0; i < N; i++) {
            (*mSlots)[i] = (*that->mSlots)[i];
            (*mIcons)[i] = NULL;
            if ((*that->mIcons)[i] != NULL) {
                (*that->mIcons)[i]->Clone((IStatusBarIcon**)&(*mIcons)[i]);
            }
        }
    }
    return NOERROR;
}

ECode CStatusBarIconList::constructor()
{
    return NOERROR;
}

ECode CStatusBarIconList::constructor(
    /* [in] */ IParcel* source)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

