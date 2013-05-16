
#ifndef __CHTTPHANDLER_H__
#define __CHTTPHANDLER_H__

#include "_CHttpHandler.h"

CarClass(CHttpHandler)
{
public:
    CARAPI OpenConnection(
        /* [in] */ IURL * pU,
        /* [out] */ IURLConnection ** ppUrlConnection);

    CARAPI OpenConnectionEx(
        /* [in] */ IURL * pU,
        /* [in] */ IProxy * pProxy,
        /* [out] */ IURLConnection ** ppUrlConnection);

    CARAPI ParseURL(
        /* [in] */ IURL * pU,
        /* [in] */ const String& str,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI SetURL(
        /* [in] */ IURL * pU,
        /* [in] */ const String& protocol,
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ const String& file,
        /* [in] */ const String& ref);

    CARAPI SetURLEx(
        /* [in] */ IURL * pU,
        /* [in] */ const String& protocol,
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ const String& authority,
        /* [in] */ const String& userInfo,
        /* [in] */ const String& file,
        /* [in] */ const String& query,
        /* [in] */ const String& ref);

    CARAPI ToExternalForm(
        /* [in] */ IURL * pUrl,
        /* [out] */ String * pS);

    CARAPI Equals(
        /* [in] */ IURL * pUrl1,
        /* [in] */ IURL * pUrl2,
        /* [out] */ Boolean * pIsEquals);

    CARAPI GetDefaultPort(
        /* [out] */ Int32 * pPortNum);

    CARAPI GetHostAddress(
        /* [in] */ IURL * pUrl,
        /* [out] */ IInetAddress ** ppHostAddress);

    CARAPI GetHashCode(
        /* [in] */ IURL * pUrl,
        /* [out] */ Int32 * pCode);

    CARAPI HostsEqual(
        /* [in] */ IURL * pA,
        /* [in] */ IURL * pB,
        /* [out] */ Boolean * pIsEqual);

    CARAPI IsSameFile(
        /* [in] */ IURL * pUrl1,
        /* [in] */ IURL * pUrl2,
        /* [out] */ Boolean * pIsSame);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CHTTPHANDLER_H__
