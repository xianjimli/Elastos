
#ifndef __CCONTENTVALUES_H__
#define __CCONTENTVALUES_H__

#include "_CContentValues.h"

CarClass(CContentValues)
{
public:
    CARAPI PutInt32(
        /* [in] */ String key,
        /* [in] */ Int32 value);
        
    CARAPI GetInt32(
        /* [in] */ String key,
        /* [out] */ Int32* value);
    
    CARAPI constructor();
    
public:
 
};

#endif //__CCONTENTVALUES_H__
