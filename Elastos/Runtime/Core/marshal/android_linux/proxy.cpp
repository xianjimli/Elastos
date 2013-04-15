
#include <stdio.h>
#include <stdlib.h>
#include <sys/atomics.h>
#include <sys/mman.h>
#include "prxstub.h"
#include "rot.h"

EXTERN_C const InterfaceID EIID_IProxy;

ECode LookupClassInfo(
    /* [in] */ REMuid rclsid,
    /* [out] */ CIClassInfo **ppClassInfo);

ECode GetRemoteClassInfo(
    /* [in] */ android::IBinder* pRemoteObj,
    /* [in] */ REMuid clsId,
    /* [out] */ CIClassInfo ** ppClassInfo);

void *GetUnalignedPtr(void *pPtr);

Address s_proxyEntryAddress = NULL;

#define SYS_PROXY_RET_OFFSET    9

#ifndef PAGE_SIZE
#define PAGE_SIZE (1u << 12)
#endif
#ifndef PAGE_MASK
#define PAGE_MASK (~(PAGE_SIZE - 1))
#endif
#ifndef PAGE_ALIGN
#define PAGE_ALIGN(va) (((va)+PAGE_SIZE-1)&PAGE_MASK)
#endif

UInt32 g_marshalVtbl[MSH_MAX_METHODS];

EXTERN_C void ProxyEntryFunc(void);

#ifdef _GNUC
#    if defined(_arm)
#       define DECL_SYS_PROXY_ENTRY()
            __asm__(                               \
            ".text;"                            \
            ".align 4;"                         \
            ".globl ProxyEntryFunc;"           \
            "ProxyEntryFunc:"                  \
            "push {r0 - r3};"            \
            "push {lr};"
            "mov  r1, #0xff;"                 \
            "ldr  pc, [r0, #4];"              \
        )
        DECL_SYS_PROXY_ENTRY();
#    elif defined(_x86)
#       define DECL_SYS_PROXY_ENTRY()             \
            __asm__(                               \
                ".text;"                           \
                ".align 4;"                        \
                ".globl _ProxyEntryFunc;"          \
                "_ProxyEntryFunc:"                 \
                ".intel_syntax;"                   \
                "push   esp;"                      \
                "mov    eax, dword ptr [esp + 8];" \
                "call   dword ptr [eax + 4];"      \
                "ret    0x4;"                      \
                ".att_syntax;"                     \
            )
        DECL_SYS_PROXY_ENTRY();
#    else
#        error "Unknown archibute"
#   endif
#else
#   if defined(_x86)
#       define DECL_SYS_PROXY_ENTRY() \
            __declspec( naked ) void ProxyEntryFunc()                   \
            {                                       \
                __asm push esp                      \
                __asm mov eax, dword ptr [esp + 8]  \
                __asm call dword ptr [eax +4]       \
                __asm ret  0x4                      \
            }
        DECL_SYS_PROXY_ENTRY();
#   endif
#endif

#ifdef _arm

EXTERN_C void __ProxyEntry(void);

EXTERN_C ECode GlobalProxyEntry(UInt32 *puArgs)
{
    return CInterfaceProxy::ProxyEntry(puArgs);
}

#ifdef _GNUC
#define DECL_PROXY_ENTRY()              \
    __asm__(                            \
        ".text;"                        \
        ".align 4;"                     \
        ".globl __ProxyEntry;"         \
        "__ProxyEntry:"                \
        "push   {r1};"         \
        "add    r0, sp, #4;"            \
        "bl     GlobalProxyEntry;"     \
        "add    sp, sp, #4;"           \
        "pop    {lr};"                \
        "pop    {r1};"                \
        "pop    {r1 - r3};"            \
        "push   {lr};"                \
        "pop    {pc};"                \
    )

DECL_PROXY_ENTRY();
#endif

#endif

#ifdef _mips

EXTERN_C ECode GlobalProxyEntry(UInt32 *puArgs)
{
    return CInterfaceProxy::ProxyEntry(puArgs);
}

