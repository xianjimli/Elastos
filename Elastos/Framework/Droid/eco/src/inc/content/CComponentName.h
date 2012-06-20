
#ifndef __CCOMPONENTNAME_H__
#define __CCOMPONENTNAME_H__

#include "_CComponentName.h"
#include <elastos/AutoPtr.h>
#include <elastos/etl_hash_fun.h>
#include <elastos/etl_function.h>

using namespace Elastos;

CarClass(CComponentName)
{
public:
    CComponentName();

    ~CComponentName();

    CARAPI GetCapsuleName(
        /* [out] */ String* capsuleName);

    CARAPI GetClassName(
        /* [out] */ String* className);

    CARAPI FlattenToString(
        /* [out] */ String* name);

    CARAPI FlattenToShortString(
        /* [out] */ String* name);

    CARAPI Equals(
        /* [in] */ IComponentName* other,
        /* [out] */ Boolean* isEqual);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI GetShortDescription(
        /* [out] */ String* description);

    CARAPI GetDescription(
        /* [out] */ String* description);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ String capsuleName,
        /* [in] */ String className);

public:
    CARAPI_(String) GetShortClassName();

    static CARAPI ReadFromParcel(
        /* [in] */ IParcel* source,
        /* [out] */ CComponentName** component);

    static CARAPI WriteToParcel(
        /* [in] */ CComponentName* component,
        /* [in] */ IParcel* dest);

private:
    String mCapsule;
    String mClass;
};

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash<AutoPtr<IComponentName> >
{
    size_t operator()(AutoPtr<IComponentName> name) const
    {
        Int32 hashCode;
        assert(name != NULL);
        name->GetHashCode(&hashCode);
        return (size_t)hashCode;
    }
};

template<> struct EqualTo<AutoPtr<IComponentName> >
{
    Boolean operator()(const AutoPtr<IComponentName>& x,
                       const AutoPtr<IComponentName>& y) const
    {
        Boolean equal;
        assert(x != NULL);
        x->Equals(y, &equal);
        return equal;
    }
};

_ELASTOS_NAMESPACE_END

#endif // __CCOMPONENTNAME_H__
