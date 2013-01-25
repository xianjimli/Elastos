
#include "content/CContentValues.h"

ECode CContentValues::constructor()
{
    return NOERROR;
}

ECode CContentValues::PutInt32(
    /* [in] */ const String& key,
    /* [in] */ Int32 value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CContentValues::PutString(
	/* [in] */ const String& key,
	/* [in] */ const String& value) 
{
	return E_NOT_IMPLEMENTED;
}


ECode CContentValues::GetInt32(
    /* [in] */ const String& key,
    /* [out] */ Int32* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CContentValues::GetSize(
    /* [out] */ Int32* size)
{
    return E_NOT_IMPLEMENTED;
}

ECode CContentValues::ValueSet(
    /* [out] */ IObjectStringMap** values)
{
    return E_NOT_IMPLEMENTED;
}
