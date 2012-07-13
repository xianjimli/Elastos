//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <stdio.h>
#include <stdlib.h>
#include "clsbase.h"

//#ifdef _win32
#include <eltypes.h>

#ifndef _WINDEF_H
typedef unsigned long long  UINT64;
#endif

//#else
//#include <mstypes.h>
//#endif
#include <assert.h>

#include "abrgcls.h"

_ELASTOS_NAMESPACE_USING

class CAbridgedBuffer
{
public:
    int Prepare(const CLSModule *pModule);
    void GenerateAbridged(void *pvBuffer);

private:
    inline int AllocData(int nSize);
    inline void WriteData(int nOffset, void *pvData, int size);
    inline void WriteParams(int nOffset, AbridgedParamsInfo c);
    inline void WriteInt(int nOffset, int n);
    inline int IWriteInfo(AbridgedInterfaceInfo *pEntry);
    inline void CWriteInfo(AbridgedClassInfo *pEntry);

    AbridgedParamsInfo GetParamAttrib(DWORD dwAttribs);
    AbridgedParamsInfo GetParamType(TypeDescriptor *pType,
                        DWORD dwAttribs, unsigned char *pStackSize);
    void WriteMethodInfo(int nAddr, MethodDescriptor *pDesc);
    int WriteMethodInfos(int nAddr, InterfaceDescriptor *pDesc);
    int WriteClsIntfs(ClassDescriptor *pDesc, int cInterfaces);
    void WriteClasses();
    void WriteInterfaces();

    int TotalParamSize(InterfaceDescriptor *pDesc);
    int CalcMethodNumber(InterfaceDescriptor *pDesc);
    void InitInterfaceIndexTable();
    int ClsIntfNumber(ClassDescriptor *pDesc);

    void AddClassInterfaceToIndex(ClassDescriptor *pClass);
    void InitClassIndexTable();

private:
    const CLSModule     *m_pModule;

    char                *m_pBuffer;
    int                 m_nClassOffset;
    int                 m_nInterfaceOffset;
    int                 m_nDataOffset;

    int                 m_cClasses;
    int                 m_interfaceNumbers[c_nMaxClassNumber];
    unsigned char       m_classVirTable[c_nMaxInterfaceNumber];

    int                 m_cInterfaces;
    union {
        int             m_methodNumbers[c_nMaxInterfaceNumber];
        int             m_interfaceAddrs[c_nMaxInterfaceNumber];
    } m_mi;

#define m_methodNumbers     m_mi.m_methodNumbers
#define m_interfaceAddrs    m_mi.m_interfaceAddrs
};

int CAbridgedBuffer::AllocData(int nSize)
{
    int nOffset = m_nDataOffset;

    m_nDataOffset += RoundUp4(nSize);
    return nOffset;
}

void CAbridgedBuffer::WriteParams(int nOffset, AbridgedParamsInfo c)
{
    *(AbridgedParamsInfo *)(m_pBuffer + nOffset) = c;
}

void CAbridgedBuffer::WriteInt(int nOffset, int n)
{
    *(int *)(m_pBuffer + nOffset) = n;
}

void CAbridgedBuffer::WriteData(int nOffset, void *pvData, int nSize)
{
    memcpy(m_pBuffer + nOffset, pvData, nSize);
}

int CAbridgedBuffer::IWriteInfo(AbridgedInterfaceInfo *pInfo)
{
    int nOffset = m_nInterfaceOffset;

    memcpy(m_pBuffer + nOffset, pInfo, sizeof(AbridgedInterfaceInfo));
    m_nInterfaceOffset += sizeof(AbridgedInterfaceInfo);

    return nOffset;
}

void CAbridgedBuffer::CWriteInfo(AbridgedClassInfo *pInfo)
{
    memcpy(m_pBuffer + m_nClassOffset, pInfo, sizeof(AbridgedClassInfo));
    m_nClassOffset += sizeof(AbridgedClassInfo);
}

AbridgedParamsInfo CAbridgedBuffer::GetParamAttrib(DWORD dwAttribs)
{
    AbridgedParamsInfo attr = 0;

    if (dwAttribs & ParamAttrib_in) attr |= Param_Attrib_in;
    if (dwAttribs & ParamAttrib_out) attr |= Param_Attrib_out;

    return attr;
}

