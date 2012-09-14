
#include "widget/CheckBox.h"

CheckBox::CheckBox()
{}

CheckBox::CheckBox(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : CompoundButton(context, attrs, defStyle)
{}

ECode CheckBox::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return CompoundButton::Init(context, attrs, defStyle);
}
