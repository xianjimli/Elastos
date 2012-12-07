
#include <elatypes.h>
#include <elapi.h>
#include "CObjectStringMap.h"
#include "CObjectContainer.h"

CObjectStringMap::CObjectStringMap() :
    mMap(11)
{}

CObjectStringMap::~CObjectStringMap()
{
    mMap.Clear();
}

ECode CObjectStringMap::Get(
    /* [in] */ const String& key,
    /* [out] */ IInterface** value)
{
    if (NULL == value) return E_INVALID_ARGUMENT;

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

ECode CObjectStringMap::Put(
    /* [in] */ const String& key,
    /* [in] */ IInterface* value)
{
    if (key.IsNull() || key.IsEmpty() || value == NULL) return NOERROR;

    mMap[key] = value;
    return NOERROR;
}

ECode CObjectStringMap::GetSize(
    /* [out] */ Int32* size)
{
    if (NULL == size) return E_INVALID_ARGUMENT;
    *size = mMap.GetSize();

    return NOERROR;
}

ECode CObjectStringMap::GetAllItems(
    /* [out] */ ArrayOf<String>** keys,
    /* [out] */ IObjectContainer** values)
{
    if (NULL == keys || NULL == values) return E_INVALID_ARGUMENT;

    Int32 size = mMap.GetSize();
    if (size == 0) {
        return NOERROR;
    }

    *keys = ArrayOf<String>::Alloc(size);
    ASSERT_SUCCEEDED(CObjectContainer::New(values));

    HashMap<String, AutoPtr<IInterface> >::Iterator iter = mMap.Begin();
    for (Int32 i=0; iter!=mMap.End(); ++iter, ++i) {
        (**keys)[i] = iter->mFirst;
        (*values)->Add(iter->mSecond);
    }

    return NOERROR;
}

// ECode CObjectStringMap::ReadFromParcel(
//     /* [in] */ IParcel *source)
// {
//     return E_NOT_IMPLEMENTED;
// }

// ECode CObjectStringMap::WriteToParcel(
//     /* [in] */ IParcel *dest)
// {
//     return E_NOT_IMPLEMENTED;
// }
