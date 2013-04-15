
#ifndef  _CSURFACEVIEWWINDOW_H__
#define  _CSURFACEVIEWWINDOW_H__

#include "_CSurfaceViewWindow.h"
#include "view/SurfaceView.h"
#include <elastos/AutoPtr.h>

CarClass(CSurfaceViewWindow)
{
public:
	CSurfaceViewWindow();

    CARAPI constructor(
        /* [in] */ Handle32 surfaceView);

    CARAPI Resized(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ IRect* coveredInsets,
        /* [in] */ IRect* visibleInsets,
        /* [in] */ Boolean reportDraw,
        /* [in] */ IConfiguration* newConfig);

    CARAPI DispatchAppVisibility(
        /* [in] */ Boolean visible);

    CARAPI DispatchGetNewSurface();

    CARAPI WindowFocusChanged(
        /* [in] */ Boolean hasFocus,
        /* [in] */ Boolean touchEnabled);

    CARAPI ExecuteCommand(
        /* [in] */ const String& command,
        /* [in] */ const String& parameters,
        /* [in] */ IParcelFileDescriptor* out);

    CARAPI CloseSystemDialogs(
        /* [in] */ const String& reason);

    CARAPI DispatchWallpaperOffsets(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Float xStep,
        /* [in] */ Float yStep,
        /* [in] */ Boolean sync);

    CARAPI DispatchWallpaperCommand(
        /* [in] */ const String& action,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 z,
        /* [in] */ IBundle* extras,
        /* [in] */ Boolean sync);

    CARAPI GetDescription(
            /* [out] */ String* description);

private:
    SurfaceView* mSurfaceView;

    Int32 mCurWidth;// = -1;
    Int32 mCurHeight;// = -1;
};

#endif  //_CSURFACEVIEWWINDOW_H__
