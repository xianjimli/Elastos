
#ifndef __CABSOLUTELAYOUTLAYOUTPARAMS_H__
#define __CABSOLUTELAYOUTLAYOUTPARAMS_H__

#include "_CAbsoluteLayoutLayoutParams.h"
#include "widget/AbsoluteLayoutLayoutParams.h"

CarClass(CAbsoluteLayoutLayoutParams), public AbsoluteLayoutLayoutParams
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

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI constructor(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IViewGroupLayoutParams* source);

private:
    // TODO: Add your private member variables here.
};

#endif // __CABSOLUTELAYOUTLAYOUTPARAMS_H__
