
#ifndef __key_h__
#define __key_h__

#include "server/CActivityRecord.h"
#include <elastos/AutoPtr.h>

class Key
{
public:
    Key(
        /* [in] */ Int32 t,
        /* [in] */ const String& p,
        /* [in] */ CActivityRecord* a,
        /* [in] */ const String& w,
        /* [in] */ Int32 r,
        /* [in] */ IIntent* i,
        /* [in] */ const String& it,
        /* [in] */ Int32 f);

    CARAPI_(Boolean) Equals(
        /* [in] */ const Key* otherObj) const;

    CARAPI_(Int32) GetHashCode();

public:
    static const Int32 ODD_PRIME_NUMBER = 37;

    Int32 mType;
    String mCapsuleName;
    AutoPtr<CActivityRecord> mActivity;
    String mWho;
    Int32 mRequestCode;
    AutoPtr<IIntent> mRequestIntent;
    String mRequestResolvedType;
    Int32 mFlags;
    Int32 mHashCode;
};

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash<Key*>
{
    size_t operator()(Key* name) const
    {
        assert(name != NULL);
        return (size_t)name->GetHashCode();
    }
};

template<> struct EqualTo<Key*>
{
    Boolean operator()(const Key* x,
                       const Key* y) const
    {
        assert(x != NULL);
        return x->Equals(y);
    }
};

_ELASTOS_NAMESPACE_END

#endif //__key_h__