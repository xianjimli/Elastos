
#ifndef __CCONTENTVALUES_H__
#define __CCONTENTVALUES_H__

#include "_CContentValues.h"

CarClass(CContentValues)
{
public:
    CARAPI PutInt32(
        /* [in] */ const String& key,
        /* [in] */ Int32 value);

    CARAPI GetInt32(
        /* [in] */ const String& key,
        /* [out] */ Int32* value);

    CARAPI GetSize(
        /* [out] */ Int32* size);

    CARAPI ValueSet(
        /* [out] */ IObjectStringMap** values);

    CARAPI constructor();

public:

};

#endif //__CCONTENTVALUES_H__
