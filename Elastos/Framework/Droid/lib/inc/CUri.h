
#ifndef __CURI_H__
#define __CURI_H__

#include "Elastos.Framework.h"

using namespace Elastos;

class CUri
{
public:
    static ECode Parse(String uriString, IUri **ppUri)
    {
        return CStringUri::New(uriString, ppUri);
    }
};

#endif //__CURI_H__
