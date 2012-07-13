
#include <stdio.h>
#include <stdlib.h>
#include <sys/atomics.h>
#include "prxstub.h"
#include "rot.h"

ECode LookupModuleInfo(
    /* [in] */ REMuid rclsid,
    /* [out] */ CIModuleInfo **ppModuleInfo);

ECode LookupClassInfo(
    /* [in] */ REMuid rclsid,
    /* [out] */ CIClassInfo **ppClassInfo);

ECode AcquireClassInfo(
    /* [in] */ const ClassID & classId,
    /* [out] */ CIClassInfo **ppClassInfo);

void FlatModuleInfo(
    /* [in] */ CIModuleInfo *pSrcModInfo,
    /* [out] */ CIModuleInfo *pDestModInfo);

void *GetUnalignedPtr(void *pPtr);

ECode CInterfaceStub::UnmarshalIn(
    /* [in] */ const CIMethodInfo *pMethodInfo,
    /* [in] */ CRemoteParcel *pParcel,
    /* [in, out] */ MarshalHeader *pOutHeader,
    /* [in, out] */ UInt32 *puArgs)
{
    return Stub_ProcessUnmsh_In(
            pMethodInfo,
            (IParcel*)pParcel,
            (UInt32 *)pOutHeader,
            puArgs);
}

ECode CInterfaceStub::MarshalOut(
    /* [in] */ UInt32 *puArgs,
    /* [in] */ MarshalHeader *pInHeader,
    /* [out] */ MarshalHeader *pOutHeader,
    /* [in] */ Boolean bOnlyReleaseIn,
    /* [in, out] */ CRemoteParcel* pParcel)
{
    MarshalHeader *pHeader;
    ECode ec;

    ec = Stub_ProcessMsh_Out(
            &(m_pInfo->methods[pInHeader->m_hMethodIndex - 4]),
            puArgs,
            (UInt32 *)pOutHeader,
            bOnlyReleaseIn,
            (IParcel*)pParcel);

    if (pParcel && SUCCEEDED(ec)) {
        // TODO:
//        pHeader = pParcel->GetMarshalHeader();
//        assert(pHeader != NULL);
//        pHeader->m_uMagic = pInHeader->m_uMagic;
//        pHeader->m_hInterfaceIndex = pInHeader->m_hInterfaceIndex ;
//        pHeader->m_hMethodIndex = pInHeader->m_hMethodIndex;
    }

    return ec;
}

CObjectStub::CObjectStub() :
    m_pInterfaces(NULL),
    m_bRequestToQuit(FALSE),
    m_cRef(1)
{
    incStrong(this);
}

CObjectStub::~CObjectStub()
{
    if (m_pInterfaces) {
        delete [] m_pInterfaces;
    }

    UnregisterExportObject((android::IBinder*)this);
}

PInterface CObjectStub::Probe(
    /* [in] */ REIID riid)
{
    Int32 n;

    if (riid == EIID_IInterface) {
        return (IInterface*)(IStub*)this;
    }

    for (n = 0; n < m_cInterfaces; n++) {
        if (riid == m_pInterfaces[n].m_pInfo->iid) {
            break;
        }
    }
    if (n == m_cInterfaces) {
        MARSHAL_DBGOUT(MSHDBG_WARNING, printf(
                "Stub: Probe failed, iid: "));
        MARSHAL_DBGOUT(MSHDBG_WARNING, DUMP_GUID(riid));

        return NULL;
    }

    return m_pInterfaces[n].m_pObject;
}

UInt32 CObjectStub::AddRef(void)
{
    Int32 lRefs = atomic_inc(&m_cRef);

    MARSHAL_DBGOUT(MSHDBG_CREF, printf(
            "Stub AddRef: %d\n", lRefs));
    return (UInt32)lRefs;
}

UInt32 CObjectStub::Release(void)
{
    Int32 lRefs = atomic_dec(&m_cRef);

    if (lRefs == 0) {
        MARSHAL_DBGOUT(MSHDBG_NORMAL, printf(
                "Stub destructed.\n"));
        delete this;

        return 0;
    }

    MARSHAL_DBGOUT(MSHDBG_CREF, printf(
            "Stub Release: %d\n", lRefs));
    return (UInt32)lRefs;
}

