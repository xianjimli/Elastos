
#include "widget/CToast.h"

ECode CToast::Show()
{
    return Toast::Show();
}

ECode CToast::Cancel()
{
    return Toast::Cancel();
}


ECode CToast::SetView(
    /* [in] */ IView* view)
{
    return Toast::SetView(view);
}

ECode CToast::GetView(
    /* [out] */ IView** view)
{
    return Toast::GetView(view);
}

ECode CToast::SetDuration(
    /* [in] */ Int32 duration)
{
    return Toast::SetDuration(duration);
}

ECode CToast::GetDuration(
    /* [out] */ Int32* duration)
{
    return Toast::GetDuration(duration);
}

ECode CToast::SetMargin(
    /* [in] */ Float horizontalMargin,
    /* [in] */ Float verticalMargin)
{
    return Toast::SetMargin(horizontalMargin, verticalMargin);
}
    
ECode CToast::GetHorizontalMargin(
    /* [out] */ Float* horizontalMargin)
{
    return Toast::GetHorizontalMargin(horizontalMargin);
}

ECode CToast::GetVerticalMargin(
    /* [out] */ Float* verticalMargin)
{
    return Toast::GetVerticalMargin(verticalMargin);
}

ECode CToast::SetGravity(
    /* [in] */ Int32 gravity,
    /* [in] */ Int32 xOffset, 
    /* [in] */ Int32 yOffset)
{
    return Toast::SetGravity(gravity, xOffset, yOffset);
}

ECode CToast::GetGravity(
    /* [out] */ Int32* gravity)
{
    return Toast::GetGravity(gravity);
}

ECode CToast::GetXOffset(
    /* [out] */ Int32* xOffset)
{
    return Toast::GetXOffset(xOffset);
}

ECode CToast::GetYOffset(
    /* [out] */ Int32* yOffset)
{
    return Toast::GetYOffset(yOffset);
}

ECode CToast::SetText(
    /* [in] */ Int32 resId)
{
    return Toast::SetText(resId);
}

ECode CToast::SetTextEx(
    /* [in] */ ICharSequence* s)
{
    return Toast::SetTextEx(s);
}

ECode CToast::constructor(
    /* [in] */ IContext* context)
{
    return Toast::Init(context);
}

