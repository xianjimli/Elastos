
#ifndef __MASKFILTER_H__
#define __MASKFILTER_H__

#include <elastos.h>
#include <SkMaskFilter.h>

using namespace Elastos;

extern "C" const InterfaceID EIID_MaskFilter;

/**
 * MaskFilter is the base class for object that perform transformations on
 * an alpha-channel mask before drawing it. A subclass of MaskFilter may be
 * installed into a Paint. Blur and emboss are implemented as subclasses of MaskFilter.
 */
class MaskFilter
{
    friend class Paint;

protected:
    ~MaskFilter();

private:
    static CARAPI_(void) NativeDestructor(
        /* [in] */ SkMaskFilter* nObj);

private:
    SkMaskFilter* mNativeInstance;
};

#endif //__MASKFILTER_H__
