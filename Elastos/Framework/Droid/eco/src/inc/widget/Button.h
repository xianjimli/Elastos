
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "widget/TextView.h"

class Button : public TextView
{
public:
    Button();

    Button(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0x01010048/*com.android.internal.R.attr.buttonStyle*/);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0x01010048/*com.android.internal.R.attr.buttonStyle*/);
};

#endif //__BUTTON_H__
