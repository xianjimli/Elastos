
#ifndef __RADIOGROUPLAYOUTPARAMS_H__
#define __RADIOGROUPLAYOUTPARAMS_H__

#include "widget/LinearLayoutLayoutParams.h"

class RadioGroupLayoutParams : public LinearLayoutLayoutParams
{
public:
    RadioGroupLayoutParams();

    RadioGroupLayoutParams(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    RadioGroupLayoutParams(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    RadioGroupLayoutParams(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Float weight);

    RadioGroupLayoutParams(
        /* [in] */ ViewGroupLayoutParams* p);

    RadioGroupLayoutParams(
        /* [in] */ ViewGroupMarginLayoutParams* source);

protected:
    CARAPI_(void) SetBaseAttributes(
        /* [in] */ ITypedArray* a,
        /* [in] */ Int32 widthAttr,
        /* [in] */ Int32 heightAttr);

};
#endif // __RADIOGROUPLAYOUTPARAMS_H__
