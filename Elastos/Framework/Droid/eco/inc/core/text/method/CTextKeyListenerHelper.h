
#ifndef __CTEXTKEYLISTENERHELPER_H__
#define __CTEXTKEYLISTENERHELPER_H__

#include "_CTextKeyListenerHelper.h"
// #include "text/method/TextKeyListener.h"

CarClass(CTextKeyListenerHelper)
{
public:
    CARAPI constructor();

    CARAPI GetInstance(
        /* [out] */ ITextKeyListener** textKeyListener);

    CARAPI GetInstanceEx(
        /*[in] */ Boolean autotext,
        /*[in] */ Capitalize cap,
        /*[out]*/ ITextKeyListener** textKeyListener);
private:
    // TODO: Add your private member variables here.
};

#endif // __CTEXTKEYLISTENER_H__
