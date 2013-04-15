
#include "ext/frameworkdef.h"
#include "widget/ImageButton.h"


ImageButton::ImageButton()
{}

ImageButton::ImageButton(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : ImageView(context, attrs, defStyle)
{
    SetFocusable(TRUE);
}

ECode ImageButton::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    FAIL_RETURN(ImageView::Init(context, attrs, defStyle));
    SetFocusable(TRUE);
    return NOERROR;
}

Boolean ImageButton::OnSetAlpha(
    /* [in] */ Int32 alpha)
{
    return FALSE;
}