AbridgedParamsInfo CAbridgedBuffer::GetParamType(
    TypeDescriptor *pType, DWORD dwAttribs, unsigned char *pStackSize)
{
    char *pszArch;
    TypeDescriptor type;

    switch (pType->type) {
        case Type_Char8:
        case Type_Byte:
        case Type_Int8:
        case Type_Boolean:
            *pStackSize += 1;
            return pType->nPointer ? Param_Type_puint8:Param_Type_uint8;

        case Type_Int16:
        case Type_UInt16:
        case Type_Char16:
            *pStackSize += 1;
            return pType->nPointer ? Param_Type_puint16:Param_Type_uint16;

        case Type_Int32:
        case Type_UInt32:
        case Type_Float:
        case Type_enum:
        case Type_ECode:
            *pStackSize += 1;
            return pType->nPointer ? Param_Type_puint32:Param_Type_uint32;

        case Type_Int64:
        case Type_UInt64:
        case Type_Double:
            if (pType->nPointer) {
                *pStackSize += 1;
                return Param_Type_puint64;
            }

            pszArch = getenv("XDK_TARGET_CPU");
            if (pszArch && !strcmp(pszArch, "mips") && !(*pStackSize % 2)) {
                *pStackSize += 1;
            }

            *pStackSize += sizeof(UINT64) / 4;
            return Param_Type_uint64;

        case Type_EMuid:
            //if is EMuid, we only push a pointer in stack.
            *pStackSize += 1;
            return Param_Type_pguid;

        case Type_struct:
        {
            UINT uSize = (UINT)(m_pModule->ppStructDir[pType->sIndex]->pDesc->nAlignSize);
            uSize = (uSize % 4 == 0 ? uSize / 4 : (uSize / 4 + 1));
            AbridgedParamsInfo pa = (AbridgedParamsInfo)uSize;
            if (pType->nPointer) {
                *pStackSize += 1;
                return (Param_Type_pstructure | PUSH_SIZE(pa));
            }
            else {//if is struct, we only push a pointer in stack.
                *pStackSize += 1;
                return (Param_Type_structure | PUSH_SIZE(pa));
            }
        }

        case Type_DateTime:
        {
            UINT uSize = sizeof(DateTime);
            uSize = (uSize % 4 == 0 ? uSize / 4 : (uSize / 4 + 1));
            if (pType->nPointer) {
                *pStackSize += 1;
                return (Param_Type_pstructure | PUSH_SIZE(uSize));
            }
            else {//if is struct, we only push a pointer in stack.
                *pStackSize += 1;
                return (Param_Type_structure | PUSH_SIZE(uSize));
            }
        }
        case Type_EGuid:
            //if is EGuid, we only push a pointer in stack.
            *pStackSize += 1;
            return Param_Type_pclsid;

        case Type_CString:
            *pStackSize += 1;
            return pType->nPointer ? Param_Type_pCString:Param_Type_CString;

        case Type_String:
            *pStackSize += 1;
            return pType->nPointer ? Param_Type_pString:Param_Type_String;

        case Type_StringBuf:
            *pStackSize += 1;
            return (Param_Type_StringBuf | PUSH_SIZE(pType->nSize));

        case Type_ArrayOf:
            *pStackSize += 1;
            if (pType->pNestedType && (Type_interface == pType->\
                pNestedType->type) && (dwAttribs & ParamAttrib_in)) {
                *pStackSize |= 0x80;
            }
            return Param_Type_ArrayOf;
        case Type_MemoryBuf:
        case Type_BufferOf:
            *pStackSize += 1;
            if (pType->pNestedType && (Type_interface == pType->\
                pNestedType->type) && (dwAttribs & ParamAttrib_in)) {
                *pStackSize |= 0x80;
            }
            return Param_Type_BufferOf;

        case Type_EventHandler:
            if (pType->nPointer) {
                *pStackSize += 1;
                return Param_Type_peventhandler;
            }
            *pStackSize += 4;
            return Param_Type_eventhandler;

        case Type_interface:
            *pStackSize += 1;
            if (dwAttribs & ParamAttrib_in) {
                *pStackSize |= 0x80;
            }
            if (dwAttribs & ParamAttrib_out) {
                *pStackSize |= 0x40;
            }
            return 1==pType->nPointer ? Param_Type_interface:Param_Type_pinterface;

        case Type_alias:
            GetOriginalType(m_pModule, pType, &type);
            return GetParamType(&type, dwAttribs, pStackSize);

        default:
            assert(TRUE == FALSE);
            return Param_Type_none;
    }
    return Param_Type_none;
}

