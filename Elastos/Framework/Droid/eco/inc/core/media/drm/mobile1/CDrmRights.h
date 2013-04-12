
#ifndef __CDRMRIGHTS_H__
#define __CDRMRIGHTS_H__

#include "_CDrmRights.h"
#include "CDrmConstraintInfo.h"

CarClass(CDrmRights)
{
public:
    CDrmRights();

    CARAPI GetConstraint(
        /*[in] */ Int32 permission,
        /*[out] */ IDrmConstraintInfo** constraint);

    CARAPI ConsumeRights(
        /* [in] */ Int32 permission,
        /* [out] */ Boolean* result);

private:
    /**
     * native method: get the constraint information of the given permission.
     *
     * @param permission    the given permission.
     * @param constraint    the instance of constraint.
     *
     * @return #JNI_DRM_SUCCESS if succeed.
     *         #JNI_DRM_FAILURE if fail.
     */
    CARAPI_(Int32) NativeGetConstraintInfo(
        /* [in] */ Int32 permission,
        /* [in] */ CDrmConstraintInfo* constraint);

    CARAPI_(Int32) NativeConsumeRights(
        /* [in] */ Int32 permission);

public:
    /**
     * Successful operation.
     */
    static const Int32 JNI_DRM_SUCCESS = 0;

    /**
     * General failure.
     */
    static const Int32 JNI_DRM_FAILURE = -1;

    /**
     * The uid of this rights object.
     */
    String mRoId;
};

#endif // __CDRMRIGHTS_H__
