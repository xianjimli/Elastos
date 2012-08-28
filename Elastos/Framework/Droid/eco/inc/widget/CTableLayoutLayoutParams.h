
#ifndef __CTABLELAYOUTLAYOUTPARAMS_H__
#define __CTABLELAYOUTLAYOUTPARAMS_H__

#include "_CTableLayoutLayoutParams.h"
#include "widget/TableLayoutLayoutParams.h"

CarClass(CTableLayoutLayoutParams), public TableLayoutLayoutParams
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
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h);

    CARAPI constructor(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Float initWeight);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IViewGroupLayoutParams* p);

    CARAPI constructor(
        /* [in] */ IViewGroupMarginLayoutParams* source);

private:
    // TODO: Add your private member variables here.
};

#endif // __CTABLELAYOUTLAYOUTPARAMS_H__
