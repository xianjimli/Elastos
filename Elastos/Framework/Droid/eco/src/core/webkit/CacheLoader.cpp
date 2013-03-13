
#include "webkit/CacheLoader.h"
#include "webkit/LoadListener.h"

CacheLoader::CacheLoader(
	/* [in] */ LoadListener* loadListener,
	/* [in] */ CCacheManager::CacheResult* result) : StreamLoader(loadListener)
{}

//@Override
CARAPI_(Boolean) CacheLoader::SetupStreamAndSendStatus()
{}

//@Override
CARAPI_(void) CacheLoader::BuildHeaders(
	/* [in] */ IHeaders* headers)
{}