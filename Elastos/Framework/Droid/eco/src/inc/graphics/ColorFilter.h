
#ifndef __COLORFILTER_H__
#define __COLORFILTER_H__

#include <elastos.h>
#include <SkColorFilter.h>

using namespace Elastos;

extern "C" const InterfaceID EIID_ColorFilter;

class ColorFilter
{
    friend class Paint;

protected:
    ~ColorFilter();

private:
    static CARAPI_(void) Finalizer(
        /* [in] */ SkColorFilter* nObj);

private:
    SkColorFilter* mNativeInstance;
};

#endif //__COLORFILTER_H__
