#include "text/method/CTextKeyListenerHelper.h"
#include "text/method/CTextKeyListener.h"

ECode CTextKeyListenerHelper::constructor() {}

ECode CTextKeyListenerHelper::GetInstance(
    /* [out] */ ITextKeyListener** textKeyListener)
{
    *textKeyListener = &(*(CTextKeyListener::GetInstance()));
    return NOERROR;
}

ECode CTextKeyListenerHelper::GetInstanceEx(
    /*[in] */ Boolean autotext,
    /*[in] */ Capitalize cap,
    /*[out]*/ ITextKeyListener** textKeyListener)
{
    *textKeyListener = &(*(CTextKeyListener::GetInstance(autotext, cap)));
    return NOERROR;
}