void CAbridgedBuffer::WriteMethodInfo(int nAddr, MethodDescriptor *pDesc)
{
    AbridgedParamsInfo param = 0;
    int n, nParamAddr;
    AbridgedMethodInfo entry;

    nParamAddr = AllocData(pDesc->cParams * sizeof(AbridgedParamsInfo));
    entry.cParams = (unsigned char)pDesc->cParams;
    entry.pParams = (AbridgedParamsInfo *)nParamAddr;
    entry.nStackSize = 0;

    for (n = 0; n < pDesc->cParams; n++) {
        param = GetParamAttrib(pDesc->ppParams[n]->dwAttribs);
        param |= GetParamType(&pDesc->ppParams[n]->type,
                        pDesc->ppParams[n]->dwAttribs, &entry.nStackSize);
        WriteParams(nParamAddr, param);
        nParamAddr += sizeof(AbridgedParamsInfo);
    }

    WriteData(nAddr, &entry, sizeof(AbridgedMethodInfo));
}

int CAbridgedBuffer::WriteMethodInfos(int nAddr, InterfaceDescriptor *pDesc)
{
    int n;

    if (pDesc != m_pModule->ppInterfaceDir[0]->pDesc) {
        nAddr = WriteMethodInfos(nAddr, m_pModule-> \
                    ppInterfaceDir[pDesc->sParentIndex]->pDesc);
    }
    for (n = 0; n < pDesc->cMethods; n++) {
        WriteMethodInfo(nAddr, pDesc->ppMethods[n]);
        nAddr += sizeof(AbridgedMethodInfo);
    }

    return nAddr;
}

void CAbridgedBuffer::WriteInterfaces()
{
    int n, nAddr;
    InterfaceDescriptor *pDesc;
    AbridgedInterfaceInfo entry;

    for (n = 0; n < m_pModule->cInterfaces; n++) {
        if (m_methodNumbers[n] != -1) {
            pDesc = m_pModule->ppInterfaceDir[n]->pDesc;

            nAddr = AllocData(m_methodNumbers[n] * sizeof(AbridgedMethodInfo));
            WriteMethodInfos(nAddr, pDesc);

            entry.cMethods = m_methodNumbers[n];
            entry.pMethods = (AbridgedMethodInfo *)nAddr;
            memcpy(&entry.iid, &pDesc->iid, sizeof(InterfaceID));

            m_interfaceAddrs[n] = IWriteInfo(&entry);
        }
    }
}

int CAbridgedBuffer::WriteClsIntfs(ClassDescriptor *pDesc, int cInterfaces)
{
    int n, nAddr, cLocal = 0;

    nAddr = AllocData(cInterfaces * sizeof(AbridgedInterfaceInfo *));

    for (n = 0; n < pDesc->cInterfaces; n++) {
        if (!(pDesc->ppInterfaces[n]->wAttribs & ClassInterfaceAttrib_callback)
            && !(m_pModule->ppInterfaceDir[pDesc->ppInterfaces[n]->sIndex]
            ->pDesc->dwAttribs & InterfaceAttrib_local)) {
            WriteInt(nAddr + (n - cLocal) * sizeof(int),
                    m_interfaceAddrs[pDesc->ppInterfaces[n]->sIndex]);
        }
        else {
            cLocal++;
        }
    }

    return nAddr;
}

void CAbridgedBuffer::WriteClasses()
{
    int n, nAddr;
    AbridgedClassInfo entry;

    for (n = 0; n < m_pModule->cClasses; n++) {
        if (m_interfaceNumbers[n] != 0) {
            nAddr = WriteClsIntfs(
                m_pModule->ppClassDir[n]->pDesc, m_interfaceNumbers[n]);

            entry.cInterfaces = m_interfaceNumbers[n];
            entry.ppInterfaces = (AbridgedInterfaceInfo **)nAddr;

            memcpy(&entry.clsid, &m_pModule->ppClassDir[n]-> \
                pDesc->clsid, sizeof(InterfaceID));
            entry.pszUunm = (char*)offsetof(AbridgedModuleInfo, szUunm);

            CWriteInfo(&entry);
        }
    }
}

