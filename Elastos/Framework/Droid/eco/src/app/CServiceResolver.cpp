
#include "app/CServiceResolver.h"
#include "utils/log.h"

#include <sys/mman.h>

extern ECode Proxy_WriteParameters(
    /* [in] */ const CIMethodInfo* methodInfo,
    /* [in] */ UInt32* args,
    /* [in, out] */ IParcel* params);

Address sResolverEntryAddress = NULL;

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
            "stmdb  sp!, {r0 - r3};"            \
             "mov    r1, #0xff;"                 \
            "ldr    pc, [r0, #4];"              \
            "nop;"                              \
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
    return CServiceResolver::CInterfaceProxy::ProxyEntry(puArgs);
}

#ifdef _GNUC
#define DECL_PROXY_ENTRY()              \
    __asm__(                            \
        ".text;"                        \
        ".align 4;"                     \
        ".globl __ProxyEntry;"         \
        "__ProxyEntry:"                \
        "stmdb  sp!, {r1, lr};"         \
        "add    r0, sp, #4;"            \
        "bl     GlobalProxyEntry;"     \
        "ldr    lr, [sp, #4];"          \
        "add    sp, sp, #24;"           \
        "mov    pc, lr;"                \
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

#ifdef _x86

#define SYS_PROXY_EXIT(ec, pret, argc)  \
    do {                                \
        UInt32 n;                         \
        n = *(UInt32 *)(pret);            \
        n &= PROXY_ENTRY_MASK;          \
        n += PROXY_ENTRY_BASE + ((argc) << PROXY_ENTRY_SHIFT);  \
        *(UInt32 *)(pret) = n;            \
        return (ec);                    \
    } while (0)

#elif defined(_arm)

#define SYS_PROXY_EXIT(ec, pret, argc)  \
    do {                                \
        return (ec);                    \
    } while (0)

#elif defined(_mips)

#ifdef _GNUC

#define SYS_PROXY_EXIT(ec, pret, argc)  \
    do {                                \
            return  (ec);               \
    } while (0);
#elif defined(_EVC)


#define SYS_PROXY_EXIT(ec, pret, argc)  \
    do {                                \
            return  (ec);               \
    } while (0)

#else
#error unknown architecture
#endif
#endif

inline UInt32 CalcMethodIndex(UInt32 callerAddr)
{
    return (callerAddr - PROXY_ENTRY_BASE) >> PROXY_ENTRY_SHIFT;
}

CAR_INLINE void *GetUnalignedPtr(void* ptr)
{
#ifdef _x86
    return *(void **)ptr;
#else
    union
    {
        void    *p;
        byte_t  bytes[4];
    } u;
    u.bytes[0] = ((byte_t *)ptr)[0];
    u.bytes[1] = ((byte_t *)ptr)[1];
    u.bytes[2] = ((byte_t *)ptr)[2];
    u.bytes[3] = ((byte_t *)ptr)[3];

    return u.p;
#endif
}


PInterface CServiceResolver::CInterfaceProxy::Probe(
    /* [in] */ CServiceResolver::CInterfaceProxy* proxy,
    /* [in] */ REIID riid)
{
    return proxy->mOwner->Probe(riid);
    return NULL;
}

UInt32 CServiceResolver::CInterfaceProxy::AddRef(
    /* [in] */ CServiceResolver::CInterfaceProxy* proxy)
{
    return proxy->mOwner->AddRef();
    return 0;
}

UInt32 CServiceResolver::CInterfaceProxy::Release(
    /* [in] */ CServiceResolver::CInterfaceProxy* proxy)
{
    return proxy->mOwner->Release();
    return 0;
}

ECode CServiceResolver::CInterfaceProxy::GetInterfaceID(
    /* [in] */ CServiceResolver::CInterfaceProxy* proxy,
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    return proxy->mOwner->GetInterfaceID(object, iid);
}

ECode CServiceResolver::CInterfaceProxy::WriteParameters(
    /* [in] */ UInt32 methodIndex,
    /* [in] */ UInt32* args,
    /* [in] */ MarshalHeader* header,
    /* [in, out] */ IParcel* params)
{
    const CIMethodInfo *methodInfo = &(mInfo->methods[methodIndex]);

    return Proxy_WriteParameters(methodInfo, args, params);
}

UInt32 CServiceResolver::CInterfaceProxy::CountMethodArgs(
    /* [in] */ UInt32 methodIndex)
{
    return GET_LENGTH((mInfo->methods[methodIndex]).reserved1);
}

ECode CServiceResolver::CInterfaceProxy::ProxyEntry(
    /* [in] */ UInt32* args)
{
    CServiceResolver::CInterfaceProxy* itfProxy;
    UInt32 methodIndex;
    MarshalHeader* header = NULL;
    IParcel* params = NULL;
    ECode ec;
#ifndef _mips
    UInt32 argc;
#endif

    args++; // skip ret address

    itfProxy = (CServiceResolver::CInterfaceProxy *)*args;
    args++; // skip this

#ifdef _x86
    methodIndex = CalcMethodIndex(*(UInt32 *)((UInt32)&args - 4));
#elif defined(_arm)
    methodIndex = args[-3];
#elif defined(_mips)
    methodIndex = CalcMethodIndex(*(args - 4) - 4);
#else
#error unknown architecture
#endif

#ifndef _mips
    // Get the stack length, not contain "this"
    argc = itfProxy->CountMethodArgs(methodIndex);
#endif

    header = (MarshalHeader*)malloc(sizeof(MarshalHeader));
    if (!header) {
        LOGE("Proxy: malloc() failed.\n");
        ec = E_OUT_OF_MEMORY;
        goto ProxyExit;
    }
    header->m_uMagic = MARSHAL_MAGIC;
    header->m_oid = 0;
    header->m_hInterfaceIndex = itfProxy->mIndex;
    header->m_hMethodIndex = methodIndex + 3;
    header->m_pContext = CTX_SAME_DOMAIN;

    CCallbackParcel::New(&params);
    ec = itfProxy->WriteParameters(methodIndex, args, header, params);
    if (SUCCEEDED(ec)) {
        ec = itfProxy->mOwner->mServiceApartment->ScheduleInvokeService(
                itfProxy->mOwner, (Handle32)header, params);
    }
    else {
        LOGE("proxy MarshalIn() failed. ec = %x\n", ec);
    }

ProxyExit:
#ifndef _mips
    SYS_PROXY_EXIT(ec, &args - 1, argc);
#else
    SYS_PROXY_EXIT(ec, &args - 1, 0);
#endif
    return NOERROR;
}

CServiceResolver::CServiceResolver()
{
}

CServiceResolver::~CServiceResolver()
{
    /*
        CInterfaceProxy*    mInterfaceProxies;
        CInterfaceStub*     mInterfaceStubs;

    */
}

PInterface CServiceResolver::Probe(REIID riid)
{
    int n;

    if (riid == EIID_IInterface) {
        return (IInterface *)this;
    }

    for (n = 0; n < mInterfaceNum; n++) {
        if (riid == mInterfaceProxies[n].mInfo->iid) {
            break;
        }
    }
    if (n == mInterfaceNum) {
        LOGE("Proxy(%d): QI failed.\n");

        return NULL;
    }

    return (IInterface *)&(mInterfaceProxies[n].mVtptr);
}

UInt32 CServiceResolver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CServiceResolver::Release()
{
    return ElRefBase::Release();
}

ECode CServiceResolver::GetInterfaceID(
    /* [in] */ IInterface *object,
    /* [out] */ InterfaceID *iid)
{
    return E_NOT_IMPLEMENTED;
}

#define PAGE_SIZE (1u << 12)
#define PAGE_MASK (~(PAGE_SIZE - 1))
#define PAGE_ALIGN(va) (((va)+PAGE_SIZE-1)&PAGE_MASK)

void CServiceResolver::InitEntry()
{
    sResolverEntryAddress =
            (Address)mmap((void*)0,
                          PAGE_ALIGN(PROXY_ENTRY_NUM * PROXY_ENTRY_SIZE),
                          PROT_READ | PROT_WRITE | PROT_EXEC,
                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (sResolverEntryAddress == 0) {
        LOGE("out of memory.\n");
        return;
    }

#if defined(_x86)
    char *p = (char *)sResolverEntryAddress;
    for (UInt32 n = 0; n < PROXY_ENTRY_NUM; n++) {
        memcpy(p, (void*)&ProxyEntryFunc, PROXY_ENTRY_SIZE);
        *(Int16*)&(p[SYS_PROXY_RET_OFFSET]) = ( (n + 1) << 2);
        p+= PROXY_ENTRY_SIZE;
    }
#elif defined(_arm)
    char * p = (char *)sResolverEntryAddress;
    for (Int32 n = 0; n < PROXY_ENTRY_NUM; n++) {
        memcpy(p, (void *)&ProxyEntryFunc, PROXY_ENTRY_SIZE);
        p[4] = n;
        p += PROXY_ENTRY_SIZE;
    }
#else
#error "unsupported architecture yet."
#endif

    g_marshalVtbl[0] = (UInt32)&CServiceResolver::CInterfaceProxy::Probe;
    g_marshalVtbl[1] = (UInt32)&CServiceResolver::CInterfaceProxy::AddRef;
    g_marshalVtbl[2] = (UInt32)&CServiceResolver::CInterfaceProxy::Release;
    g_marshalVtbl[3] = (UInt32)&CServiceResolver::CInterfaceProxy::GetInterfaceID;
    for (int m = 4; m < MSH_MAX_METHODS; m++) {
        g_marshalVtbl[m] = PROXY_ENTRY_BASE + ((m - 3) << PROXY_ENTRY_SHIFT);
    }
}

IInterface* CServiceResolver::Create(
    /* [in] */ IInterface* object,
    /* [in] */ CIClassInfo* classInfo,
    /* [in] */ IServiceApartment* apartment)
{
    Int32 n;
    InterfaceID iid;
    CServiceResolver* service = NULL;
    CServiceResolver::CInterfaceProxy* interfaceProxies = NULL;
    CServiceResolver::CInterfaceStub* interfaceStubs = NULL;

    object->GetInterfaceID(object, &iid);

    service = new CServiceResolver();
    if (service == NULL) {
        LOGE("Create service object: out of memory.\n");
        return NULL;
    }

    service->mService = (IObject*)object->Probe(EIID_IObject);
    service->mServiceApartment = apartment;

    if (sResolverEntryAddress == 0) {
        InitEntry();
    }

    service->mClassInfo = classInfo;
    service->mInterfaceNum = service->mClassInfo->interfaceNum;
    interfaceProxies = new CInterfaceProxy[service->mInterfaceNum];
    if (interfaceProxies == NULL) {
        LOGE("Create proxy interfaces: out of memory.\n");
        goto ErrorExit;
    }
    service->mInterfaceProxies = interfaceProxies;
    memset(interfaceProxies, 0, sizeof(CInterfaceProxy) * (service->mInterfaceNum));
    for (n = 0; n < service->mInterfaceNum; n++) {
        interfaceProxies[n].mIndex = n;
        interfaceProxies[n].mContext = CTX_SAME_DOMAIN;
        interfaceProxies[n].mOwner = service;
        CIInterfaceInfo *interfaceInfo =
            (CIInterfaceInfo *)GetUnalignedPtr(
                service->mClassInfo->interfaces + n);
        interfaceProxies[n].mInfo = interfaceInfo;
        interfaceProxies[n].mVtptr = g_marshalVtbl;

#ifdef _x86
        interfaceProxies[n].mProxyEntry = (PVoid)&CServiceResolver::CInterfaceProxy::ProxyEntry;
#elif defined(_arm)
        interfaceProxies[n].mProxyEntry = (PVoid)&__ProxyEntry;
#elif defined(_mips)
        interfaceProxies[n].mProxyEntry = (PVoid)&ProxyContinue;
#else
#error unknown architecture
#endif
    }

    interfaceStubs = new CInterfaceStub[service->mInterfaceNum];
    if (!interfaceStubs) {
        LOGE("Create stub: out of memory.\n");
        goto ErrorExit;
    }
    service->mInterfaceStubs = interfaceStubs;
    for (n = 0; n < service->mInterfaceNum; n++) {
        CIInterfaceInfo *interfaceInfo =
            (CIInterfaceInfo *)GetUnalignedPtr(
                service->mClassInfo->interfaces + n);
        IInterface* obj = object->Probe(interfaceInfo->iid);
        if (!obj) {
            printf("Create stub: no such interface.\n");
            goto ErrorExit;
        }
        interfaceStubs[n].mInfo = interfaceInfo;
        interfaceStubs[n].mObject = obj;
        interfaceStubs[n].mOwner = service;
    }

    return service->Probe(iid);

ErrorExit:

    delete service;
    return NULL;
}
