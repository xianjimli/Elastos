
#ifndef __CWEBHISTORYITEM_H__
#define __CWEBHISTORYITEM_H__

#include "_CWebHistoryItem.h"

CarClass(CWebHistoryItem)
{
public:
    CARAPI GetId(
        /* [out] */ Int32 * pId);

    CARAPI GetUrl(
        /* [out] */ CString * pUrl);

    CARAPI GetOriginalUrl(
        /* [out] */ String * pOriginalUrl);

    CARAPI GetTitle(
        /* [out] */ String * pTitle);

    CARAPI GetFavicon(
        /* [out] */ IBitmap ** ppFavicon);

    CARAPI GetTouchIconUrl(
        /* [out] */ String * pTouchIconUrl);

    CARAPI GetCustomData(
        /* [out] */ IInterface ** ppCustomData);

    CARAPI SetCustomData(
        /* [in] */ IInterface * pData);

private:
    // TODO: Add your private member variables here.
};

#endif // __CWEBHISTORYITEM_H__
