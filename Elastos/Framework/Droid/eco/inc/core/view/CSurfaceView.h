
#ifndef __CSURFACEVIEW_H__
#define __CSURFACEVIEW_H__

#include "_CSurfaceView.h"
#include "view/SurfaceView.h"
#include "view/ViewMacro.h"

CarClass(CSurfaceView), public SurfaceView
{
public:
    IVIEW_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    IAccessibilityEventSource_METHODS_DECL();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetHolder(
        /* [out] */ ISurfaceHolder ** ppHolder);

    CARAPI SetZOrderMediaOverlay(
        /* [in] */ Boolean isMediaOverlay);

    CARAPI SetZOrderOnTop(
        /* [in] */ Boolean onTop);

    CARAPI SetWindowType(
        /* [in] */ Int32 type);

    CARAPI IsFixedSize(
        /* [out] */ Boolean * pSize);

    CARAPI constructor(
        /* [in] */ IContext * pContext);

    CARAPI constructor(
        /* [in] */ IContext * pContext,
        /* [in] */ IAttributeSet * pAttrs);

    CARAPI constructor(
        /* [in] */ IContext * pContext,
        /* [in] */ IAttributeSet * pAttrs,
        /* [in] */ Int32 defStyle);

private:
    // TODO: Add your private member variables here.
};

#endif // __CSURFACEVIEW_H__
