
#ifndef __CTABHOSTTABSPEC_H__
#define __CTABHOSTTABSPEC_H__

#include "_CTabHostTabSpec.h"
#include "widget/TabHostTabSpec.h"

CarClass(CTabHostTabSpec), public TabHostTabSpec
{
public:

    CARAPI SetIndicator(
        /* [in] */ ICharSequence * pLabel,
        /* [out] */ ITabHostTabSpec ** ppSpec);

    CARAPI SetIndicatorEx(
        /* [in] */ ICharSequence * pLabel,
        /* [in] */ IDrawable * pIcon,
        /* [out] */ ITabHostTabSpec ** ppSpec);

    CARAPI SetIndicatorEx2(
        /* [in] */ IView * pView,
        /* [out] */ ITabHostTabSpec ** ppSpec);

    CARAPI SetContent(
        /* [in] */ Int32 viewId,
        /* [out] */ ITabHostTabSpec ** ppSpec);

    CARAPI SetContentEx(
        /* [in] */ ITabHostTabContentFactory * pContentFactory,
        /* [out] */ ITabHostTabSpec ** ppSpec);

    CARAPI SetContentEx2(
        /* [in] */ IIntent * pIntent,
        /* [out] */ ITabHostTabSpec ** ppSpec);

    CARAPI GetTag(
        /* [out] */ String * pStr);

    CARAPI constructor(
        /* [in] */ const String& tag);

private:
    // TODO: Add your private member variables here.
};

#endif // __CTABHOSTTABSPEC_H__
