
#ifndef _CICONMENUVIEWLAYOUTPARAMS_H__
#define _CICONMENUVIEWLAYOUTPARAMS_H__

#include "_CIconMenuViewLayoutParams.h"
#include "view/menu/IconMenuViewLayoutParams.h"


CarClass(CIconMenuViewLayoutParams), public IconMenuViewLayoutParams
{
public:
    CARAPI constructor(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
    	/* [in] */ Int32 width,
    	/* [in] */ Int32 height);

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
};

#endif   //_CICONMENUVIEWLAYOUTPARAMS_H__
