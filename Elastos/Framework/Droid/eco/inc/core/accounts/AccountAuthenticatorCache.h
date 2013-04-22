
#ifndef __ACCOUNTaUTHENTICATORCACHE_H__
#define __ACCOUNTaUTHENTICATORCACHE_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
//#include <elastos/ElRefBase.h>

/**
 * A cache of services that export the {@link IAccountAuthenticator} interface. This cache
 * is built by interrogating the {@link PackageManager} and is updated as packages are added,
 * removed and changed. The authenticators are referred to by their account type and
 * are made available via the {@link RegisteredServicesCache#getServiceInfo} method.
 * @hide
 */
class AccountAuthenticatorCache /*extends RegisteredServicesCache<AuthenticatorDescription>*/
{
public:
    AccountAuthenticatorCache(
        /* [in] */ IContext* context);

    CARAPI ParseServiceAttributes(
        /* [in] */ IResources* res,
        /* [in] */ const String& capsuleName,
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ IInterface** description);

private:
    class MySerializer/* implements XmlSerializerAndParser<AuthenticatorDescription> */
    {
    public:
        CARAPI WriteAsXml(
            /* [in] */ IAuthenticatorDescription* item,
            /* [in] */ IXmlSerializer* out);

        CARAPI CreateFromXml(
            /* [in] */ IXmlPullParser* parser,
            /* [out] */ IInterface** description);
    };

private:
    static const CString TAG;
    static MySerializer* sSerializer;
};

#endif //__ACCOUNTaUTHENTICATORCACHE_H__
