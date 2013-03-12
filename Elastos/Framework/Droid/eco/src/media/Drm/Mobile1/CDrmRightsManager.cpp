
#include "CDrmRightsManager.h"
#include "CDrmRights.h"
#include "CDrmRawContent.h"
#include "cmdef.h"
#include <objmng/svc_drm.h>
#include <elastos/List.h>

//use namespace
namespace JNI_CDrmRightsManager {
    
/**
 * Define the mime type of DRM data.
 */
#define TYPE_DRM_MESSAGE            0x48    /**< The mime type is "application/vnd.oma.drm.message" */
#define TYPE_DRM_RIGHTS_XML         0x4a    /**< The mime type is "application/vnd.oma.drm.rights+xml" */
#define TYPE_DRM_RIGHTS_WBXML       0x4b    /**< The mime type is "application/vnd.oma.drm.rights+wbxml" */

/**
 * DRM data structure
 */
typedef struct _DrmData {
    /**
     * The id of the DRM content.
     */
    Int32 id;

    /**
     * The pointer of DRM raw content InputStream object.
     */ 
    AutoPtr<IInputStream> pInData;

    /**
     * The len of the InputStream object.
     */
    Int32 len;

    /**
     * The next DRM data.
     */
    struct _DrmData *next;
} DrmData;

/**
 * Allocate a new item of DrmData.
 *
 * \return a pointer to a DrmData item if allocate successfully,
 *         otherwise return NULL
 */

static DrmData * NewItem(void)
{
    DrmData *d = (DrmData *)malloc(sizeof(DrmData));

    if (d != NULL) {
        d->id = -1;
        d->next = NULL;
    }

    return d;
}

/**
 * Free the memory of the specified DrmData item <code>d</code>.
 *
 * \param d - a pointer to DrmData
 */
static void FreeItem(
    /* [in] */ DrmData *d)
{
    assert(d != NULL);

    free(d);
}


static Int32 GetInputStreamDataLength(
    /* [in] */ Int32 handle)
{
    AutoPtr<IInputStream> inputStream;
    Int32 len;
    DrmData* p;

    p = (DrmData *)handle;

    if (NULL == p){
        return 0;
    }

    len = p->len;
    inputStream = p->pInData;

    if (p->len <= 0 || inputStream == NULL){
        return 0;
    }

    Int32 number;
    inputStream->Available(&number);
    if (0 > number)
        return 0;

    return len;
}

Int32 ReadInputStreamData(
    /* [in] */ Int32 handle,
    /* [in] */ uint8_t* buf,
    /* [in] */ Int32 bufLen)
{
    AutoPtr<IInputStream> inputStream;
    Int32 len;
    DrmData* p;

    p = (DrmData *)handle;

    if (NULL == p || NULL == buf || bufLen <- 0)
        return 0;

    inputStream = p->pInData;
    len = p->len;

    if (p->len <= 0 || inputStream == NULL)
        return 0;

    ArrayOf<Byte>* tmp = ArrayOf<Byte>::Alloc(bufLen);
    inputStream->ReadBufferEx(0, bufLen, tmp, &len);
    memcpy(buf, tmp->GetPayload(), len);
    ArrayOf<Byte>::Free(tmp);

    if (-1 == len)
        return -1;

    return len;
}

static Int32 SetRightsFields(
    IDrmRights* rights,
    T_DRM_Rights_Info* pRoInfo)
{
    if (NULL == rights){
        return CDrmRightsManager::JNI_DRM_FAILURE;
    }

    /* set roId field */
    //field = GetFieldID(env, clazz, "roId", "Ljava/lang/String;");
    //str = NewStringUTF(env, (char *)pRoInfo->roId);
    //SetObjectField(rights, field, str);


    return CDrmRightsManager::JNI_DRM_SUCCESS;
}

static Int32 NativeInstallDrmRights(
    /* [in] */ IInputStream* data,
    /* [in] */ Int32 len,
    /* [in] */ Int32 mimeType,
    /* [in] */ IDrmRights* rights)
{
    T_DRM_Input_Data inData;
    DrmData* drmInData;
    T_DRM_Rights_Info rightsInfo;

    switch (mimeType) {
    case CDrmRightsManager::DRM_MIMETYPE_RIGHTS_XML:
        mimeType = TYPE_DRM_RIGHTS_XML;
        break;
    case CDrmRightsManager::DRM_MIMETYPE_RIGHTS_WBXML:
        mimeType = TYPE_DRM_RIGHTS_WBXML;
        break;
    case CDrmRightsManager::DRM_MIMETYPE_MESSAGE:
        mimeType = TYPE_DRM_MESSAGE;
        break;
    default:
        return CDrmRightsManager::JNI_DRM_FAILURE;
    }

    drmInData = NewItem();
    if (NULL == drmInData){
        return CDrmRightsManager::JNI_DRM_FAILURE;
    }

    drmInData->pInData = data;
    //data->Release();
    drmInData->len = len;

    inData.inputHandle = (int32_t)drmInData;
    inData.mimeType = mimeType;
    inData.getInputDataLength = GetInputStreamDataLength;
    inData.readInputData = ReadInputStreamData;

    memset(&rightsInfo, 0, sizeof(T_DRM_Rights_Info));
    if (DRM_FAILURE == SVC_drm_installRights(inData, &rightsInfo))
        return CDrmRightsManager::JNI_DRM_FAILURE;

    FreeItem(drmInData);

    return SetRightsFields(rights, &rightsInfo);
}

static Int32 NativeQueryRights(
    /* [in] */ IDrmRawContent* rawContent,
    /* [in] */ IDrmRights* rights)
{
    Int32 mId;
    T_DRM_Rights_Info rightsInfo;

    memset(&rightsInfo, 0, sizeof(T_DRM_Rights_Info));
    if (DRM_SUCCESS != SVC_drm_getRightsInfo(mId, &rightsInfo))
        return CDrmRightsManager::JNI_DRM_FAILURE;

    return SetRightsFields(rights, &rightsInfo);
}

static Int32 NativeGetNumOfRights()
{
    T_DRM_Rights_Info_Node *pRightsList;
    T_DRM_Rights_Info_Node *pCurNode;
    int32_t num = 0;

    if (DRM_FAILURE == SVC_drm_viewAllRights(&pRightsList))
        return CDrmRightsManager::JNI_DRM_FAILURE;

    pCurNode = pRightsList;
    while (pCurNode != NULL) {
        num++;
        pCurNode = pCurNode->next;
    }

    SVC_drm_freeRightsInfoList(pRightsList);

    return num;
}

static Int32 NativeGetRightsList(
    /* [in] */ ArrayOf<IDrmRights*>* rightsArray,
    /* [in] */ Int32 numRights)
{
    T_DRM_Rights_Info_Node *pRightsList;
    T_DRM_Rights_Info_Node *pCurNode;
    int32_t index;

    if (DRM_FAILURE == SVC_drm_viewAllRights(&pRightsList))
        return CDrmRightsManager::JNI_DRM_FAILURE;

    pCurNode = pRightsList;
    for (index = 0; NULL != pCurNode; index++) {
        //IDrmRights* mRights = rightsArray[index];//
        IDrmRights* mRights;
        mRights = (*rightsArray)[index];
        if (NULL == mRights)
            break;

        if (CDrmRightsManager::JNI_DRM_FAILURE == SetRightsFields(mRights, &(pCurNode->roInfo)))
            break;

        //rightsArray[index] = mRights;

        pCurNode = pCurNode->next;
    }

    SVC_drm_freeRightsInfoList(pRightsList);

    return index;
}

static Int32 NativeDeleteRights(
    /* [in] */ IDrmRights* rights)
{
    uint8_t *nativeStr;

    if (0 == strcmp("ForwardLock", (char *)nativeStr))
        return CDrmRightsManager::JNI_DRM_SUCCESS;

    if (DRM_SUCCESS != SVC_drm_deleteRights(nativeStr)) {
        //ReleaseStringUTFChars(str, (char *)nativeStr);
        return CDrmRightsManager::JNI_DRM_FAILURE;
    }

    //ReleaseStringUTFChars(str, (char *)nativeStr);
    return CDrmRightsManager::JNI_DRM_SUCCESS;
}

};//end namespace

