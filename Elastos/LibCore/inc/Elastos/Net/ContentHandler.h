
#ifndef __CONTENTHANDLER_H__
#define __CONTENTHANDLER_H__

#include "Elastos.Net_server.h"

/**
 * This class converts the content of a certain format (i.e. a MIME type) into a
 * Java type object. It is created by {@code ContentHandlerFactory}. The data
 * values should be accessed via {@code URL} or {@code URLConnection}.
 *
 * @see ContentHandlerFactory
 * @see URL#getContent()
 * @see URLConnection#getContent()
 */
class ContentHandler
{
public:
    virtual CARAPI GetContent(
        /* [in] */ IURLConnection* uConn,
        /* [out] */ IInterface** obj) = 0;

    virtual CARAPI GetContentEx(
        /* [in] */ IURLConnection* uConn,
        /* [in] */ const ArrayOf<IInterface*>& types,
        /* [out] */ IInterface** obj);
};

#endif //__CONTENTHANDLER_H__
