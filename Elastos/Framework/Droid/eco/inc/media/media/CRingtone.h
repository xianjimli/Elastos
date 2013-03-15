
#ifndef __CRINGTONE_H__
#define __CRINGTONE_H__

#include "_CRingtone.h"

#include "media/Ringtone.h"

CarClass(CRingtone), public Ringtone
{
public:
    CARAPI SetStreamType(
        /* [in] */ Int32 streamType);

    CARAPI GetStreamType(
        /* [out] */ Int32 * pType);

    CARAPI GetTitle(
        /* [in] */ IContext * pContext,
        /* [out] */ String * pTitle);

    CARAPI Play();

    CARAPI Stop();

    CARAPI IsPlaying(
        /* [out] */ Boolean * pPlaying);

    CARAPI constructor(
        /* [in] */ IContext * pContext);

private:
    // TODO: Add your private member variables here.
};

#endif // __CRINGTONE_H__