const Int32 CDrmRightsManager::DRM_MIMETYPE_RIGHTS_XML;
const Int32 CDrmRightsManager::DRM_MIMETYPE_RIGHTS_WBXML;
const Int32 CDrmRightsManager::DRM_MIMETYPE_MESSAGE;
const Int32 CDrmRightsManager::JNI_DRM_SUCCESS;
const Int32 CDrmRightsManager::JNI_DRM_FAILURE;

using namespace JNI_CDrmRightsManager;

ECode CDrmRightsManager::GetInstance(
    /*[out] */ IDrmRightsManager** singleton)
{
    if (singleton == NULL) {
        CDrmRightsManager::New(&mSingleton);
        }

    *singleton = mSingleton;
    return NOERROR;
}

ECode CDrmRightsManager::InstallRights(
    /* [in] */ IInputStream* rightsData,
    /* [in] */ Int32 len,
    /* [in] */ const String & mimeTypeStr,
    /* [out] */ IDrmRights** rights)
{
    Int32 mMimeType = 0;

    if (mimeTypeStr.Equals(DRM_MIMETYPE_RIGHTS_XML_STRING)){
        mMimeType = DRM_MIMETYPE_RIGHTS_XML;
    }
    else if (mimeTypeStr.Equals(DRM_MIMETYPE_RIGHTS_WBXML_STRING)){
        mMimeType = DRM_MIMETYPE_RIGHTS_WBXML;
    }
    else if (mimeTypeStr.Equals(DRM_MIMETYPE_MESSAGE_STRING)){
        mMimeType = DRM_MIMETYPE_MESSAGE;
    }
    else{
        //throw new IllegalArgumentException("mimeType must be DRM_MIMETYPE_RIGHTS_XML or DRM_MIMETYPE_RIGHTS_WBXML or DRM_MIMETYPE_MESSAGE");
        return E_INVALID_ARGUMENT;
    }

    if (len <= 0){
        *rights = NULL;
    }

    //IDrmRights* mRights = new DrmRights();//
    AutoPtr<IDrmRights> mRights;
    CDrmRights::New((IDrmRights**)&mRights);

    /* call native method to install this rights object. */
    Int32 res = JNI_CDrmRightsManager::NativeInstallDrmRights(rightsData, len, mMimeType, mRights);

    if (JNI_DRM_FAILURE == res){
        //throw new DrmException("nativeInstallDrmRights() returned JNI_DRM_FAILURE");
    }

    *rights = mRights;
    return NOERROR;
}

