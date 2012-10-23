
#ifndef __CQWERTYKEYLISTENER_H__
#define __CQWERTYKEYLISTENER_H__

#include "_CQwertyKeyListener.h"
#include "text/method/QwertyKeyListener.h"

CarClass(CQwertyKeyListener), public QwertyKeyListener
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

    CARAPI constructor(
        /* [in] */ Capitalize cap,
        /* [in] */ Boolean autotext);

private:
    // TODO: Add your private member variables here.
};

#endif // __CQWERTYKEYLISTENER_H__