ECode CObjectStub::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CObjectStub::Invoke(
    /* [in] */ const android::Parcel& data,
    /* [out] */ android::Parcel* reply)
{
    MarshalHeader *pInHeader = NULL, *pOutHeader = NULL;
    UInt32 *puArgs, uMethodAddr, uMethodIndex;
    ECode ec, orgec = NOERROR;
    CInterfaceStub *pCurInterface;
    const CIMethodInfo *pMethodInfo;
    CRemoteParcel *pParcel = NULL;
    CRemoteParcel* pReplyParcel = NULL;
    UInt32 uInSize;
#ifdef _x86
    UInt32 uEAX, uEDX, uECX, uESP;
#elif defined(_arm)
    struct ArgumentBuffer
    {
        UInt32    r0;
        UInt32    r1;
        UInt32    r2;
        UInt32    r3;
    } argbuf;
#if defined(_GNUC)
    UInt32 r0, r1, r2, r3, ip, lr, sp;
#else
#error Unknown C++ compiler
#endif // defined(_GNUC)
#elif defined(_mips)
    UInt32 uV0, uV1;
    UInt32 uA0, uA1, uA2, uA3;
    UInt32 uT0, uT1, uT2, uT3, uT4, uT5, uT6, uT7, uT8, uT9;
    UInt32 uSp;
#else
#error unknown architecture
#endif
    Boolean bOnlyReleaseIn = FALSE;

    MARSHAL_DBGOUT(MSHDBG_NORMAL,
            printf("Stub: in Invoke.\n"));

    pInHeader = (MarshalHeader*)data.readInplace(sizeof(*pInHeader));
    if (pInHeader == NULL) {
        MARSHAL_DBGOUT(MSHDBG_ERROR,
                printf("Stub error: read MarshalHeader failed.\n"));
        goto ErrorExit;
    }
    if (pInHeader->m_uMagic != MARSHAL_MAGIC) {
        MARSHAL_DBGOUT(MSHDBG_ERROR,
                printf("Stub: invalid magic - %x\n", pInHeader->m_uMagic));
        goto ErrorExit;
    }

#if defined(_DEBUG)
    if (pInHeader->m_uInSize < sizeof(MarshalHeader)) {
        goto ErrorExit;
    }
    if (pInHeader->m_hInterfaceIndex >= m_cInterfaces) {
        MARSHAL_DBGOUT(MSHDBG_ERROR,
                printf("Stub: interface index error - %d:%d\n",
                pInHeader->m_hInterfaceIndex, m_cInterfaces));
        goto ErrorExit;
    }
    MARSHAL_DBGOUT(MSHDBG_NORMAL,
            printf("Stub: interface idx(%d), method idx(%d)\n",
            pInHeader->m_hInterfaceIndex, pInHeader->m_hMethodIndex));
#endif

    if (pInHeader->m_hMethodIndex >= 4) {
        pCurInterface = &(m_pInterfaces[pInHeader->m_hInterfaceIndex]);

        uMethodIndex = pInHeader->m_hMethodIndex - 4;
        if (uMethodIndex >= pCurInterface->m_pInfo->methodNumMinus4) {
            MARSHAL_DBGOUT(MSHDBG_ERROR,
                    printf("Stub: method index out of range - %d:%d\n",
                    uMethodIndex,
                    pCurInterface->m_pInfo->methodNumMinus4));
            goto ErrorExit;
        }

        uMethodAddr =
                pCurInterface->m_pInterface->m_vPtr->m_vTable[uMethodIndex + 4];

        if (0 != pInHeader->m_uOutSize) {
            pOutHeader = (MarshalHeader *)alloca(pInHeader->m_uOutSize);
            if (!pOutHeader) {
                MARSHAL_DBGOUT(MSHDBG_ERROR,
                        printf("Stub error: alloca() failed.\n"));
                ec = E_OUT_OF_MEMORY;
                goto ErrorExit;
            }
        }

        pMethodInfo = &((pCurInterface->m_pInfo)->methods[uMethodIndex]);
        uInSize = GET_LENGTH(pMethodInfo->reserved1) * 4 + 4;
        MARSHAL_DBGOUT(MSHDBG_NORMAL,
                printf("Stub: method args stack size (%d)\n", uInSize));
        puArgs = (UInt32 *)alloca(uInSize);
        if (!puArgs) {
            MARSHAL_DBGOUT(MSHDBG_ERROR,
                    printf("Stub error: alloca() failed.\n"));
            ec = E_OUT_OF_MEMORY;
            goto ErrorExit;
        }
#ifdef _x86
        GET_REG(ESP, puArgs);
#endif

        pParcel = new CRemoteParcel(const_cast<android::Parcel*>(&data));
        if (!pParcel) {
            MARSHAL_DBGOUT(MSHDBG_ERROR,
                    printf("Stub error: new CRemoteParcel failed.\n"));
            ec = E_OUT_OF_MEMORY;
            goto ErrorExit;
        }
        ec = pCurInterface->UnmarshalIn(pMethodInfo,
                                        pParcel,
                                        pOutHeader,
                                        puArgs + 1);
        if (FAILED(ec)) {
            MARSHAL_DBGOUT(MSHDBG_ERROR,
                    printf("Stub error: UnmarshalIn() failed.\n"));
            goto ErrorExit;
        }

        *puArgs = (UInt32)pCurInterface->m_pObject; // fill this

        MARSHAL_DBGOUT(MSHDBG_NORMAL,
                printf("Stub: invoke method - args(%x), addr(%x) \n",
                puArgs, uMethodAddr));

#ifdef _x86
        GET_REG(EAX, uEAX);
        GET_REG(EDX, uEDX);
        GET_REG(ECX, uECX);
        GET_REG(ESP, uESP);
        assert(uESP == (UInt32)puArgs);
        // NOTE: we should promise STUB_INVOKE_METHOD just occures
        // in use space , if it used in kernel mode , the return
        // args in stack have the possibilty for being modified
        //
        STUB_INVOKE_METHOD(ec, puArgs, uMethodAddr);
        SET_REG(ECX, uECX);
        SET_REG(EDX, uEDX);
        SET_REG(EAX, uEAX);
#elif defined(_arm)

#if defined(_GNUC)
        if (sizeof(UInt32) * 4 >= uInSize) {
            GET_REG(r0, r0);
            GET_REG(r1, r1);
            GET_REG(r2, r2);
            GET_REG(r3, r3);
            GET_REG(ip, ip);
            GET_REG(lr, lr);
            STUB_INVOKE_METHOD1(ec, puArgs, uMethodAddr);
            SET_REG(lr, lr);
            SET_REG(ip, ip);
            SET_REG(r3, r3);
            SET_REG(r2, r2);
            SET_REG(r1, r1);
            SET_REG(r0, r0);
        }
        else {
            argbuf = *(ArgumentBuffer *)puArgs;
            GET_REG(r0, r0);
            GET_REG(r1, r1);
            GET_REG(r2, r2);
            GET_REG(r3, r3);
            GET_REG(ip, ip);
            GET_REG(lr, lr);
            GET_REG(sp, sp);
            STUB_INVOKE_METHOD2(ec, puArgs, uMethodAddr);
            SET_REG(sp, sp);
            SET_REG(lr, lr);
            SET_REG(ip, ip);
            SET_REG(r3, r3);
            SET_REG(r2, r2);
            SET_REG(r1, r1);
            SET_REG(r0, r0);
            *(ArgumentBuffer *)puArgs = argbuf;
        }
#else

#error Unknown C++ compiler

#endif // defined(_GNUC)

#elif defined(_mips)
        GET_REG($2, uV0);
        GET_REG($3, uV1);
        GET_REG($4, uA0);
        GET_REG($5, uA1);
        GET_REG($6, uA2);
        GET_REG($7, uA3);
        GET_REG($8, uT0);
        GET_REG($9, uT1);
        GET_REG($10, uT2);
        GET_REG($11, uT3);
        GET_REG($12, uT4);
        GET_REG($13, uT5);
        GET_REG($14, uT6);
        GET_REG($15, uT7);
        GET_REG($24, uT8);
        GET_REG($25, uT9);
        GET_REG($29, uSp);
        STUB_INVOKE_METHOD(ec, puArgs, uMethodAddr);
        SET_REG($29, uSp);
        SET_REG($2, uV0);
        SET_REG($3, uV1);
        SET_REG($4, uA0);
        SET_REG($5, uA1);
        SET_REG($6, uA2);
        SET_REG($7, uA3);
        SET_REG($8, uT0);
        SET_REG($9, uT1);
        SET_REG($10, uT2);
        SET_REG($11, uT3);
        SET_REG($12, uT4);
        SET_REG($13, uT5);
        SET_REG($14, uT6);
        SET_REG($15, uT7);
        SET_REG($24, uT8);
        SET_REG($25, uT9);
#else
#error unknown architecture
#endif

        if (FAILED(ec) && GET_IN_INTERFACE_MARK(pMethodInfo->reserved1)) {
            bOnlyReleaseIn = TRUE;
        }

        reply->writeInt32((int32_t)ec);
        // puArgs + 1 , skip this pointer
        if ((pOutHeader && SUCCEEDED(ec))
            || GET_IN_INTERFACE_MARK(pMethodInfo->reserved1)) {

            if (pOutHeader && SUCCEEDED(ec)) pReplyParcel = new CRemoteParcel(reply);
//            pReplyParcel->WriteInt32(ec);
            orgec = pCurInterface->MarshalOut(puArgs + 1,
                                              pInHeader,
                                              pOutHeader,
                                              bOnlyReleaseIn,
                                              pReplyParcel);

            if (FAILED(orgec)) {
                MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                        "Stub: marshal out args fail, hr(%x)\n", ec));
                ec = orgec;
                goto ErrorExit;
            }
        }
    }
    else {
        if (pInHeader->m_hMethodIndex == 0) {
            MARSHAL_DBGOUT(MSHDBG_ERROR,
                    printf("Stub error: Remote Probe().\n"));
        }
        else if (pInHeader->m_hMethodIndex == 1) {
            MARSHAL_DBGOUT(MSHDBG_ERROR,
                    printf("Stub error: Remote AddRef().\n"));
        }
        else if (pInHeader->m_hMethodIndex == 2) {
            MARSHAL_DBGOUT(MSHDBG_ERROR,
                    printf("Stub error: Remote Release().\n"));
        }
        else if (pInHeader->m_hMethodIndex == 3) {
            MARSHAL_DBGOUT(MSHDBG_ERROR,
                    printf("Stub error: Remote GetInterfaceID().\n"));
        }
        ec = E_INVALID_OPERATION;    // Do not allow remote Probe().
        goto ErrorExit;
    }

    MARSHAL_DBGOUT(MSHDBG_NORMAL, printf(
            "Stub: invoke server method ok, _SysReply\n"));

