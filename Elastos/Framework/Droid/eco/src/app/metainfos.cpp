
#include <elastos.h>
#include <clsinfo.h>

#include <dlfcn.h>

using namespace Elastos;

CAR_INLINE void RelocateUnalignedPtr(void *pPtr, size_t offset)
{
#ifdef _x86
    *(size_t *)pPtr += offset;
#else
    union
    {
        size_t  s;
        byte_t  bytes[4];
    } u;
    u.bytes[0] = ((byte_t *)pPtr)[0];
    u.bytes[1] = ((byte_t *)pPtr)[1];
    u.bytes[2] = ((byte_t *)pPtr)[2];
    u.bytes[3] = ((byte_t *)pPtr)[3];

    u.s += offset;

    ((byte_t *)pPtr)[0] = u.bytes[0];
    ((byte_t *)pPtr)[1] = u.bytes[1];
    ((byte_t *)pPtr)[2] = u.bytes[2];
    ((byte_t *)pPtr)[3] = u.bytes[3];
#endif
}

void RelocateModuleInfo(
    /* [in] */ CIModuleInfo* moduleInfoSrc,
    /* [out] */ CIModuleInfo* moduleInfoDest)
{
    Int32 i, j;
    CIClassInfo * pClassDir;
    CIInterfaceInfo ** ppInterfaceDir;
    CIInterfaceInfo * pInterfaceDir;
    CIMethodInfo * pMethodDir;

    memcpy(moduleInfoDest, moduleInfoSrc, *(UInt32*)moduleInfoSrc);

    moduleInfoDest->interfaces = (CIInterfaceInfo*)((Byte *)moduleInfoDest +
        (UInt32)moduleInfoDest->interfaces);
    pInterfaceDir = moduleInfoDest->interfaces;
    for (i = 0; i < moduleInfoDest->interfaceNum; i++) {
        pInterfaceDir[i].methods = (CIMethodInfo *) \
              ((Byte *)moduleInfoDest + (UInt32)pInterfaceDir[i].methods);
        pMethodDir = pInterfaceDir[i].methods;
        for (j = 0; j < pInterfaceDir[i].methodNumMinus4; j++) {
            pMethodDir[j].params = (CIBaseType*) \
                 ((Byte *)moduleInfoDest + (UInt32)pMethodDir[j].params);
        }
    }

    moduleInfoDest->classes = (CIClassInfo *)((Byte *)moduleInfoDest +
       (UInt32)moduleInfoDest->classes);
    pClassDir = moduleInfoDest->classes;
    for (i = 0; i < moduleInfoDest->classNum; i++) {
        pClassDir[i].interfaces = (CIInterfaceInfo **) \
                ((Byte *)moduleInfoDest + (Int32)(pClassDir[i].interfaces));
        ppInterfaceDir = pClassDir[i].interfaces;
        for (j = 0; j < pClassDir[i].interfaceNum; j++) {
            RelocateUnalignedPtr(ppInterfaceDir + j, (size_t)moduleInfoDest);
        }
        pClassDir[i].pszUunm =
                (char*)((Byte *)moduleInfoDest + (UInt32)pClassDir[i].pszUunm);
    }
}

CIModuleInfo* AcquireModuleInfo(
    /* [in] */ String moduleName)
{
    CIModuleInfo* moduleInfoSrc;
    CIModuleInfo* moduleInfoDest;
    void* moduleHandle = dlopen(moduleName, RTLD_NOW);

    typedef ECode (STDCALL *CarDllGetClassObject_t)(
            REMuid clsid, REIID riid, IInterface **ppObj);

    CarDllGetClassObject_t func = (CarDllGetClassObject_t)
            dlsym(moduleHandle, "DllGetClassObject");

    func(ECLSID_ClassInfo, EIID_IInterface, (IInterface**)&moduleInfoSrc);

    moduleInfoDest = (CIModuleInfo *)calloc(moduleInfoSrc->totalSize, 1);
    RelocateModuleInfo(moduleInfoSrc, moduleInfoDest);
    return moduleInfoDest;
}

ECode AcquireClassInfo(
    /* [in] */ const ClassID & classId,
    /* [in] */ CIModuleInfo* moduleInfo,
    /* [out] */ CIClassInfo** classInfo)
{
    if (classInfo == NULL) return E_INVALID_ARGUMENT;

    for (Int32 m = 0; m < moduleInfo->classNum; m++) {
        if (moduleInfo->classes[m].clsid == classId) {
            *classInfo = &(moduleInfo->classes[m]);
            return NOERROR;
        }
    }

    return E_DOES_NOT_EXIST;
}
