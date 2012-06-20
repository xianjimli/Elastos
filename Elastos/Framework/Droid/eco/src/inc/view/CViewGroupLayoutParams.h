
#ifndef __CVIEWGROUPLAYOUTPARAMS_H__
#define __CVIEWGROUPLAYOUTPARAMS_H__

#include "_CViewGroupLayoutParams.h"
#include "view/ViewGroupLayoutParams.h"

CarClass(CViewGroupLayoutParams), public ViewGroupLayoutParams
{
public:
    CARAPI GetDescription(
        /* [out] */ String* description);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI constructor(
        /* [in] */ IViewGroupLayoutParams* source);

    CARAPI SetWidth(
        /* [in] */ Int32 width);

    CARAPI SetHeight(
        /* [in] */ Int32 height);

    CARAPI GetWidth(
        /* [out] */ Int32* width);

    CARAPI GetHeight(
        /* [out] */ Int32* height);
};

#endif //__CVIEWGROUPLAYOUTPARAMS_H__
