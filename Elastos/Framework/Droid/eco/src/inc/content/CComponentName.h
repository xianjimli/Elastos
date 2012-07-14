
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
        /* [in] */ const String& capsuleName,
        /* [in] */ const String& className);

public:
    CARAPI_(String) GetShortClassName();

    /**
     * Recover a ComponentName from a String that was previously created with
     * {@link #flattenToString()}.  It splits the string at the first '/',
     * taking the part before as the package name and the part after as the
     * class name.  As a special convenience (to use, for example, when
     * parsing component names on the command line), if the '/' is immediately
     * followed by a '.' then the final class name will be the concatenation
     * of the package name with the string following the '/'.  Thus
     * "com.foo/.Blah" becomes package="com.foo" class="com.foo.Blah".
     *
     * @param str The String that was returned by flattenToString().
     * @return Returns a new ComponentName containing the package and class
     * names that were encoded in <var>str</var>
     *
     * @see #flattenToString()
     */
    static AutoPtr<IComponentName> UnflattenFromString(
        /* [in] */ const String& str);

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
