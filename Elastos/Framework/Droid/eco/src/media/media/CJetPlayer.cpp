
#include "media/CJetPlayer.h"

ECode CJetPlayer::Clone(
    /* [out] */ IInterface ** ppJetPlayer)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CJetPlayer::ReleaseEx()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CJetPlayer::GetMaxTracks(
    /* [out] */ Int32 * pTracks)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CJetPlayer::LoadJetFile(
    /* [in] */ const String& path,
    /* [out] */ Boolean * pRes)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CJetPlayer::LoadJetFileEx(
    /* [in] */ IAssetFileDescriptor * pAfd,
    /* [out] */ Boolean * pRes)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CJetPlayer::CloseJetFile(
    /* [out] */ Boolean * pClosed)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CJetPlayer::Play(
    /* [out] */ Boolean * pStarted)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CJetPlayer::Pause(
    /* [out] */ Boolean * pPaused)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CJetPlayer::QueueJetSegment(
    /* [in] */ Int32 segmentNum,
    /* [in] */ Int32 libNum,
    /* [in] */ Int32 repeatCount,
    /* [in] */ Int32 transpose,
    /* [in] */ Int32 muteFlags,
    /* [in] */ Byte userID,
    /* [out] */ Boolean * pQueued)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CJetPlayer::QueueJetSegmentMuteArray(
    /* [in] */ Int32 segmentNum,
    /* [in] */ Int32 libNum,
    /* [in] */ Int32 repeatCount,
    /* [in] */ Int32 transpose,
    /* [in] */ const ArrayOf<Boolean> & muteArray,
    /* [in] */ Byte userID,
    /* [out] */ Boolean * pQueued)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CJetPlayer::SetMuteFlags(
    /* [in] */ Int32 muteFlags,
    /* [in] */ Boolean sync,
    /* [out] */ Boolean * pFlags)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CJetPlayer::SetMuteArray(
    /* [in] */ const ArrayOf<Boolean> & muteArray,
    /* [in] */ Boolean sync,
    /* [out] */ Boolean * pFalgs)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CJetPlayer::SetMuteFlag(
    /* [in] */ Int32 trackId,
    /* [in] */ Boolean muteFlag,
    /* [in] */ Boolean sync,
    /* [out] */ Boolean * pUpdated)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CJetPlayer::TriggerClip(
    /* [in] */ Int32 clipId,
    /* [out] */ Boolean * pTrigger)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CJetPlayer::ClearQueue(
    /* [out] */ Boolean * pCleared)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CJetPlayer::SetEventListener(
    /* [in] */ IOnJetEventListener * pListener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CJetPlayer::SetEventListenerEx(
    /* [in] */ IOnJetEventListener * pListener,
    /* [in] */ IHandler * pHandler)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CJetPlayer::constructor()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

