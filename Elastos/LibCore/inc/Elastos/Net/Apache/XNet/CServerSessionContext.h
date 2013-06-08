
#ifndef __CSERVERSESSIONCONTEXT_H__
#define __CSERVERSESSIONCONTEXT_H__

#include "_CServerSessionContext.h"

CarClass(CServerSessionContext)
{
public:
    CARAPI GetSession(
        /* [in] */ const ArrayOf<Byte> & sessionId,
        /* [out] */ ISSLSession ** ppSession);

    CARAPI GetSessionCacheSize(
        /* [out] */ Int32 * pSize);

    CARAPI GetSessionTimeout(
        /* [out] */ Int32 * pTime);

    CARAPI SetSessionCacheSize(
        /* [in] */ Int32 size);

    CARAPI SetSessionTimeout(
        /* [in] */ Int32 seconds);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CSERVERSESSIONCONTEXT_H__
