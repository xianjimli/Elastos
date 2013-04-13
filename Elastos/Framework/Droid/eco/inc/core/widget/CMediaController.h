
#ifndef __CMEDIACONTROLLER_H__
#define __CMEDIACONTROLLER_H__

#include "_CMediaController.h"
#include "widget/MediaController.h"
#include "view/ViewMacro.h"

CarClass(CMediaController), public MediaController
{
public:
    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Boolean useFastForward);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    IVIEW_METHODS_DECL();

    IVIEWGROUP_METHODS_DECL();

    IVIEWPARENT_METHODS_DECL();

    IVIEWMANAGER_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    IAccessibilityEventSource_METHODS_DECL();

    CARAPI SetMediaPlayer(
        /* [in] */ IMediaPlayerControl* player);

    CARAPI SetAnchorView(
        /* [in] */ IView* view);

    CARAPI Show();

    CARAPI ShowEx(
        /* [in] */ Int32 timeout);

    CARAPI IsShowing(
        /* [out] */ Boolean* isShowing);

    CARAPI Hide();

    CARAPI SetPrevNextListeners(
        /* [in] */ IViewOnClickListener* next,
        /* [in] */ IViewOnClickListener* prev);

    CARAPI SetForegroundGravity(
        /* [in] */ Int32 foregroundGravity);

    CARAPI SetForeground(
        /* [in] */ IDrawable* drawable);

    CARAPI GetForeground(
        /* [out] */ IDrawable** foreground);

    CARAPI SetMeasureAllChildren(
        /* [in] */ Boolean measureAll);

    CARAPI GetConsiderGoneChildrenWhenMeasuring(
        /* [out] */ Boolean* measureAll);
};

#endif //__CMEDIACONTROLLER_H__
