
#ifndef __DEFAULTFILENAMEMAP_H__
#define __DEFAULTFILENAMEMAP_H__

#include "cmdef.h"
#include "Elastos.Net_server.h"
#include <elastos/ElRefBase.h>

/**
 * Implements {@link FileNameMap} in terms of {@link libcore.net.MimeUtils}.
 */
class DefaultFileNameMap: public ElRefBase, public IFileNameMap
{
public:
    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI GetContentTypeFor(
        /* [in] */ const String& filename,
        /* [out] */ String* type);
};

#endif //__DEFAULTFILENAMEMAP_H__
