
#include "webkit/ContentLoader.h"

ContentLoader::ContentLoader(
	/* [in] */ const String& rawUrl, 
	/* [in] */ LoadListener* loadListener) : StreamLoader(loadListener)
{}

//@Override
CARAPI_(Boolean) ContentLoaderSetupStreamAndSendStatus()
{}

//@Override
CARAPI_(void) ContentLoaderBuildHeaders(
	/* [in] */ IHeaders* headers)
{}