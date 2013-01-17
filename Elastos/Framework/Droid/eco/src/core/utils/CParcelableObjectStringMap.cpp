
#include "utils/CParcelableObjectStringMap.h"
#include "utils/CParcelableObjectContainer.h"

CParcelableObjectStringMap::CParcelableObjectStringMap() :
    mMap(11)
{}

CParcelableObjectStringMap::~CParcelableObjectStringMap()
{
    mMap.Clear();
}

ECode CParcelableObjectStringMap::Get(
    /* [in] */ const String& key,
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value);

    HashMap<String, AutoPtr<IInterface> >::Iterator it =
            mMap.Find(key);
    if (it != mMap.End()) {
        *value = it->mSecond;
        if (*value != NULL) (*value)->AddRef();
        return NOERROR;
    }
    *value = NULL;
    return NOERROR;
}

ECode CParcelableObjectStringMap::Put(
    /* [in] */ const String& key,
    /* [in] */ IInterface* value)
{
    if (key.IsNull() || key.IsEmpty() || value == NULL) return NOERROR;

    mMap[key] = value;
    return NOERROR;
}

ECode CParcelableObjectStringMap::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    *size = mMap.GetSize();

    return NOERROR;
}

ECode CParcelableObjectStringMap::GetAllItems(
    /* [out] */ ArrayOf<String>** keys,
    /* [out] */ IObjectContainer** values)
{
    VALIDATE_NOT_NULL(keys);
    VALIDATE_NOT_NULL(values);

    Int32 size = mMap.GetSize();
    if (size == 0) {
        return NOERROR;
    }

    *keys = ArrayOf<String>::Alloc(size);
    ASSERT_SUCCEEDED(CParcelableObjectContainer::New(values));

    HashMap<String, AutoPtr<IInterface> >::Iterator iter = mMap.Begin();
    for (Int32 i=0; iter!=mMap.End(); ++iter, ++i) {
        (**keys)[i] = iter->mFirst;
        (*values)->Add(iter->mSecond);
    }

    return NOERROR;
}

ECode CParcelableObjectStringMap::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    return E_NOT_IMPLEMENTED;
}

ECode CParcelableObjectStringMap::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    return E_NOT_IMPLEMENTED;
}
