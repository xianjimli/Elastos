
#ifndef __CWORKSOURCE_H__
#define __CWORKSOURCE_H__

#include "_CWorkSource.h"
#include "ext/frameworkdef.h"

CarClass(CWorkSource)
{
public:
    CWorkSource();

    CARAPI Size(
        /* [out] */ Int32* size);

    CARAPI Get(
        /* [in] */ Int32 index,
        /* [out] */ Int32* uid);

    CARAPI Clear();

    CARAPI Diff(
        /* [in] */ IWorkSource* other,
        /* [out] */ Boolean* result);

    CARAPI Set(
        /* [in] */ IWorkSource* other);

    CARAPI SetEx(
        /* [in] */ Int32 uid);

    CARAPI SetReturningDiffs(
        /* [in] */ IWorkSource* other,
        /* [out, callee] */ ArrayOf<IWorkSource>** diffs);

    CARAPI Add(
        /* [in] */ IWorkSource* other,
        /* [out] */ Boolean* result);

    CARAPI AddReturningNewbs(
        /* [in] */ IWorkSource* other,
        /* [out] */ IWorkSource** result);

    CARAPI AddEx(
        /* [in] */ Int32 uid,
        /* [out] */ Boolean* result);

    CARAPI AddReturningNewbsEx(
        /* [in] */ Int32 uid,
        /* [out] */ IWorkSource** result);

    CARAPI Remove(
        /* [in] */ IWorkSource* other,
        /* [out] */ Boolean* result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IWorkSource* orig);

    CARAPI constructor(
        /* [in] */ Int32 uid);

    CARAPI constructor(
        /* [in] */ IParcel* src);

public:
    Int32 mNum;
    ArrayOf<Int32>* mUids;
};

#endif // __CWorkSource_H__
