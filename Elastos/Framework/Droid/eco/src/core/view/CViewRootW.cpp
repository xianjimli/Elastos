
#include "view/CViewRootW.h"

ECode CViewRootW::constructor(
    /* [in] */ Handle32 viewRoot,
    /* [in] */ IContext* ctx)
{
    mViewRoot = (ViewRoot*)viewRoot;

    return NOERROR;
}

ECode CViewRootW::Resized(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ IRect* coveredInsets,
    /* [in] */ IRect* visibleInsets,
    /* [in] */ Boolean reportDraw,
    /* [in] */ IConfiguration* newConfig)
{
    if (mViewRoot != NULL) {
        mViewRoot->DispatchResized(w, h, coveredInsets,
            visibleInsets, reportDraw, newConfig);
    }

    return NOERROR;
}

ECode CViewRootW::DispatchAppVisibility(
    /* [in] */ Boolean visible)
{
    if (mViewRoot != NULL) {
        mViewRoot->DispatchAppVisibility(visible);
    }

    return NOERROR;
}

ECode CViewRootW::DispatchGetNewSurface()
{
    if (mViewRoot != NULL) {
        mViewRoot->DispatchGetNewSurface();
    }

    return NOERROR;
}

ECode CViewRootW::WindowFocusChanged(
    /* [in] */ Boolean hasFocus,
    /* [in] */ Boolean inTouchMode)
{
    if (mViewRoot != NULL) {
        mViewRoot->WindowFocusChanged(hasFocus, inTouchMode);
    }

    return NOERROR;
}

ECode CViewRootW::ExecuteCommand(
    /* [in] */ const String& command,
    /* [in] */ const String& parameters,
    /* [in] */ IParcelFileDescriptor* descriptor)
{
//    final ViewRoot viewRoot = mViewRoot.get();
//    if (viewRoot != null) {
//        final View view = viewRoot.mView;
//        if (view != null) {
//            if (checkCallingPermission(Manifest.permission.DUMP) !=
//                    PackageManager.PERMISSION_GRANTED) {
//                throw new SecurityException("Insufficient permissions to invoke"
//                        + " executeCommand() from pid=" + Binder.getCallingPid()
//                        + ", uid=" + Binder.getCallingUid());
//            }
//
//            OutputStream clientStream = null;
//            try {
//                clientStream = new ParcelFileDescriptor.AutoCloseOutputStream(out);
//                ViewDebug.dispatchCommand(view, command, parameters, clientStream);
//            } catch (IOException e) {
//                e.printStackTrace();
//            } finally {
//                if (clientStream != null) {
//                    try {
//                        clientStream.close();
//                    } catch (IOException e) {
//                        e.printStackTrace();
//                    }
//                }
//            }
//        }
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CViewRootW::CloseSystemDialogs(
    /* [in] */ const String& reason)
{
    if (mViewRoot != NULL) {
        mViewRoot->DispatchCloseSystemDialogs(reason);
    }

    return NOERROR;
}

ECode CViewRootW::DispatchWallpaperOffsets(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float xStep,
    /* [in] */ Float yStep,
    /* [in] */ Boolean sync)
{
    if (sync) {
        return ViewRoot::sWindowSession->WallpaperOffsetsComplete(
            (IBinder*)this);
    }

    return NOERROR;
}

ECode CViewRootW::DispatchWallpaperCommand(
    /* [in] */ const String& action,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 z,
    /* [in] */ IBundle* extras,
    /* [in] */ Boolean sync)
{
    if (sync) {
        return ViewRoot::sWindowSession->WallpaperCommandComplete(
            (IBinder*)this, NULL);
    }

    return NOERROR;
}

ECode CViewRootW::GetDescription(
    /* [out] */ String* description)
{
    return E_NOT_IMPLEMENTED;
}
