
#ifndef  _CKEYBOARDVIEW_H__
#define  _CKEYBOARDVIEW_H__

#include "ext/frameworkext.h"
#include "_CKeyboardView.h"
#include "inputmethodservice/KeyboardView.h"
#include "view/ViewMacro.h"

/**
 * A view that renders a virtual {@link Keyboard}. It handles rendering of keys and
 * detecting key presses and touch movements.
 *
 * @attr ref android.R.styleable#KeyboardView_keyBackground
 * @attr ref android.R.styleable#KeyboardView_keyPreviewLayout
 * @attr ref android.R.styleable#KeyboardView_keyPreviewOffset
 * @attr ref android.R.styleable#KeyboardView_labelTextSize
 * @attr ref android.R.styleable#KeyboardView_keyTextSize
 * @attr ref android.R.styleable#KeyboardView_keyTextColor
 * @attr ref android.R.styleable#KeyboardView_verticalCorrection
 * @attr ref android.R.styleable#KeyboardView_popupLayout
 */
CarClass(CKeyboardView), public KeyboardView
{
public:
    IVIEW_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    IAccessibilityEventSource_METHODS_DECL();

    CKeyboardView();

    ~CKeyboardView();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI OnClick(
        /* [in] */ IView* v);

    CARAPI SetOnKeyboardActionListener(
        /* [in] */ IOnKeyboardActionListener* listener);

private:
    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);
};

#endif  //_KEYBOARDVIEW_H__
