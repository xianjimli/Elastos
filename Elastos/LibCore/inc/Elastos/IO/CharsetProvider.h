#ifndef __CHARSETPROVIDER_H__
#define __CHARSETPROVIDER_H__

#include <elastos.h>
#include <Elastos.IO.h>

using namespace Elastos;

class CharsetProvider
{
public:
    CharsetProvider();

    // TODO: 
    // virtual CARAPI Charsets(
    //      /* [out] */ Iterator<ICharset>** charset) = 0;

    virtual CARAPI CharsetForName(
        /* [in] */ const String& charsetName,
        /* [out] */ ICharset** charset) = 0;

private:
    // The permission required to construct a new provider.
    // TODO:
    // static const RuntimePermission CONSTRUCT_PERM
    //     = new RuntimePermission("charsetProvider");
};

#endif //__CHARSETPROVIDER_H__