
#include "webkit/DataLoader.h"
#include "webkit/LoadListener.h"

#include <stdlib.h>

DataLoader::DataLoader(
	/* [in] */ const String& dataUrl, 
	/* [in] */ LoadListener* loadListener) : StreamLoader(loadListener)
{

	assert(loadListener);

    String url = dataUrl.Substring(strlen("data:"));
    
    Byte* data = NULL;
    Int32 commaIndex = url.IndexOf(',');
    if (commaIndex != -1) {
        String contentType = url.Substring(0, commaIndex);
//        data = url.Substring(commaIndex + 1).GetBytes();
//        loadListener->ParseContentTypeHeader(contentType);
//        if ("base64".equals(loadListener.transferEncoding())) {
//            data = Base64.decode(data);
//        }
    } else {
//        data = url.getBytes();
    }

    if (data != NULL) {
//        mDataStream = new ByteArrayInputStream(data);
//        mContentLength = data->length;
    }

}

//@Override
CARAPI_(Boolean) DataLoader::SetupStreamAndSendStatus()
{
	if (mDataStream != NULL) {
        mLoadListener->Status(1, 1, 200, (String)"OK");
        return TRUE;
    } else {
//        mLoadListener->Error(EventHandler.ERROR,
//                mContext->GetString(R.string.httpError));
        return FALSE;
    }
}

//@Override
CARAPI_(void) DataLoader::BuildHeaders(
	/* [in] */ IHeaders* h)
{}