
#ifndef __CLINEARLAYOUTLAYOUTPARAMS_H__
#define __CLINEARLAYOUTLAYOUTPARAMS_H__

#include "_CLinearLayoutLayoutParams.h"
#include "widget/LinearLayoutLayoutParams.h"

CarClass(CLinearLayoutLayoutParams), public LinearLayoutLayoutParams
{
public:
    CARAPI GetDescription(
        /* [out] */ String* description);

    CARAPI SetWidth(
        /* [in] */ Int32 width);

    CARAPI SetHeight(
        /* [in] */ Int32 height);

    CARAPI GetWidth(
        /* [out] */ Int32* width);

    CARAPI GetHeight(
        /* [out] */ Int32* height);

    CARAPI SetMargins(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    CARAPI GetMargins(
        /* [out] */ Int32* left,
        /* [out] */ Int32* top,
        /* [out] */ Int32* right,
        /* [out] */ Int32* bottom);

    CARAPI GetWeight(
        /* [out] */ Float* weight);

    CARAPI GetGravity(
        /* [out] */ Int32* gravity);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Float weight);

    CARAPI constructor(
        /* [in] */ IViewGroupLayoutParams* p);

    CARAPI constructor(
        /* [in] */ IViewGroupMarginLayoutParams* source);

private:
    // TODO: Add your private member variables here.
};

#endif // __CLINEARLAYOUTLAYOUTPARAMS_H__