void CAbridgedBuffer::GenerateAbridged(void *pvBuffer)
{
    AbridgedModuleInfo *pInfo = (AbridgedModuleInfo *)pvBuffer;

    m_pBuffer = (char *)pvBuffer;

    pInfo->cClasses = m_cClasses;
    pInfo->cInterfaces = m_cInterfaces;
    pInfo->pClasses = (AbridgedClassInfo *)m_nClassOffset;
    pInfo->pInterfaces = (AbridgedInterfaceInfo *)m_nInterfaceOffset;
    if (m_pModule->pszUunm) {
        strcpy(pInfo->szUunm, m_pModule->pszUunm);
    }
    else {
        pInfo->szUunm[0] = '\0';
    }

    WriteInterfaces();
    WriteClasses();

    pInfo->nTotalSize = m_nDataOffset;
}

int CAbridgedBuffer::CalcMethodNumber(InterfaceDescriptor *pDesc)
{
    int cMethods = pDesc->cMethods;

    while (pDesc != m_pModule->ppInterfaceDir[0]->pDesc) {
        pDesc = m_pModule-> \
                    ppInterfaceDir[pDesc->sParentIndex]->pDesc;
        cMethods += pDesc->cMethods;
    }

    return cMethods;
}

void CAbridgedBuffer::InitInterfaceIndexTable()
{
    int n, cMethods;
    InterfaceDirEntry *pEntry;

    m_cInterfaces = 0;
    memset(m_methodNumbers, -1, sizeof(int) * m_pModule->cInterfaces);

    for (n = 0; n < m_pModule->cInterfaces; n++) {
        pEntry = m_pModule->ppInterfaceDir[n];
        if (!pEntry->pszNameSpace &&
            !(pEntry->pDesc->dwAttribs & InterfaceAttrib_local)) {
            cMethods = CalcMethodNumber(pEntry->pDesc);
            m_cInterfaces++;
            m_methodNumbers[n] = cMethods;
        }
    }
}

void CAbridgedBuffer::AddClassInterfaceToIndex(ClassDescriptor *pClass)
{
    int n, cMethods, nIndex, nIntfIdx, nVirTable=0;
    InterfaceDirEntry *pEntry;
    InterfaceDirEntry *pIntfEntry;
    InterfaceDescriptor *pIntfDesc;

    for (n = 0; n < pClass->cInterfaces; n++) {
        nIndex = pClass->ppInterfaces[n]->sIndex;
        pEntry = m_pModule->ppInterfaceDir[nIndex];
        pIntfDesc = pEntry->pDesc;

        if (!(pClass->ppInterfaces[n]->wAttribs & ClassInterfaceAttrib_callback)
            && !(pEntry->pDesc->dwAttribs & InterfaceAttrib_local)
            && pEntry->pszNameSpace
            && -1 == m_methodNumbers[nIndex]) {
            cMethods = CalcMethodNumber(pEntry->pDesc);
            if (cMethods >= 0) {
                m_cInterfaces++;
                m_methodNumbers[nIndex] = cMethods;
            }
        }
        // add parents interface to abridge cls
        while (pIntfDesc->sParentIndex != 0
            && !(pClass->ppInterfaces[n]->wAttribs & ClassInterfaceAttrib_callback)
            && !(pEntry->pDesc->dwAttribs & InterfaceAttrib_local)) {
            nIntfIdx = CreateClassInterface(pIntfDesc->sParentIndex, pClass);
            pIntfEntry = m_pModule->ppInterfaceDir[pIntfDesc->sParentIndex];
            m_classVirTable[pIntfDesc->sParentIndex] = nVirTable;
            pIntfDesc = pIntfEntry->pDesc;

            if (m_methodNumbers[pIntfDesc->sParentIndex] == -1) {
                cMethods = CalcMethodNumber(m_pModule->ppInterfaceDir[pIntfDesc->sParentIndex]->pDesc);
                if (cMethods >= 0) {
                    m_cInterfaces++;
                    m_methodNumbers[pIntfDesc->sParentIndex] = cMethods;
                }
            }
        }
        if (m_classVirTable[nIndex] == 0xff) m_classVirTable[nIndex] = nVirTable++;
    }
}

