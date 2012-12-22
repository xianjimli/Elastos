
#include "location/CGpsStatus.h"
#include "location/CGpsSatellite.h"

const Int32 CGpsStatus::NUM_SATELLITES;

CGpsStatus::CGpsStatus()
    : mTimeToFirstFix(0)
{
    ASSERT_SUCCEEDED(CObjectContainer::New((IObjectContainer**)&mSatellites));
    mSatellites->GetObjectEnumerator((IObjectEnumerator**)&mSatellitesEnum);
}

ECode CGpsStatus::SetStatus(
    /* [in] */ Int32 svCount,
    /* [in] */ const ArrayOf<Int32>& prns,
    /* [in] */ const ArrayOf<Float>& snrs,
    /* [in] */ const ArrayOf<Float>& elevations,
    /* [in] */ const ArrayOf<Float>& azimuths,
    /* [in] */ Int32 ephemerisMask,
    /* [in] */ Int32 almanacMask,
    /* [in] */ Int32 usedInFixMask)
{
    Boolean hasNext = FALSE;
    while(mSatellitesEnum->MoveNext(&hasNext), hasNext) {
        AutoPtr<IInterface> itf;
        mSatellitesEnum->Current((IInterface**)&itf);
        AutoPtr<CGpsSatellite> satellite;
        satellite = (CGpsSatellite*)(IGpsSatellite*)(IInterface*)itf;
        satellite->mValid = FALSE;
    }

    for (Int32 i = 0; i < svCount; i++) {
        Int32 prn = prns[i] - 1;
        Int32 prnShift = (1 << prn);
        Int32 count;
        mSatellites->GetObjectCount(&count);
        if (prn >= 0 && prn < count) {
            mSatellitesEnum->Reset();
            for (Int32 j = prn; j >= 0; j--) {
                mSatellitesEnum->MoveNext(&hasNext);
            }
            AutoPtr<IInterface> itf;
            mSatellitesEnum->Current((IInterface**)&itf);
            AutoPtr<CGpsSatellite> satellite;
            satellite = (CGpsSatellite*)(IGpsSatellite*)(IInterface*)itf;

            satellite->mValid = TRUE;
            satellite->mSnr = snrs[i];
            satellite->mElevation = elevations[i];
            satellite->mAzimuth = azimuths[i];
            satellite->mHasEphemeris = ((ephemerisMask & prnShift) != 0);
            satellite->mHasAlmanac = ((almanacMask & prnShift) != 0);
            satellite->mUsedInFix = ((usedInFixMask & prnShift) != 0);
        }
    }

    return NOERROR;
}

ECode CGpsStatus::SetStatusEx(
    /* [in] */ IGpsStatus* status)
{
    status->GetTimeToFirstFix(&mTimeToFirstFix);

    AutoPtr<CGpsStatus> statusSrc = (CGpsStatus*)status;

    Boolean hasNext, hasNextSrc = FALSE;
    while(mSatellitesEnum->MoveNext(&hasNext), hasNext,
            statusSrc->mSatellitesEnum->MoveNext(&hasNextSrc), hasNextSrc) {
        AutoPtr<IInterface> itf, itfSrc;
        mSatellitesEnum->Current((IInterface**)&itf);
        statusSrc->mSatellitesEnum->Current((IInterface**)&itfSrc);
        AutoPtr<IGpsSatellite> satellite = (IGpsSatellite*)(IInterface*)itf;
        satellite->SetStatus((IGpsSatellite*)(IInterface*)itfSrc);
    }

    return NOERROR;
}

ECode CGpsStatus::SetTimeToFirstFix(
    /* [in] */ Int32 ttff)
{
    mTimeToFirstFix = ttff;

    return NOERROR;
}

ECode CGpsStatus::GetTimeToFirstFix(
    /* [out] */ Int32* time)
{
    VALIDATE_NOT_NULL(time);
    *time = mTimeToFirstFix;

    return NOERROR;
}

ECode CGpsStatus::GetSatellites(
    /* [out] */ IObjectEnumerator** satellites)
{
    VALIDATE_NOT_NULL(satellites);
    *satellites = mSatellitesEnum;

    return NOERROR;
}

ECode CGpsStatus::GetMaxSatellites(
    /* [out] */ Int32* maxNumber)
{
    VALIDATE_NOT_NULL(maxNumber);
    *maxNumber = NUM_SATELLITES;

    return NOERROR;
}

ECode CGpsStatus::constructor()
{
    for (Int32 i = 0; i < NUM_SATELLITES; i++) {
        AutoPtr<IGpsSatellite> satellite;
        ASSERT_SUCCEEDED(CGpsSatellite::New(i + 1, (IGpsSatellite**)&satellite));
        mSatellites->Add((IInterface*)satellite);
    }

    return NOERROR;
}


