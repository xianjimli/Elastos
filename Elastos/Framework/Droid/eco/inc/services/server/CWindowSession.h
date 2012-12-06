
#ifndef __CWINDOWSESSION_H__
#define __CWINDOWSESSION_H__

#include "_CWindowSession.h"
#include "server/CWindowManagerService.h"
#include <elastos/AutoPtr.h>

CarClass(CWindowSession)
{
public:
    CARAPI constructor(
        /* [in] */ IWindowManagerEx* wmService,
        /* [in] */ IInputMethodClientStub* client,
        /* [in] */ IInputContextStub* inputContext);

    CARAPI Add(
        /* [in] */ IInnerWindow* window,
        /* [in] */ IWindowManagerLayoutParams* attrs,
        /* [in] */ Int32 viewVisibility,
        /* [in] */ IRect* inContentInsets,
        /* [out] */ IRect** outContentInsets,
        /* [in] */ IInputChannel* inInputChannel,
        /* [out] */ IInputChannel** outInputChannel,
        /* [out] */ Int32* result);

    CARAPI AddWithoutInputChannel(
        /* [in] */ IInnerWindow* window,
        /* [in] */ IWindowManagerLayoutParams* attrs,
        /* [in] */ Int32 viewVisibility,
        /* [out] */ IRect** outContentInsets,
        /* [out] */ Int32* result);

    CARAPI Remove(
        /* [in] */ IInnerWindow* window);

    CARAPI Relayout(
        /* [in] */ IInnerWindow* window,
        /* [in] */ IWindowManagerLayoutParams* attrs,
        /* [in] */ Int32 requestedWidth,
        /* [in] */ Int32 requestedHeight,
        /* [in] */ Int32 viewFlags,
        /* [in] */ Boolean insetsPending,
        /* [in] */ IRect* inFrame,
        /* [in] */ IRect* inContentInsets,
        /* [in] */ IRect* inVisibleInsets,
        /* [in] */ IConfiguration* inConfig,
        /* [in] */ ISurface* inSurface,
        /* [out] */ IRect** outFrame,
        /* [out] */ IRect** outContentInsets,
        /* [out] */ IRect** outVisibleInsets,
        /* [out] */ IConfiguration** outConfig,
        /* [out] */ Int32* result,
        /* [out] */ ISurface** outSurface);

    CARAPI SetTransparentRegion(
        /* [in] */ IInnerWindow* window,
        /* [in] */ IRegion* region);

    CARAPI SetInsets(
        /* [in] */ IInnerWindow* window,
        /* [in] */ Int32 touchableInsets,
        /* [in] */ IRect* contentInsets,
        /* [in] */ IRect* visibleInsets);

    CARAPI GetDisplayFrame(
        /* [in] */ IInnerWindow* window,
        /* [out] */ IRect** outDisplayFrame);

    CARAPI FinishDrawing(
        /* [in] */ IInnerWindow* window);

    CARAPI SetInTouchMode(
        /* [in] */ Boolean showFocus);

    CARAPI GetInTouchMode(
        /* [out] */ Boolean* result);

    CARAPI PerformHapticFeedback(
        /* [in] */ IInnerWindow* window,
        /* [in] */ Int32 effectId,
        /* [in] */ Boolean always,
        /* [out] */ Boolean* result);

    CARAPI SetWallpaperPosition(
        /* [in] */ IBinder* windowToken,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Float xstep,
        /* [in] */ Float ystep);

    CARAPI WallpaperOffsetsComplete(
        /* [in] */ IBinder* window);

    CARAPI SendWallpaperCommand(
        /* [in] */ IBinder* window,
        /* [in] */ const String& action,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 z,
        /* [in] */ IBundle* extras,
        /* [in] */ Boolean sync,
        /* [out] */ IBundle** result);

    CARAPI WallpaperCommandComplete(
        /* [in] */ IBinder* window,
        /* [in] */ IBundle* result);

public:
    CARAPI_(void) WindowAddedLocked();

public:
    AutoPtr<CWindowManagerService> mWMService;
    AutoPtr<IInputMethodClientStub> mClient;
    AutoPtr<IInputContextStub> mInputContext;
    Int32 mUid;
    Int32 mPid;
    AutoPtr<ISurfaceSession> mSurfaceSession;
    Int32 mNumWindow;
};

#endif //__CWINDOWSESSION_H__
