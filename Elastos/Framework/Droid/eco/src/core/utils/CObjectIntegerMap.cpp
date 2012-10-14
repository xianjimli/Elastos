
#include "utils/CObjectIntegerMap.h"

CObjectIntegerMap::CObjectIntegerMap()
    : mMap(11)
{}

CObjectIntegerMap::~CObjectIntegerMap()
{
    mMap.Clear();
}

ECode CObjectIntegerMap::Get(
    /* [in] */ Int32 key,
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value);

    HashMap<Int32, AutoPtr<IInterface> >::Iterator it =
            mMap.Find(key);
    if (it != mMap.End()) {
        *value = it->mSecond;
        if (*value != NULL) (*value)->AddRef();
        return NOERROR;
    }
    *value = NULL;
    return NOERROR;
}

ECode CObjectIntegerMap::Put(
    /* [in] */ Int32 key,
    /* [in] */ IInterface* value)
{
    if (value == NULL) return NOERROR;

    mMap[key] = value;
    return NOERROR;
}
