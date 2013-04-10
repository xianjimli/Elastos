
#ifndef __CDRMRIGHTSMANAGER_H__
#define __CDRMRIGHTSMANAGER_H__

#include "_CDrmRightsManager.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>
#include <Elastos.IO.h>
#include "CDrmRights.h"

CarClass(CDrmRightsManager)
{
public:
    CARAPI InstallRights(
        /* [in] */ IInputStream* rightsData,
        /* [in] */ Int32 length,
        /* [in] */ const String& mimeTypeStr,
        /* [out] */ IDrmRights** rights);

    CARAPI QueryRights(
        /* [in] */ IDrmRawContent* content,
        /* [out] */ IDrmRights** rights);

    CARAPI GetRightsList(
        /* [out] */ IObjectContainer** rightsList);

    CARAPI DeleteRights(
        /* [in] */ IDrmRights* rights);

private:
    /**
     * native method: install rights object to local client.
     *
     * @param data      input DRM rights object data to be installed.
     * @param len       the length of the data.
     * @param mimeType  the mime type of this DRM rights object. the value of this field includes:
     *                      #DRM_MIMETYPE_RIGHTS_XML
     *                      #DRM_MIMETYPE_RIGHTS_WBXML
     * @parma rights    the instance of DRMRights to be filled.
     *
     * @return #JNI_DRM_SUCCESS if succeed.
     *         #JNI_DRM_FAILURE if fail.
     */
    CARAPI_(Int32) NativeInstallDrmRights(
        /* [in] */ IInputStream* data,
        /* [in] */ Int32 len,
        /* [in] */ Int32 mimeType,
        /* [in] */ CDrmRights* rights);

    /**
     * native method: query the given DRM content's rights object.
     *
     * @param content   the given DRM content.
     * @param rights    the instance of rights to set if have.
     *
     * @return #JNI_DRM_SUCCESS if succeed.
     *         #JNI_DRM_FAILURE if fail.
     */
    CARAPI_(Int32) NativeQueryRights(
        /* [in] */ IDrmRawContent* content,
        /* [in] */ IDrmRights* rights);

    /**
     * native method: get how many rights object in current DRM agent.
     *
     * @return the number of the rights object.
     *         #JNI_DRM_FAILURE if fail.
     */
    CARAPI_(Int32) NativeGetNumOfRights();

    /**
     * native method: get all the rights object in current local agent.
     *
     * @param rights    the array instance of rights object.
     * @param numRights how many rights can be saved.
     *
     * @return the number of the rights object has been gotten.
     *         #JNI_DRM_FAILURE if fail.
     */
    CARAPI_(Int32) NativeGetRightsList(
        /* [in] */ ArrayOf<IDrmRights*>* rightsArray,
        /* [in] */ Int32 numRights);

    /**
     * native method: delete a specified rights object.
     *
     * @param rights    the specified rights object to be deleted.
     *
     * @return #JNI_DRM_SUCCESS if succeed.
     *         #JNI_DRM_FAILURE if fail.
     */
    CARAPI_(Int32) NativeDeleteRights(
        /* [in] */ IDrmRights* rights);

public:
    /**
     * The id of "application/vnd.oma.drm.rights+xml" mime type.
     */
    static const Int32 DRM_MIMETYPE_RIGHTS_XML = 3;

    /**
     * The id of "application/vnd.oma.drm.rights+wbxml" mime type.
     */
    static const Int32 DRM_MIMETYPE_RIGHTS_WBXML = 4;

    /**
     * The id of "application/vnd.oma.drm.message" mime type.
     */
    static const Int32 DRM_MIMETYPE_MESSAGE = 1;

    /**
     * Successful operation.
     */
    static const Int32 JNI_DRM_SUCCESS = 0;

    /**
     * General failure.
     */
    static const Int32 JNI_DRM_FAILURE = -1;
};

#endif // __CDRMRIGHTSMANAGER_H__
