
#include "CContentProviderOne.h"
#include "ContentProviderDemo_server.h"

ECode CContentProviderOne::GetClassID(
    /* [out] */ ClassID* clsid)
{
    if (clsid == NULL) return E_INVALID_ARGUMENT;
    
    *clsid = ECLSID_CContentProviderOne;
    return NOERROR;
}

ECode CContentProviderOne::Delete(
    /* [in] */ IUri* uri, 
    /* [in] */ String selection, 
    /* [in] */ const ArrayOf<String>& selectionArgs,
    /* [out] */ Int32* rowsAffected)
{
    return NOERROR;
}

ECode CContentProviderOne::GetType(
    /* [in] */ IUri* uri,
    /* [out] */ String* type)
{
    *type = String::Duplicate("GetType Succeeded!");
    return NOERROR;
}
     
ECode CContentProviderOne::Insert(
    /* [in] */ IUri* uri, 
    /* [in] */ IContentValues* values,
    /* [out] */ IUri** insertedItemUri)
{
    return NOERROR;
}
    
ECode CContentProviderOne::Query(
    /* [in] */ IUri* uri, 
    /* [in] */ const ArrayOf<String>& projection, 
    /* [in] */ String selection, 
    /* [in] */ const ArrayOf<String>& selectionArgs, 
    /* [in] */ String sortOrder,
    /* [out] */ ICursor** cursor)
{
    return NOERROR;
}
    
ECode CContentProviderOne::Update(
    /* [in] */ IUri* uri, 
    /* [in] */ IContentValues* values, 
    /* [in] */ String selection, 
    /* [in] */ const ArrayOf<String>& selectionArgs,
    /* [out] */ Int32* rowsAffected)
{
    return NOERROR;
}