#ifdef _GNUC
void __ProxyContinue()
{
DECL_ASMENTRY(ProxyContinue)
#elif defined(_EVC)
DECL_NAKED void ProxyContinue()
{
#else
#error unknown compiler
#endif
    ASM(
        "sw     $4, 0x0($29);"
        "sw     $5, 0x4($29);"
        "sw     $6, 0x8($29);"
        "sw     $7, 0xc($29);"
        "addiu  $29, $29, -0x1c;"
        "sw     $12, 0x10($29);"
        "add    $4, $29, 0x18;"
        "sw     $4, 0x14($29);"
        "la     $9, GlobalProxyEntry;"
        "jalr   $9;"
        "addiu  $29, $29, 0x1c;"
        "lw     $31, -4($29);"
        "jr     $31;");
}
#endif

void InitProxyEntry()
{
    s_proxyEntryAddress =
            (Address)mmap((void*)0,
                          PAGE_ALIGN(PROXY_ENTRY_NUM * PROXY_ENTRY_SIZE),
                          PROT_READ | PROT_WRITE | PROT_EXEC,
                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (s_proxyEntryAddress == 0) {
        printf("out of memory.\n");
        return;
    }

#if defined(_x86)
    char *p = (char *)s_proxyEntryAddress;
    for (UInt32 n = 0; n < PROXY_ENTRY_NUM; n++) {
        memcpy(p, (void*)&ProxyEntryFunc, PROXY_ENTRY_SIZE);
        *(Int16*)&(p[SYS_PROXY_RET_OFFSET]) = ( (n + 1) << 2);
        p+= PROXY_ENTRY_SIZE;
    }
#elif defined(_arm)
    char * p = (char *)s_proxyEntryAddress;
    for (Int32 n = 0; n < PROXY_ENTRY_NUM; n++) {
        memcpy(p, (void *)&ProxyEntryFunc, PROXY_ENTRY_SIZE);
        p[8] = n;
        p += PROXY_ENTRY_SIZE;
    }
#else
#error "unsupported architecture yet."
#endif

    g_marshalVtbl[0] = (UInt32)&CInterfaceProxy::S_Probe;
    g_marshalVtbl[1] = (UInt32)&CInterfaceProxy::S_AddRef;
    g_marshalVtbl[2] = (UInt32)&CInterfaceProxy::S_Release;
    g_marshalVtbl[3] = (UInt32)&CInterfaceProxy::S_GetInterfaceID;
    for (int m = 4; m < MSH_MAX_METHODS; m++) {
        g_marshalVtbl[m] = PROXY_ENTRY_BASE + ((m - 4) << PROXY_ENTRY_SHIFT);
    }
}

void UninitProxyEntry()
{
     if (0 != s_proxyEntryAddress) {
        munmap((void*)s_proxyEntryAddress,
               PAGE_ALIGN(PROXY_ENTRY_NUM * PROXY_ENTRY_SIZE));
        s_proxyEntryAddress = (Address)NULL;
    }
}

PInterface CInterfaceProxy::S_Probe(
    /* [in] */ CInterfaceProxy *pThis,
    /* [in] */ REIID riid)
{
    return pThis->m_pOwner->Probe(riid);
}

UInt32 CInterfaceProxy::S_AddRef(
    /* [in] */ CInterfaceProxy *pThis)
{
    return pThis->m_pOwner->AddRef();
}

UInt32 CInterfaceProxy::S_Release(
    /* [in] */ CInterfaceProxy *pThis)
{
    return pThis->m_pOwner->Release();
}

ECode CInterfaceProxy::S_GetInterfaceID(
    /* [in] */ CInterfaceProxy *pThis,
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    //todo: not correct.
    return pThis->m_pOwner->GetInterfaceID(pObject, pIID);
}

ECode CInterfaceProxy::BufferSize(
    /* [in] */ UInt32 uMethodIndex,
    /* [in] */ UInt32 *puArgs,
    /* [out] */ UInt32 *puInSize,
    /* [out] */ UInt32 *puOutSize)
{
    ECode ec;
    const CIMethodInfo *pMethodInfo;
    pMethodInfo = &(m_pInfo->methods[uMethodIndex]);

    ec = Proxy_ProcessMsh_BufferSize(
            pMethodInfo,
            puArgs,
            puInSize,
            puOutSize);
    if (0 != *puOutSize){
        *puOutSize += sizeof(MarshalHeader);
    }

    if (SUCCEEDED(ec)) {
        *puInSize += sizeof(MarshalHeader);
    }
    return ec;
}

ECode CInterfaceProxy::MarshalIn(
    /* [in] */ UInt32 uMethodIndex,
    /* [in] */ UInt32 *puArgs,
    /* [in, out] */ CRemoteParcel *pParcel)
{
    ECode ec;
    const CIMethodInfo *pMethodInfo;
    MarshalHeader *pHeader;

    pMethodInfo = &(m_pInfo->methods[uMethodIndex]);

    ec = Proxy_ProcessMsh_In(
            pMethodInfo,
            puArgs,
            (IParcel*)pParcel);

    if (SUCCEEDED(ec)) {
        pHeader = pParcel->GetMarshalHeader();
        assert(pHeader != NULL);
        pHeader->m_uMagic = MARSHAL_MAGIC;
        pHeader->m_hInterfaceIndex = m_uIndex;
        pHeader->m_hMethodIndex = uMethodIndex + 4;
    }

    return ec;
}

ECode CInterfaceProxy::UnmarshalOut(
    /* [in] */ UInt32 uMethodIndex,
    /* [out] */ CRemoteParcel *pParcel,
    /* [in] */ UInt32 *puArgs)
{
    // TODO:
//    MarshalHeader *pHeader = pParcel->GetMarshalHeader();
//
//    if (pHeader->m_uMagic != MARSHAL_MAGIC) {
//        MARSHAL_DBGOUT(MSHDBG_ERROR,
//                printf("Proxy unmsh: invalid magic(%x)\n", pHeader->m_uMagic));
//        return E_MARSHAL_DATA_TRANSPORT_ERROR;
//    }

#if defined(_DEBUG)
    // TODO:
//    if (pHeader->m_hInterfaceIndex != (Int16)m_uIndex) {
//        MARSHAL_DBGOUT(MSHDBG_ERROR,
//                printf("Proxy unmsh: invalid iidx(%x)\n",
//                pHeader->m_hInterfaceIndex));
//        return E_MARSHAL_DATA_TRANSPORT_ERROR;
//    }
//    if (pHeader->m_hMethodIndex != (Int16)(uMethodIndex + 4)) {
//        MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
//                "Proxy unmsh: invalid method(%x)\n", pHeader->m_hMethodIndex));
//    }
#endif

    // TODO:
    return Proxy_ProcessUnmsh_Out(
            &(m_pInfo->methods[uMethodIndex]),
            (IParcel*)pParcel,
            0/*pHeader->m_uOutSize - sizeof(MarshalHeader)*/,
            puArgs);
}

UInt32 CInterfaceProxy::CountMethodArgs(
    /* [in] */ UInt32 uMethodIndex)
{
    return GET_LENGTH((m_pInfo->methods[uMethodIndex]).reserved1);
}

Boolean CInterfaceProxy::MethodHasOutArgs(
    /* [in] */ UInt32 uMethodIndex)
{
    int n, cParams;
    const CIMethodInfo *pMethodInfo;
    const CIBaseType *pParams;

    pMethodInfo = &(m_pInfo->methods[uMethodIndex]);
    cParams = pMethodInfo->paramNum;
    pParams = pMethodInfo->params;

    for (n = 0; n < cParams; n++) {
        if (BT_IS_OUT(pParams[n])) return TRUE;
    }

    return FALSE;
}

ECode CInterfaceProxy::ProxyEntry(UInt32 *puArgs)
{
    CInterfaceProxy *pThis;
    UInt32 uMethodIndex, uInSize, uOutSize;
    Int32 size = 0;
    MarshalHeader *pInHeader = NULL;
    CRemoteParcel *pInParcel = NULL, *pOutParcel = NULL;
    ECode ec, orgec;
#ifndef _mips
    UInt32 cArgs;
#endif


    puArgs++; // skip ret address

    pThis = (CInterfaceProxy *)*puArgs;
    puArgs++; // skip this

    MARSHAL_DBGOUT(MSHDBG_NORMAL,
            printf("*puArgs = %x, puArgs = %x", *puArgs, puArgs));
    MARSHAL_DBGOUT(MSHDBG_NORMAL, printf("iid: "));
    MARSHAL_DBGOUT(MSHDBG_NORMAL, DUMP_GUID(pThis->m_pInfo->iid));

#ifdef _x86
    uMethodIndex = CalcMethodIndex(*(UInt32 *)((UInt32)&puArgs - 4));
#elif defined(_arm)
    uMethodIndex = puArgs[-3];
#elif defined(_mips)
    uMethodIndex = CalcMethodIndex(*(puArgs - 4) - 4);
#else
#error unknown architecture
#endif

#ifndef _mips
    // Get the stack length, not contain "this"
    cArgs = pThis->CountMethodArgs(uMethodIndex);

    MARSHAL_DBGOUT(MSHDBG_NORMAL, printf(
            "Method index(%d), args size(%d)\n", uMethodIndex + 4, cArgs * 4));
#endif

    // Calculate the package size
    //
    // NOTE:
    //  1. Alloc pOutHeader on the stack with MAX-out-size
    //  2. Assign pInHeader->m_uOutSize with MAX-out-size
    //  3. Pass the MIN-out-size to SysInvoke's last parameter
    //  4. Call Thread::ReallocBuffer in SysReply if necessary to pass back the
    //      marshaled-out data with error info
    //
    ec = pThis->BufferSize(uMethodIndex, puArgs, &uInSize, &uOutSize);
    if (FAILED(ec)) {
        MARSHAL_DBGOUT(MSHDBG_ERROR,
                printf("Proxy BufferSize() failed, ec = %x\n", ec));
        goto ProxyExit;
    }

    MARSHAL_DBGOUT(MSHDBG_NORMAL, printf(
            "Buffer size: isize(%d), osize(%d)\n", uInSize, uOutSize));
    assert(uInSize >= sizeof(MarshalHeader));

    pInParcel = new CRemoteParcel();
    ec = pThis->MarshalIn(uMethodIndex, puArgs, pInParcel);

    if (SUCCEEDED(ec)) {
        android::Parcel *pData, reply;

        pInParcel->GetElementSize(&size);
        pInParcel->GetElementPayload((Handle32*)&pData);
        pInHeader = pInParcel->GetMarshalHeader();
        pInHeader->m_uInSize = size;
        pInHeader->m_uOutSize = uOutSize;
        MARSHAL_DBGOUT(MSHDBG_NORMAL, printf(
                "Before RemoteInvoke: ParcelSize(%d)\n", size));
        android::status_t st;
        if ((st = pThis->m_pOwner->m_pBinder->transact(IStub::INVOKE, *pData, &reply))
                != android::NO_ERROR) {
            MARSHAL_DBGOUT(MSHDBG_ERROR,
                    printf("Remote invoke failed. <status: 0x%x>\n", st));
            ec = E_FAIL;
            goto ProxyExit;
        }

        // TODO:
//        pOutParcel = new CRemoteParcel(&reply);
//        pOutParcel->ReadInt32(&ec);
        ec = (ECode)reply.readInt32();
        if (SUCCEEDED(ec)) {
            if (pThis->MethodHasOutArgs(uMethodIndex)) {
                if (uOutSize) {
                    pOutParcel = new CRemoteParcel(&reply);
                    orgec = pThis->UnmarshalOut(uMethodIndex,
                                                pOutParcel,
                                                puArgs);
                    if (FAILED(orgec)) {
                        ec = orgec;
                    }
                }
            }
        }
        else {
            MARSHAL_DBGOUT(MSHDBG_WARNING,
                    printf("proxy RemoteInvoke() exit. ec = %x\n", ec));
        }
    }
    else {
        MARSHAL_DBGOUT(MSHDBG_ERROR,
                printf("proxy MarshalIn() failed. ec = %x\n", ec));
    }

ProxyExit:
    MARSHAL_DBGOUT(MSHDBG_NORMAL, printf("Exit proxy: ec(%x)\n", ec));

    if (pInParcel != NULL) delete pInParcel;
    if (pOutParcel != NULL) delete pOutParcel;

#ifndef _mips
    SYS_PROXY_EXIT(ec, &puArgs - 1, cArgs);
#else
    SYS_PROXY_EXIT(ec, &puArgs - 1, 0);
#endif
}

CObjectProxy::CObjectProxy() :
    m_pInterfaces(NULL),
    m_pICallbackConnector(NULL),
    m_cRef(1)
{}

CObjectProxy::~CObjectProxy()
{
    if (m_pInterfaces) {
        for (Int32 n = 0; n < m_cInterfaces; n++) {
            if (m_pInterfaces[n].m_pvVptr) {
                delete [] (UInt32 *)(m_pInterfaces[n].m_pvVptr);
            }
        }
        delete [] m_pInterfaces;
    }
}

static const EMuid ECLSID_XOR_CallbackSink = \
/* e724df56-e16a-4599-8edd-a97ab245d583 */
{0xe724df56,0xe16a,0x4599,{0x8e,0xdd,0xa9,0x7a,0xb2,0x45,0xd5,0x83}};

PInterface CObjectProxy::Probe(REIID riid)
{
    int n;

    if (riid == EIID_IInterface) {
        return (IInterface*)(IProxy*)this;
    }
    else if (riid == EIID_IProxy) {
        return (IProxy*)this;
    }

    if (riid == EIID_CALLBACK_CONNECTOR) {
        if (NULL == m_pICallbackConnector) {
            ClassID ezclsid;
            IInterface *pTemp = (IInterface *)&(m_pInterfaces[0].m_pvVptr);
            this->GetClassID(&ezclsid.clsid);

            ezclsid.clsid.Data1    ^= ECLSID_XOR_CallbackSink.Data1;
            ezclsid.clsid.Data2    ^= ECLSID_XOR_CallbackSink.Data2;
            ezclsid.clsid.Data3    ^= ECLSID_XOR_CallbackSink.Data3;
            ezclsid.clsid.Data4[0] ^= ECLSID_XOR_CallbackSink.Data4[0];
            ezclsid.clsid.Data4[1] ^= ECLSID_XOR_CallbackSink.Data4[1];
            ezclsid.clsid.Data4[2] ^= ECLSID_XOR_CallbackSink.Data4[2];
            ezclsid.clsid.Data4[3] ^= ECLSID_XOR_CallbackSink.Data4[3];
            ezclsid.clsid.Data4[4] ^= ECLSID_XOR_CallbackSink.Data4[4];
            ezclsid.clsid.Data4[5] ^= ECLSID_XOR_CallbackSink.Data4[5];
            ezclsid.clsid.Data4[6] ^= ECLSID_XOR_CallbackSink.Data4[6];
            ezclsid.clsid.Data4[7] ^= ECLSID_XOR_CallbackSink.Data4[7];
            ezclsid.pUunm = (char*)alloca(sizeof(char) \
                    * (strlen(((CIClassInfo*)m_pInfo)->pszUunm) + 1));
            strcpy(ezclsid.pUunm, ((CIClassInfo*)m_pInfo)->pszUunm);
            _CObject_AcquireClassFactory(ezclsid, RGM_SAME_DOMAIN, EIID_CALLBACK_CONNECTOR, &pTemp);
            m_pICallbackConnector = (ICallbackConnector*)pTemp;
        }
        return (IInterface*)m_pICallbackConnector;
    }

    for (n = 0; n < m_cInterfaces; n++) {
        if (riid == m_pInterfaces[n].m_pInfo->iid) {
            break;
        }
    }
    if (n == m_cInterfaces) {
        MARSHAL_DBGOUT(MSHDBG_WARNING, printf(
                "Proxy: QI failed, iid: "));
        MARSHAL_DBGOUT(MSHDBG_WARNING, DUMP_GUID(riid));

        return NULL;
    }

    return (IInterface *)&(m_pInterfaces[n].m_pvVptr);
}

UInt32 CObjectProxy::AddRef(void)
{
    Int32 lRefs = atomic_inc(&m_cRef);

    MARSHAL_DBGOUT(MSHDBG_CREF, printf(
            "Proxy AddRef: %d\n", lRefs));
    return (UInt32)lRefs;
}

UInt32 CObjectProxy::Release(void)
{
    ECode ec;

    Int32 lRefs = atomic_dec(&m_cRef);

    if (lRefs == 0) {
        MARSHAL_DBGOUT(MSHDBG_NORMAL, printf(
                "Proxy destructed.\n"));

        ec = UnregisterImportObject(m_pBinder.get());
        if (ec == S_FALSE) {
            return 1;// other thread hold the proxy
        }

        if (m_pICallbackConnector) {
            m_pICallbackConnector->DisconnectCallbackSink();
            delete m_pICallbackConnector;
        }

        //bugbug: maybe we don't need call release of stub,
        //        we should give this task to the binder.
//        android::Parcel data, reply;
//        if (m_pBinder->transact(IStub::RELEASE, data, &reply) != android::NO_ERROR) {
//            MARSHAL_DBGOUT(MSHDBG_ERROR,
//                    printf("Call stub release failed.\n"));
//        }

        delete this;

        return 0;
    }

    MARSHAL_DBGOUT(MSHDBG_CREF, printf(
            "ProxyRelease: %d\n", lRefs));

    return (UInt32)lRefs;
}

ECode CObjectProxy::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IProxy *)this) {
        *pIID = EIID_IProxy;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode CObjectProxy::GetInterface(
    /* [in] */ UInt32 uIndex,
    /* [out] */ IInterface **ppObj)
{
    assert(ppObj != NULL);

    if (uIndex < (UInt32)m_cInterfaces) {
        *ppObj = (IInterface *)&(m_pInterfaces[uIndex].m_pvVptr);
        this->AddRef();
        return NOERROR;
    }
    MARSHAL_DBGOUT(MSHDBG_WARNING, printf(
            "Proxy: IndexQI failed - idx(%d), cInterfaces(%d)\n",
            uIndex, m_cInterfaces));

    return E_NO_INTERFACE;
}

ECode CObjectProxy::GetInterfaceIndex(
     /* [in] */ IInterface *pObj,
     /* [out] */ UInt32* pIndex)
{
    assert(pIndex != NULL);

    Int32 Index = 0;
    while (Index < m_cInterfaces) {
        if ((void*)pObj == &(m_pInterfaces[Index].m_pvVptr)) {
            *pIndex = Index;
            return NOERROR;
        }
        Index++;
    }
    return E_NO_INTERFACE;
}

ECode CObjectProxy::GetClassID(
    /* [out] */ EMuid *pClsid)
{
    assert(pClsid != NULL);

    *pClsid = ((CIClassInfo*)m_pInfo)->clsid;
    return NOERROR;
}

ECode CObjectProxy::GetClassInfo(
    /* [out] */ CIClassInfo **ppClassInfo)
{
    assert(ppClassInfo != NULL);

    *ppClassInfo = m_pInfo;
    return NOERROR;
}

ECode CObjectProxy::S_CreateObject(
    /* [in] */ REMuid rclsid,
    /* [in] */ const android::sp<android::IBinder> & binder,
    /* [out] */ IProxy **ppIProxy)
{
    CObjectProxy *pProxy;
    CInterfaceProxy *pInterfaces;
    Int32 n;
    ECode ec;

    if (ppIProxy == NULL) return E_INVALID_ARGUMENT;

    pProxy = new CObjectProxy();
    if (!pProxy) {
        MARSHAL_DBGOUT(MSHDBG_ERROR,
                printf("Create proxy object: out of memory.\n"));
        return E_OUT_OF_MEMORY;
    }

    pProxy->m_pBinder = binder;

    ec = LookupClassInfo(rclsid, &(pProxy->m_pInfo));
    if (FAILED(ec)) {
        ec = GetRemoteClassInfo(pProxy->m_pBinder.get(),
                                 rclsid,
                                 &pProxy->m_pInfo);
        if (FAILED(ec)) goto ErrorExit;
    }

    pProxy->m_cInterfaces = ((CIClassInfo*)(pProxy->m_pInfo))->interfaceNum;
    pInterfaces = new CInterfaceProxy[pProxy->m_cInterfaces];
    if (!pInterfaces) {
        MARSHAL_DBGOUT(MSHDBG_ERROR,
                printf("Create proxy interfaces: out of memory.\n"));
        ec = E_OUT_OF_MEMORY;
        goto ErrorExit;
    }
    pProxy->m_pInterfaces = pInterfaces;
    memset(pInterfaces, 0, sizeof(CInterfaceProxy) * pProxy->m_cInterfaces);
    for (n = 0; n < pProxy->m_cInterfaces; n++) {
        pInterfaces[n].m_uIndex = n;
        pInterfaces[n].m_pOwner = pProxy;
        CIInterfaceInfo *pInterfaceInfo =
            (CIInterfaceInfo *)GetUnalignedPtr(
                    pProxy->m_pInfo->interfaces + n);
        pInterfaces[n].m_pInfo = pInterfaceInfo;
        pInterfaces[n].m_pvVptr = g_marshalVtbl;

#ifdef _x86
        pInterfaces[n].m_pvProxyEntry = (PVoid)&CInterfaceProxy::ProxyEntry;
#elif defined(_arm)
        pInterfaces[n].m_pvProxyEntry = (PVoid)&__ProxyEntry;
#elif defined(_mips)
        pInterfaces[n].m_pvProxyEntry = (PVoid)&ProxyContinue;
#else
#error unknown architecture
#endif
    }

    ec = RegisterImportObject(pProxy->m_pBinder.get(), (IProxy*)pProxy);
    if (FAILED(ec)) {
        MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                "Create proxy: register import object failed, ec(%x)\n",
                ec));
        goto ErrorExit;
    }

    *ppIProxy = (IProxy*)pProxy;

    return NOERROR;

ErrorExit:
    delete pProxy;
    return ec;
}