ECode CDrmRightsManager::QueryRights(
    /* [in] */ IDrmRawContent * content,
    /* [out] */ IDrmRights ** rights)
{
    //IDrmRights* mRights = new DrmRights();//
    AutoPtr<IDrmRights> mRights;
    CDrmRights::New((IDrmRights**)&mRights);

    /* call native method to query the rights */
    Int32 res = JNI_CDrmRightsManager::NativeQueryRights(content, mRights);

    if (JNI_DRM_FAILURE == res){
        *rights = NULL;
    }

    *rights = mRights;
    return NOERROR;
}

ECode CDrmRightsManager::GetRightsList(
    /* [out,callee] */ ArrayOf<IDrmRights*>** rightsList)
{
    VALIDATE_NOT_NULL(rightsList);//check the rightsList assert NULL
    //List mRightsList = new ArrayList();

    /* call native method to get how many rights object in current agent */
    Int32 mNum = JNI_CDrmRightsManager::NativeGetNumOfRights();

    if (JNI_DRM_FAILURE == mNum)
        return NULL;

    if (mNum > 0) {
        //DrmRights[] rightsArray = new DrmRights[num];//
        ArrayOf<IDrmRights*>* rightsArray = ArrayOf<IDrmRights*>::Alloc(mNum);
        Int32 i;

        for (i = 0; i < mNum; i++)
            //rightsArray[i] = new DrmRights();//
            CDrmRights::New((IDrmRights**)&rightsArray[i]);

        /* call native method to get all the rights information */
        mNum = JNI_CDrmRightsManager::NativeGetRightsList(rightsArray, mNum);

        if (JNI_DRM_FAILURE == mNum)
            *rightsList = NULL;

        *rightsList = rightsArray;    
        /* add all rights informations to ArrayList */
        //for (i = 0; i < mNum; i++)
        //    mRightsList.add(rightsArray[i]);
    }
    else {
        *rightsList = NULL;    
    }
    return NOERROR;
}

ECode CDrmRightsManager::DeleteRights(
    /* [in] */ IDrmRights* rights)
{
    /* call native method to delete the specified rights object */
    Int32 mRes = JNI_CDrmRightsManager::NativeDeleteRights(rights);

    if (JNI_DRM_FAILURE == mRes){
        rights = NULL;
    }

    return NOERROR;
}

ECode CDrmRightsManager::constructor()
{
    return NOERROR;
}