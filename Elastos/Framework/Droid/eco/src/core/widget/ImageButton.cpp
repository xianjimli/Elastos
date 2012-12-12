
#include "widget/ImageButton.h"

static const Int32 R_Attr_ImageButtonStyle = 0x01010072;

ImageButton::ImageButton()
{

}

ImageButton::ImageButton(
    /* [in] */ IContext* context) : ImageView(context, NULL, R_Attr_ImageButtonStyle)
{
    SetFocusable(TRUE);
}

ImageButton::ImageButton(
    /* [in] */ IContext* context, 
    /* [in] */ IAttributeSet* attrs) : ImageView(context, attrs, R_Attr_ImageButtonStyle)
{
    SetFocusable(TRUE);
}

ImageButton::ImageButton(
    /* [in] */ IContext* context, 
    /* [in] */ IAttributeSet* attrs, 
    /* [in] */ Int32 defStyle)  : ImageView(context, attrs, defStyle)
{
    SetFocusable(TRUE);
}

Boolean ImageButton::OnSetAlpha(
    /* [in] */ Int32 alpha)
{
    return FALSE;
}
