
#ifndef __CVIEWROOTW_H__
#define __CVIEWROOTW_H__

#include "_CViewRootW.h"
#include "view/ViewRoot.h"

class ViewRoot;

CarClass(CViewRootW)
{
public:
    CARAPI constructor(
        /* [in] */ Handle32 viewRoot,
        /* [in] */ IContext* ctx);

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
        /* [in] */ Boolean inTouchMode);

    CARAPI ExecuteCommand(
        /* [in] */ const String& command,
        /* [in] */ const String& parameters,
        /* [in] */ IParcelFileDescriptor* descriptor);

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
    static CARAPI_(Int32) CheckCallingPermission(
        /* [in] */ const String& permission);

private:
    AutoPtr<ViewRoot> mViewRoot;
};

#endif //__CVIEWROOTW_H__
