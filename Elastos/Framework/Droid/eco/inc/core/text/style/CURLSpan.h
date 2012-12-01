
#ifndef __CURLSPAN_H__
#define __CURLSPAN_H__

#include "_CURLSpan.h"
#include "text/style/ClickableSpan.h"

CarClass(CURLSpan), public ClickableSpan
{
public:
    CARAPI constructor(
        /* [in] */ const String& url);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetURL(
        /* [in] */ String* url);

    CARAPI OnClick(
        /* [in] */ IView* widget);

    CARAPI UpdateDrawState(
        /* [in] */ ITextPaint* tp);

    CARAPI GetUnderlying(
        /* [in] */ ICharacterStyle** cs);

    CARAPI GetSpanTypeId(
        /* [in] */ Int32* id);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* src);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dst);

private:
    String mURL;
};

#endif //__CURLSPAN_H__
