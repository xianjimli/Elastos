
#ifndef __CJETPLAYER_H__
#define __CJETPLAYER_H__

#include "_CJetPlayer.h"

#include "media/JetPlayer.h"

CarClass(CJetPlayer), public JetPlayer
{
public:
    CARAPI Clone(
        /* [out] */ IInterface ** ppJetPlayer);

    CARAPI ReleaseResources();

    CARAPI GetMaxTracks(
        /* [out] */ Int32 * pTracks);

    CARAPI LoadJetFile(
        /* [in] */ const String& path,
        /* [out] */ Boolean * pRes);

    CARAPI LoadJetFileEx(
        /* [in] */ IAssetFileDescriptor * pAfd,
        /* [out] */ Boolean * pRes);

    CARAPI CloseJetFile(
        /* [out] */ Boolean * pClosed);

    CARAPI Play(
        /* [out] */ Boolean * pStarted);

    CARAPI Pause(
        /* [out] */ Boolean * pPaused);

    CARAPI QueueJetSegment(
        /* [in] */ Int32 segmentNum,
        /* [in] */ Int32 libNum,
        /* [in] */ Int32 repeatCount,
        /* [in] */ Int32 transpose,
        /* [in] */ Int32 muteFlags,
        /* [in] */ Byte userID,
        /* [out] */ Boolean * pQueued);

    CARAPI QueueJetSegmentMuteArray(
        /* [in] */ Int32 segmentNum,
        /* [in] */ Int32 libNum,
        /* [in] */ Int32 repeatCount,
        /* [in] */ Int32 transpose,
        /* [in] */ const ArrayOf<Boolean> & muteArray,
        /* [in] */ Byte userID,
        /* [out] */ Boolean * pQueued);

    CARAPI SetMuteFlags(
        /* [in] */ Int32 muteFlags,
        /* [in] */ Boolean sync,
        /* [out] */ Boolean * pFlags);

    CARAPI SetMuteArray(
        /* [in] */ const ArrayOf<Boolean> & muteArray,
        /* [in] */ Boolean sync,
        /* [out] */ Boolean * pFalgs);

    CARAPI SetMuteFlag(
        /* [in] */ Int32 trackId,
        /* [in] */ Boolean muteFlag,
        /* [in] */ Boolean sync,
        /* [out] */ Boolean * pUpdated);

    CARAPI TriggerClip(
        /* [in] */ Int32 clipId,
        /* [out] */ Boolean * pTrigger);

    CARAPI ClearQueue(
        /* [out] */ Boolean * pCleared);

    CARAPI SetEventListener(
        /* [in] */ IOnJetEventListener * pListener);

    CARAPI SetEventListenerEx(
        /* [in] */ IOnJetEventListener * pListener,
        /* [in] */ IApartment * pHandler);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

#endif // __CJETPLAYER_H__
