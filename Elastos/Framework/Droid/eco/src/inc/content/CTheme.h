
#ifndef __CTHEME_H__
#define __CTHEME_H__

#include "_CTheme.h"
#include "content/CAssetManager.h"
#include "content/CResources.h"

CarClass(CTheme)
{
public:
    CTheme();

    CARAPI ApplyStyle(
        /* [in] */ Int32 resid,
        /* [in] */ Boolean force);

    CARAPI SetTo(
        /* [in] */ ITheme* other);

    CARAPI ObtainStyledAttributes(
        /* [in] */ const ArrayOf<Int32>& attrs,
        /* [out] */ ITypedArray** styles);

    CARAPI ObtainStyledAttributesEx(
        /* [in] */ Int32 resid,
        /* [in] */ const ArrayOf<Int32>& attrs,
        /* [out] */ ITypedArray** styles);

    CARAPI ObtainStyledAttributesEx2(
        /* [in] */ IAttributeSet* set,
        /* [in] */ const ArrayOf<Int32>& attrs,
        /* [in] */ Int32 defStyleAttr,
        /* [in] */ Int32 defStyleRes,
        /* [out] */ ITypedArray** styles);

    CARAPI ResolveAttribute(
        /* [in] */ Int32 resid,
        /* [in, out] */ ITypedValue* outValue,
        /* [in] */ Boolean resolveRefs,
        /* [out] */ Boolean* isFounded);

    CARAPI Dump(
        /* [in] */ Int32 priority,
        /* [in] */ String tag,
        /* [in] */ String prefix);

    CARAPI constructor(
        /* [in] */ IResources* res,
        /* [in] */ IAssetManager* assets);

private:
    AutoPtr<CAssetManager> mAssets;
    android::ResTable::Theme* mTheme;
    AutoPtr<CResources> mResources;
};

#endif // __CTHEME_H__
