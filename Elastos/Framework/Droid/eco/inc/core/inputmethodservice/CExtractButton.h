
#ifndef  _CEXTRACTBUTTON_H__
#define  _CEXTRACTBUTTON_H__

#include "ext/frameworkext.h"
#include "_CExtractButton.h"
#include "widget/Button.h"
#include "view/ViewMacro.h"
#include "widget/TextViewMacro.h"


class _ExtractButton: public Button
{
public:
    _ExtractButton();

    ~_ExtractButton();

    /**
     * Pretend like the window this view is in always has focus, so it will
     * highlight when selected.
     */
    CARAPI_(Boolean) HasWindowFocus();
};

/**
 * Specialization of {@link Button} that ignores the window not being focused.
 */
CarClass(CExtractButton), public _ExtractButton
{
public:
    IVIEW_METHODS_DECL();

    ITEXTVIEW_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    IAccessibilityEventSource_METHODS_DECL();

    CExtractButton();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI OnPreDraw(
        /* [out] */ Boolean* result);
};

#endif  //_CEXTRACTBUTTON_H__
