
#ifndef __CSERVICEINFO_H__
#define __CSERVICEINFO_H__

#include "_CServiceInfo.h"
#include "capsule/ComponentInfo.h"

using namespace Elastos;

CarClass(CServiceInfo), public ComponentInfo
{
public:
    CServiceInfo();

    ~CServiceInfo();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IServiceInfo* orig);

    CARAPI LoadLabel(
        /* [in] */ ICapsuleManager* pm,
        /* [out] */ ICharSequence** label);

    CARAPI LoadIcon(
        /* [in] */ ICapsuleManager* pm,
        /* [out] */ IDrawable** icon);

    CARAPI LoadLogo(
        /* [in] */ ICapsuleManager* pm,
        /* [out] */ IDrawable** icon);

    CARAPI LoadXmlMetaData(
        /* [in] */ ICapsuleManager* pm,
        /* [in] */ const String& name,
        /* [out] */ IXmlResourceParser** resource);

    CARAPI GetComponentName(
        /* [out] */ String * pCapsuleName,
        /* [out] */ String * pClassName);

    CARAPI SetComponentName(
        /* [in] */ const String& capsuleName,
        /* [in] */ const String& className);

    CARAPI GetApplicationInfo(
        /* [out] */ IApplicationInfo** info);

    CARAPI SetApplicationInfo(
        /* [in] */ IApplicationInfo* info);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

public:
    /**
     * Optional name of a permission required to be able to access this
     * Service.  From the "permission" attribute.
     */
    String mPermission;
};

#endif // __CSERVICEINFO_H__
