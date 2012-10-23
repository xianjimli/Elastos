
#ifndef __CTEXTKEYLISTENER_H__
#define __CTEXTKEYLISTENER_H__

#include "_CTextKeyListener.h"
#include "text/method/TextKeyListener.h"

CarClass(CTextKeyListener), public TextKeyListener
{
public:
    CARAPI GetInputType(
        /* [out] */ Int32* type);

    CARAPI OnKeyDown(
        /* [in] */ IView* view,
        /* [in] */ IEditable* text,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnKeyUp(
        /* [in] */ IView* view,
        /* [in] */ IEditable* text,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnKeyOther(
        /* [in] */ IView* view,
        /* [in] */ IEditable* text,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI ClearMetaKeyState(
        /* [in] */ IView* view,
        /* [in] */ IEditable* content,
        /* [in] */ Int32 states);

    CARAPI ReleaseListener();

    CARAPI GetPrefs(
        /* [in] */ IContext* context,
        /* [out] */ Int32* prefs);

    CARAPI OnSpanAdded(
        /* [in] */ ISpannable* text,
        /* [in] */ IInterface* what,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI OnSpanRemoved(
        /* [in] */ ISpannable* text,
        /* [in] */ IInterface* what,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI OnSpanChanged(
        /* [in] */ ISpannable* text,
        /* [in] */ IInterface* what,
        /* [in] */ Int32 ostart,
        /* [in] */ Int32 oend,
        /* [in] */ Int32 nstart,
        /* [in] */ Int32 nend);

    CARAPI constructor(
        /* [in] */ Capitalize cap,
        /* [in] */ Boolean autotext);

private:
    // TODO: Add your private member variables here.
};

#endif // __CTEXTKEYLISTENER_H__