ErrorExit:

    if (pParcel != NULL) delete pParcel;
    if (pReplyParcel != NULL) delete pReplyParcel;

    return ec;
}

ECode CObjectStub::GetClassID(
    /* [out] */ EMuid *pClsid)
{
    assert(pClsid != NULL);

    *pClsid = ((CIClassInfo*)m_pInfo)->clsid;
    return NOERROR;
}

ECode CObjectStub::GetClassInfo(
    /* [out] */ CIClassInfo **ppClassInfo)
{
    assert(ppClassInfo != NULL);

    *ppClassInfo = m_pInfo;
    return NOERROR;
}

ECode CObjectStub::GetInterfaceIndex(
    /* [in] */ IInterface *pObj,
    /* [out] */ UInt32 *puIndex)
{
    for (UInt32 n = 0; n < (UInt32)m_cInterfaces; n++) {
        if (m_pInterfaces[n].m_pObject == pObj) {
            *puIndex = n;
            return NOERROR;
        }
    }
    MARSHAL_DBGOUT(MSHDBG_WARNING, printf(
        "Stub: InterfaceIndex failed - pObj(%x)\n", pObj));

    return E_NO_INTERFACE;
}

android::status_t CObjectStub::onTransact(
            /* [in] */ uint32_t code,
            /* [in] */ const android::Parcel& data,
            /* [out] */ android::Parcel* reply,
            /* [in] */ uint32_t flags)
{
    switch(code) {
        case INVOKE: {
                Invoke(data, reply);
                return android::NO_ERROR;
            }
        case GET_CLASS_INFO: {
                EMuid clsid;
                CIModuleInfo *pSrcModInfo;
                CIModuleInfo *pDestModInfo;

                GetClassID(&clsid);
                if (FAILED(LookupModuleInfo(clsid, &pSrcModInfo))) {
                    MARSHAL_DBGOUT(MSHDBG_ERROR,
                            printf("Lookup module info fail.\n"));
                    reply->writeInt32((int32_t)E_FAIL);
                    return android::NO_ERROR;
                }

                pDestModInfo = (CIModuleInfo *)calloc(pSrcModInfo->totalSize, 1);
                FlatModuleInfo(pSrcModInfo, pDestModInfo);

                reply->writeInt32((int32_t)NOERROR);
                reply->writeInt32(pDestModInfo->totalSize);
                reply->write((void*)pDestModInfo, pDestModInfo->totalSize);

                free(pDestModInfo);

                return android::NO_ERROR;
            }
        case RELEASE: {
                return android::NO_ERROR;
            }
        default:
            return BBinder::onTransact(code, data, reply, flags);
    }
}

