
#ifndef __RADIOBUTTON_H__
#define __RADIOBUTTON_H__

#include "widget/CompoundButton.h"

class RadioButton : public CompoundButton
{
public:
    RadioButton(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0x0101007e/*com.android.internal.R.attr.radioButtonStyle*/);

    //@Override
    CARAPI Toggle();

protected:
    RadioButton();
};

#endif //__BUTTON_H__
