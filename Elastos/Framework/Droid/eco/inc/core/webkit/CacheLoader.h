#ifndef __CACHELOADER_H__
#define __CACHELOADER_H__

#include "ext/frameworkext.h"
#include "StreamLoader.h"
#include "CCacheManager.h"

class LoadListener;

class CacheLoader : public StreamLoader
{
public:
    /**
     * Constructs a CacheLoader for use when loading content from the cache.
     *
     * @param loadListener LoadListener to pass the content to
     * @param result CacheResult used as the source for the content.
     */
    CacheLoader(
    	/* [in] */ LoadListener* loadListener,
    	/* [in] */ CCacheManager::CacheResult* result);

public:
	CCacheManager::CacheResult* mCacheResult;  // Content source

protected:

    //@Override
	CARAPI_(Boolean) SetupStreamAndSendStatus();

    //@Override
	CARAPI_(void) BuildHeaders(
		/* [in] */ IHeaders* headers);
};

#endif //__CACHELOADER_H__