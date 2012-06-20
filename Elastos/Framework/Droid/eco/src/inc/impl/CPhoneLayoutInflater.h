
#ifndef __CPHONELAYOUTINFLATER_H__
#define __CPHONELAYOUTINFLATER_H__

#include "_CPhoneLayoutInflater.h"
#include "view/LayoutInflater.h"

CarClass(CPhoneLayoutInflater), public LayoutInflater
{
public:
    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI GetContext(
        /* [out] */ IContext** context);

    CARAPI GetFactory(
        /* [out] */ ILayoutInflaterFactory** factory);

    CARAPI SetFactory(
        /* [in] */ ILayoutInflaterFactory* factory);

    CARAPI GetFilter(
        /* [out] */ ILayoutInflaterFilter** filter);

    CARAPI SetFilter(
        /* [in] */ ILayoutInflaterFilter* filter);

    CARAPI Inflate(
        /* [in] */ Int32 resource,
        /* [in] */ IViewGroup* root,
        /* [out] */ IView** view);

    CARAPI InflateEx(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IViewGroup* root,
        /* [out] */ IView** view);

    CARAPI InflateEx2(
        /* [in] */ Int32 resource,
        /* [in] */ IViewGroup* root,
        /* [in] */ Boolean attachToRoot,
        /* [out] */ IView** view);

    CARAPI InflateEx3(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IViewGroup* root,
        /* [in] */ Boolean attachToRoot,
        /* [out] */ IView** view);

private:
    // TODO: Add your private member variables here.
};

#endif // __CPHONELAYOUTINFLATER_H__
