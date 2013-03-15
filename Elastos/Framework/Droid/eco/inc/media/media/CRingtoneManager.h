
#ifndef __CRINGTONEMANAGER_H__
#define __CRINGTONEMANAGER_H__

#include "_CRingtoneManager.h"

#include "media/RingtoneManager.h"

CarClass(CRingtoneManager), public RingtoneManager
{
public:
    CARAPI SetType(
        /* [in] */ Int32 type);

    CARAPI InferStreamType(
        /* [out] */ Int32 * pType);

    CARAPI SetStopPreviousRingtone(
        /* [in] */ Boolean stopPreviousRingtone);

    CARAPI GetStopPreviousRingtone(
        /* [out] */ Boolean * pStopPreviousRingtone);

    CARAPI StopPreviousRingtone();

    CARAPI GetIncludeDrm(
        /* [out] */ Boolean * pIncluded);

    CARAPI SetIncludeDrm(
        /* [in] */ Boolean includeDrm);

    CARAPI GetCursor(
        /* [out] */ ICursor ** ppCurse);

    CARAPI GetRingtone(
        /* [in] */ Int32 position,
        /* [out] */ IRingtone ** ppRingtone);

    CARAPI GetRingtoneUri(
        /* [in] */ Int32 position,
        /* [out] */ IUri ** ppUri);

    CARAPI GetRingtonePosition(
        /* [in] */ IUri * pRingtoneUri,
        /* [out] */ Int32 * pPos);

    CARAPI GetValidRingtoneUri(
        /* [in] */ IContext * pContext,
        /* [out] */ IUri ** ppUri);

    CARAPI GetActualDefaultRingtoneUri(
        /* [in] */ IContext * pContext,
        /* [in] */ Int32 type,
        /* [out] */ IUri ** ppUri);

    CARAPI SetActualDefaultRingtoneUri(
        /* [in] */ IContext * pContext,
        /* [in] */ Int32 type,
        /* [in] */ IUri * pRingtoneUri);

    CARAPI IsDefault(
        /* [in] */ IUri * pRingtoneUri,
        /* [out] */ Boolean * pDefault);

    CARAPI GetDefaultType(
        /* [in] */ IUri * pDefaultRingtoneUri,
        /* [out] */ Int32 * pType);

    CARAPI GetDefaultUri(
        /* [in] */ Int32 type,
        /* [out] */ IUri ** ppUri);

    CARAPI constructor(
        /* [in] */ IActivity * pActivity);

    CARAPI constructor(
        /* [in] */ IContext * pContext);

private:
    // TODO: Add your private member variables here.
};

#endif // __CRINGTONEMANAGER_H__
