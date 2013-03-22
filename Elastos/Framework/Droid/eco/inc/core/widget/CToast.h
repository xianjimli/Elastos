
#ifndef __CTOAST_H__
#define __CTOAST_H__

#include "_CToast.h"
#include "widget/Toast.h"

CarClass(CToast), public Toast
{
public:
    CARAPI Show();

    CARAPI Cancel();

    CARAPI SetView(
        /* [in] */ IView* view);

    CARAPI GetView(
        /* [out] */ IView** view);

    CARAPI SetDuration(
        /* [in] */ Int32 duration);

    CARAPI GetDuration(
        /* [out] */ Int32* duration);

    CARAPI SetMargin(
        /* [in] */ Float horizontalMargin,
        /* [in] */ Float verticalMargin);
        
    CARAPI GetHorizontalMargin(
        /* [out] */ Float* horizontalMargin);

    CARAPI GetVerticalMargin(
        /* [out] */ Float* verticalMargin);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity,
        /* [in] */ Int32 xOffset, 
        /* [in] */ Int32 yOffset);

    CARAPI GetGravity(
        /* [out] */ Int32* gravity);

    CARAPI GetXOffset(
        /* [out] */ Int32* xOffset);

    CARAPI GetYOffset(
        /* [out] */ Int32* yOffset);
    
    CARAPI SetText(
        /* [in] */ Int32 resId);

    CARAPI SetTextEx(
        /* [in] */ ICharSequence* s);

    CARAPI constructor(
        /* [in] */ IContext* context);

private:
    // TODO: Add your private member variables here.
};

#endif // __CTOAST_H__
