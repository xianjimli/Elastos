
#ifndef __CHECKBOX_H__
#define __CHECKBOX_H__

#include "widget/CompoundButton.h"

class CheckBox : public CompoundButton
{
public:
    CheckBox(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0x0101006c/*com.android.internal.R.attr.checkboxStyle*/);

protected:
    CheckBox();
};

#endif //__CHECKBOXBUTTON_H__
