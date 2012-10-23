
#ifndef __BASEKEYLISTENER_H__
#define __BASEKEYLISTENER_H__

#include "text/method/MetaKeyKeyListener.h"

class BaseKeyListener : public MetaKeyKeyListener
{
public:
    virtual CARAPI_(Boolean) Backspace(
        /* [in] */ IView* view,
        /* [in] */ IEditable* content,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnKeyDown(
        /* [in] */ IView* view,
        /* [in] */ IEditable* content,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnKeyOther(
        /* [in] */ IView* view,
        /* [in] */ IEditable* content,
        /* [in] */ IKeyEvent* event);

protected:
    static CARAPI_(Int32) MakeTextContentType(
        /* [in] */ Capitalize caps,
        /* [in] */ Boolean autoText);

private:
    CARAPI_(Boolean) AltBackspace(
        /* [in] */ IView* view,
        /* [in] */ IEditable* content,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

protected:
    static const AutoPtr<IInterface> OLD_SEL_START;
};
#endif //__BASEKEYLISTENER_H__
