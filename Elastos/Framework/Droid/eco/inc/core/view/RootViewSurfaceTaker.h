#ifndef __ROOTVIEWSURFACETAKER_H__
#define __ROOTVIEWSURFACETAKER_H__

#include "ext/frameworkext.h"

extern "C" const InterfaceID EIID_RootViewSurfaceTaker;

interface RootViewSurfaceTaker
{
    virtual CARAPI WillYouTakeTheSurface(
        /* [out] */ ISurfaceHolderCallback2** cback) = 0;

    virtual CARAPI SetSurfaceType(
        /* [in] */ Int32 type) = 0;

    virtual CARAPI SetSurfaceFormat(
        /* [in] */ Int32 format) = 0;

    virtual CARAPI SetSurfaceKeepScreenOn(
        /* [in] */ Boolean keepOn) = 0;

    virtual CARAPI WillYouTakeTheInputQueue(
        /* [out] */ IInputQueueCallback** inputQueueCallback) = 0;
};

#endif //__ROOTVIEWSURFACETAKER_H__
