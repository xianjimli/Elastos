
#ifndef __CAPP_H__
#define __CAPP_H__

#include <elastos.h>
#include "_CApp.h"
#include "GeckoApp.h"

class CApp : public GeckoApp
{
public:
    ECode GetPackageName(
        /* [out] */ String* pPackageName);

    ECode GetContentProcessName(
        /* [out] */ String* pProcessName);
};

#endif //__CAPP_H__