int CAbridgedBuffer::ClsIntfNumber(ClassDescriptor *pDesc)
{
    int n, c = 0;

    for (n = 0; n < pDesc->cInterfaces; n++) {
        if (!(pDesc->ppInterfaces[n]->wAttribs & ClassInterfaceAttrib_callback)
            && !(m_pModule->ppInterfaceDir
            [pDesc->ppInterfaces[n]->sIndex]->pDesc->dwAttribs
            & InterfaceAttrib_local)) {
            c++;
        }
    }
    return c;
}

void CAbridgedBuffer::InitClassIndexTable()
{
    int n;
    ClassDirEntry *pEntry;

    m_cClasses = 0;
    memset(m_interfaceNumbers, 0, sizeof(int) * m_pModule->cClasses);
    memset(m_classVirTable, 0xFF, sizeof(m_classVirTable));

    for (n = 0; n < m_pModule->cClasses; n++) {
        pEntry = m_pModule->ppClassDir[n];
        if (!pEntry->pszNameSpace
            && !(pEntry->pDesc->dwAttribs & ClassAttrib_classlocal)
            && !(pEntry->pDesc->dwAttribs & ClassAttrib_t_generic)
            && 0 != pEntry->pDesc->cInterfaces) {
            AddClassInterfaceToIndex(pEntry->pDesc);
            m_cClasses++;
            m_interfaceNumbers[n] = ClsIntfNumber(pEntry->pDesc);
        }
    }
}

int CAbridgedBuffer::TotalParamSize(InterfaceDescriptor *pDesc)
{
    int n, size = 0;

    if (pDesc != m_pModule->ppInterfaceDir[0]->pDesc) {
        size += TotalParamSize(m_pModule-> \
                        ppInterfaceDir[pDesc->sParentIndex]->pDesc);
    }

    for (n = 0; n < pDesc->cMethods; n++) {
        size += RoundUp4(pDesc->ppMethods[n]->cParams * sizeof(AbridgedParamsInfo));
    }

    return size;
}

int CAbridgedBuffer::Prepare(const CLSModule *pModule)
{
    int n, size;

    m_pModule = pModule;

    InitInterfaceIndexTable();
    InitClassIndexTable();

    size = sizeof(AbridgedModuleInfo);
    if (pModule->pszUunm) size += strlen(pModule->pszUunm);
    size = m_nClassOffset = RoundUp4(size);

    size += RoundUp4(m_cClasses * sizeof(AbridgedClassInfo));
    m_nInterfaceOffset = size;

    size += RoundUp4(m_cInterfaces * sizeof(AbridgedInterfaceInfo));
    m_nDataOffset = size;

    for (n = 0; n < m_pModule->cClasses; n++) {
        if (m_interfaceNumbers[n] != 0) {
            size += RoundUp4(
                    m_interfaceNumbers[n] * sizeof(AbridgedInterfaceInfo *));
        }
    }
    for (n = 0; n < m_pModule->cInterfaces; n++) {
        if (m_methodNumbers[n] != 0) {
            size += RoundUp4(m_methodNumbers[n] * sizeof(AbridgedMethodInfo));
            size += TotalParamSize(m_pModule->ppInterfaceDir[n]->pDesc);
        }
    }

    return size;
}

int AbridgeCLS(const CLSModule *pModule, void **ppvAbrgCLS)
{
    int n;
    CAbridgedBuffer buffer;
    void *pvBuffer;
    CLSModule *pModuleTmp;

    pModuleTmp = CreateCLS();
    CopyCLS(pModule, pModuleTmp);

    n = buffer.Prepare(pModuleTmp);
    pvBuffer = (void *)new char[n];
    if (!pvBuffer) _ReturnError (CLSError_OutOfMemory);

    buffer.GenerateAbridged(pvBuffer);
    DestroyCLS(pModuleTmp);
    *ppvAbrgCLS = pvBuffer;

    _ReturnOK (CLS_NoError);
}

void DisposeAbridgedCLS(void *pvAbrgCLS)
{
    delete [] (char *)pvAbrgCLS;
}
