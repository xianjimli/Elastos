
#ifndef __CSTATUSBARICONLIST_H__
#define __CSTATUSBARICONLIST_H__

#include "_CStatusBarIconList.h"
#include "utils/AutoStringArray.h"
#include <elastos/AutoPtr.h>

using namespace Elastos;

CarClass(CStatusBarIconList)
{
public:
    CStatusBarIconList();

    ~CStatusBarIconList();

    CARAPI DefineSlots(
        /* [in] */ const ArrayOf<String>& slots);

    CARAPI GetSlotIndex(
        /* [in] */ const String& slot,
        /* [out] */ Int32* index);

    CARAPI GetSize(
        /* [out] */ Int32* size);

    CARAPI SetIcon(
        /* [in] */ Int32 index,
        /* [in] */ IStatusBarIcon* icon);

    CARAPI RemoveIcon(
        /* [in] */ Int32 index);

    CARAPI GetSlot(
        /* [in] */ Int32 index,
        /* [out] */ String* slot);

    CARAPI GetIcon(
        /* [in] */ Int32 index,
        /* [out] */ IStatusBarIcon** icon);

    CARAPI GetViewIndex(
        /* [in] */ Int32 index,
        /* [out] */ Int32* viewIndex);

    CARAPI CopyFrom(
        /* [in] */ IStatusBarIconList* list);

    CARAPI GetDescription(
        /* [out] */ String* str);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcel* source);

private:
    AutoStringArray mSlots;
    ArrayOf< AutoPtr<IStatusBarIcon> >* mIcons;
};

#endif // __CSTATUSBARICONLIST_H__