ECode CObjectStub::S_CreateObject(
    /* [in] */ IInterface *pObject,
    /* [out] */ IStub **ppIStub)
{
    CObjectStub *pStub;
    CInterfaceStub *pInterfaces;
    IInterface *pObj;
    IObject *pObj1;
    ClassID clsid;
    InterfaceID iid;
    ECode ec = NOERROR;
    CIClassInfo ** ppClassInfo = NULL;
    Int32 n;

    if (!pObject || !ppIStub) {
        return E_INVALID_ARGUMENT;
    }

    *ppIStub = NULL;

    pStub = new CObjectStub();
    if (!pStub) {
        return E_OUT_OF_MEMORY;
    }

    pObj1 = (IObject*)pObject->Probe(EIID_IObject);
    if (!pObj1) {
	    MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                "Create stub: interface do not support EIID_IObject QI.\n"));
        goto ErrorExit;
    }

    ec = pObj1->GetClassID(&clsid);
    if (FAILED(ec)) {
        MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                "Create stub: fail to get object's ClassID.\n"));
        goto ErrorExit;
    }

    MARSHAL_DBGOUT(MSHDBG_NORMAL, printf(
            "QI class info ok. EMuid is:\n"));
    MARSHAL_DBGOUT(MSHDBG_NORMAL, DUMP_CLSID(clsid));

    ec = pObject->GetInterfaceID(pObject, &iid);
    if (FAILED(ec)) {
        MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                "Create stub: interface does not have InterfaceID.\n"));
        goto ErrorExit;
    }
    MARSHAL_DBGOUT(MSHDBG_NORMAL, printf(
            "QI EIID info ok. EIID is:\n"));
    MARSHAL_DBGOUT(MSHDBG_NORMAL, DUMP_GUID(iid));

    ec = LookupClassInfo(clsid.clsid, &(pStub->m_pInfo));
    if (FAILED(ec)) {
        ec = AcquireClassInfo(clsid, &(pStub->m_pInfo));
        if (FAILED(ec)) {
            MARSHAL_DBGOUT(MSHDBG_ERROR,
                    printf("Create Stub: get class info fail, the ec = 0x%08x\n", ec));
            goto ErrorExit;
        }
    }

    ppClassInfo = &(pStub->m_pInfo);

    MARSHAL_DBGOUT(MSHDBG_NORMAL, printf(
            "Create stub: Get class info ok.\n"));

    pInterfaces = new CInterfaceStub[(*ppClassInfo)->interfaceNum];
    if (!pInterfaces) {
        MARSHAL_DBGOUT(MSHDBG_ERROR, printf("Create stub: out of memory.\n"));
        ec = E_OUT_OF_MEMORY;
        goto ErrorExit;
    }
    pStub->m_cInterfaces = (*ppClassInfo)->interfaceNum;
    pStub->m_pInterfaces = pInterfaces;
    pObject->AddRef();
    for (n = 0; n < (*ppClassInfo)->interfaceNum; n++) {
        CIInterfaceInfo *pInterfaceInfo =
                (CIInterfaceInfo *)GetUnalignedPtr(
                        (*ppClassInfo)->interfaces + n);
        pObj = pObject->Probe(pInterfaceInfo->iid);
        if (!pObj) {
            MARSHAL_DBGOUT(MSHDBG_ERROR,
                    printf("Create stub: no such interface.\n"));
            ec = E_NO_INTERFACE;
            pObject->Release();
            goto ErrorExit;
        }
        pInterfaces[n].m_pInfo = pInterfaceInfo;
        pInterfaces[n].m_pObject = pObj;
    }

    ec = RegisterExportObject((android::IBinder*)pStub, pObject, pStub);
    if (FAILED(ec)) {
        pObject->Release();
        MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                "Create stub: register export object failed, ec(%x)\n", ec));
        goto ErrorExit;
    }

    *ppIStub = (IStub *)pStub;

    return NOERROR;

ErrorExit:
    delete pStub;
    return ec;
}

