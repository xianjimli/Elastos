
#ifndef __ATTRIBUTECACHE_H__
#define __ATTRIBUTECACHE_H__

#include "ext/frameworkext.h"

class AttributeCache
{
public:
    static CARAPI_(AttributeCache*) GetInstance();

    CARAPI_(void) UpdateConfiguration(
        /* [in] */ IConfiguration* config);
};

#endif // __ATTRIBUTECACHE_H__
