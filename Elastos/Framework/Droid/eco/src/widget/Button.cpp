
#include "widget/Button.h"

Button::Button()
{}

Button::Button(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : TextView(context, attrs, defStyle)
{}

ECode Button::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    ASSERT_SUCCEEDED(TextView::Init(context, attrs, defStyle));
    return NOERROR;
}
