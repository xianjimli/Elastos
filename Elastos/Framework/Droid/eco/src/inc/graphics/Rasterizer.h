
#ifndef __RASTERIZER_H__
#define __RASTERIZER_H__

#include <elastos.h>
#include <SkRasterizer.h>

using namespace Elastos;

extern "C" const InterfaceID EIID_Rasterizer;

class Rasterizer
{
    friend class Paint;

protected:
    ~Rasterizer();

private:
    static CARAPI_(void) Finalizer(
        /* [in] */ SkRasterizer* nObj);

private:
    SkRasterizer* mNativeInstance;
};

#endif //__RASTERIZER_H__
