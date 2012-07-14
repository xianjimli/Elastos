
#ifndef __CCONTENTPROVIDERONE_H__
#define __CCONTENTPROVIDERONE_H__

#include "CContentProvider.h"
#include "_CContentProviderOne.h"

using namespace Elastos;

class CContentProviderOne : public CContentProvider
{
public:
    CARAPI GetClassID(
        /* [out] */ ClassID *pCLSID);
    
    CARAPI Delete(
        /* [in] */ IUri* uri, 
        /* [in] */ const String& selection, 
        /* [in] */ const ArrayOf<String>& selectionArgs,
        /* [out] */ Int32* rowsAffected);
    
    CARAPI GetType(
        /* [in] */ IUri* uri,
        /* [out] */ String* type);
     
    CARAPI Insert(
        /* [in] */ IUri* uri, 
        /* [in] */ IContentValues* values,
        /* [out] */ IUri** insertedItemUri); 
    
    CARAPI Query(
        /* [in] */ IUri* uri, 
        /* [in] */ const ArrayOf<String>& projection, 
        /* [in] */ const String& selection, 
        /* [in] */ const ArrayOf<String>& selectionArgs, 
        /* [in] */ const String& sortOrder,
        /* [out] */ ICursor** cursor);
    
    CARAPI Update(
        /* [in] */ IUri* uri, 
        /* [in] */ IContentValues* values, 
        /* [in] */ const String& selection, 
        /* [in] */ const ArrayOf<String>& selectionArgs,
        /* [out] */ Int32* rowsAffected);
};

#endif //__CCONTENTPROVIDERONE_H__
