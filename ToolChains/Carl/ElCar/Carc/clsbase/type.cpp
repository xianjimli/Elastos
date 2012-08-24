//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <stdio.h>
#include <assert.h>

#include <eltypes.h>
#include "clsbase.h"

//alignment = 4;
#define STRUCT_TYPE_ALIGNMENT(t) \
        (t == Type_Char8 ? 1 : \
        (t == Type_Int8 ? 1 : \
        (t == Type_Byte ? 1 : \
        (t == Type_Boolean ? 1 : \
        (t == Type_Int16 ? 2 : \
        (t == Type_UInt16 ? 2 : \
        (t == Type_Char16 ? 2 : \
        (t == Type_Int32 ? 4 : \
        (t == Type_UInt32 ? 4 : \
        (t == Type_ECode ? 4 : \
        (t == Type_enum  ? 4 : \
        (t == Type_Float ? 4 : \
        (t == Type_Int64 ? 8 : \
        (t == Type_UInt64 ? 8 : \
        (t == Type_Double ? 8 : \
        (t == Type_DateTime ? sizeof(_ELASTOS DateTime) : 0))))))))))))))))

#define STRUCT_TYPE_TO_SIZE(t) \
        (t == Type_Char8 ? 1 : \
        (t == Type_Int8 ? 1 : \
        (t == Type_Byte ? 1 : \
        (t == Type_Boolean ? 1 : \
        (t == Type_Int16 ? 2 : \
        (t == Type_UInt16 ? 2 : \
        (t == Type_Char16 ? 2 : \
        (t == Type_Int32 ? 4 : \
        (t == Type_UInt32 ? 4 : \
        (t == Type_ECode ? 4 : \
        (t == Type_enum  ? 4 : \
        (t == Type_Float ? 4 : \
        (t == Type_Int64 ? 8 : \
        (t == Type_UInt64 ? 8 : \
        (t == Type_Double ? 8 : \
        (t == Type_DateTime ? sizeof(_ELASTOS DateTime) : 0))))))))))))))))

#define STRUCT_IS_PRIMARY_TYPE(t) \
        (t == Type_Char8 ? TRUE : \
        (t == Type_Byte ? TRUE : \
        (t == Type_Boolean ? TRUE : \
        (t == Type_Int16 ? TRUE : \
        (t == Type_Char16 ? TRUE : \
        (t == Type_Int32 ? TRUE : \
        (t == Type_ECode ? TRUE : \
        (t == Type_enum  ? TRUE : \
        (t == Type_Float ? TRUE : \
        (t == Type_Int64 ? TRUE : \
        (t == Type_Double ? TRUE : \
        (t == Type_DateTime ? TRUE : FALSE))))))))))))

#define STRUCT_ALIGN_SIZE(size, align) \
        ((size % align) == 0 ? (size) : ((size / align + 1) * align))

int GetIdentifyType(
    const char *pszName, const CLSModule *pModule, TypeDescriptor *pType)
{
    int n;

    n = SelectAliasDirEntry(pszName, pModule);
    if (n >= 0) {
        pType->type = Type_alias;
        pType->sIndex = n;
        _Return (CLS_NoError);
    }

    n = SelectInterfaceDirEntry(pszName, pModule);
    if (n >= 0) {
        pType->type = Type_interface;
        pType->sIndex = n;
        _Return (CLS_NoError);
    }

    n = SelectEnumDirEntry(pszName, pModule);
    if (n >= 0) {
        pType->type = Type_enum;
        pType->sIndex = n;
        _Return (CLS_NoError);
    }

    n = SelectStructDirEntry(pszName, pModule);
    if (n >= 0) {
        pType->type = Type_struct;
        pType->sIndex = n;
        _Return (CLS_NoError);
    }

    _ReturnError (CLSError_NotFound);
}

BOOL IsLocalStruct(const CLSModule *pModule, StructDescriptor *pDesc)
{
    unsigned short i;
    TypeDescriptor orgtype, basetype, *pt = NULL;

    for (i = 0; i < pDesc->cElems; i++) {
        pt = &(pDesc->ppElems[i]->type);
Loop:
        if (pt->type == Type_Array) {
            memset(&basetype, 0, sizeof(TypeDescriptor));
            GetArrayBaseType(pModule, pt, &basetype);
            pt = &basetype;
        }
        if (pt->type == Type_alias) {
            memset(&orgtype, 0, sizeof(TypeDescriptor));
            GetOriginalType(pModule, pt, &orgtype);
            pt = &orgtype;
            goto Loop;
        }

        if (pt->nPointer) return TRUE;

        if (!STRUCT_IS_PRIMARY_TYPE(pt->type)) {
            if (pt->type == Type_struct) {
                if (IsLocalStruct(pModule, pModule->ppStructDir[pt->sIndex]->pDesc)) {
                    return TRUE;
                }
                else continue;
            }
            return TRUE;
        }
    }

    return FALSE;
}

BOOL IsLocalCarQuintet(const CLSModule *pModule,
    const TypeDescriptor *pType, DWORD dwAttribs)
{
    switch (pType->type) {
        case Type_Int8:
        case Type_UInt16:
        case Type_UInt32:
        case Type_UInt64:
        case Type_PVoid:
            _ReturnOK (TRUE);

        case Type_interface:
            if (pModule->ppInterfaceDir[pType->sIndex]->pDesc->dwAttribs
                    & InterfaceAttrib_local) _ReturnOK (TRUE);
            break;

        default:
            if (1 <= pType->nPointer) _ReturnOK (TRUE);
            break;
    }

    _ReturnOK (FALSE);
}

static void GetTypeFromIndex(const CLSModule *pModule,
    StructDescriptor *pDesc, unsigned short index, TypeDescriptor *pTypeDesc)
{
    TypeDescriptor basetype, orgtype, *pt = NULL;
    pt = &(pDesc->ppElems[index]->type);
    if (pt->type == Type_Array) {
        memset(&basetype, 0, sizeof(TypeDescriptor));
        GetArrayBaseType(pModule, pt, &basetype);
        pt = &basetype;
    }
    if (pt->type == Type_alias) {
        memset(&orgtype, 0, sizeof(TypeDescriptor));
        GetOriginalType(pModule, pt, &orgtype);
        pt = &orgtype;
    }

    memcpy(pTypeDesc, pt, sizeof(TypeDescriptor));
}

static UINT GetMaxAlignmentOfStructElems(const CLSModule *pModule, StructDescriptor *pDesc)
{
    UINT uMaxAlign = 0;
    unsigned short i;
    UINT uAlign;
    TypeDescriptor typeDesc;

    for (i = 0; i < pDesc->cElems; i++) {
        GetTypeFromIndex(pModule, pDesc, i, &typeDesc);

        if (Type_struct == typeDesc.type) {
            uAlign = GetMaxAlignmentOfStructElems(pModule, pModule->ppStructDir[typeDesc.sIndex]->pDesc);
        }
        else {
            uAlign = STRUCT_TYPE_ALIGNMENT(typeDesc.type);
        }

        if (0 == uAlign) return 0;

        if (uAlign > uMaxAlign) uMaxAlign = uAlign;
    }
    return uMaxAlign;
}

static UINT CalcArrayElements(const CLSModule *pModule, TypeDescriptor *pDesc)
{
    assert(Type_Array == pDesc->type);
    UINT nElements;
    TypeDescriptor *pSubDesc;

    pSubDesc = &pModule->ppArrayDir[pDesc->sIndex]->type;
    nElements = pModule->ppArrayDir[pDesc->sIndex]->nElements;

    while (Type_Array == pSubDesc->type) {
        nElements *= pModule->ppArrayDir[pSubDesc->sIndex]->nElements;
        pSubDesc = &pModule->ppArrayDir[pSubDesc->sIndex]->type;
    }

    return nElements;
}

static UINT SizeOfStruct(const CLSModule *pModule, StructDescriptor *pDesc, UINT *pAlignment)
{
    UINT uAlignment = GetMaxAlignmentOfStructElems(pModule, pDesc);
    if (uAlignment > c_nStructMaxAlignSize) uAlignment = c_nStructMaxAlignSize;
    if (pAlignment) *pAlignment = uAlignment;
    if (pDesc->nAlignSize) return pDesc->nAlignSize;

    UINT uSizeOfStruct = 0, uTryAlignSize = 0;
    UINT uNextAlignment = 0;
    UINT uCurSize = 0, uNextSize = 0;
    unsigned short i = 0;
    TypeDescriptor orgTypeDesc;

    //the size of the first element of the current struct
    GetTypeFromIndex(pModule, pDesc, 0, &orgTypeDesc);
    if (Type_struct == orgTypeDesc.type) {
        uCurSize = SizeOfStruct(pModule, \
                    pModule->ppStructDir[orgTypeDesc.sIndex]->pDesc, NULL);
    }
    else {
        uCurSize = STRUCT_TYPE_TO_SIZE(orgTypeDesc.type);
    }

    if (Type_Array == pDesc->ppElems[0]->type.type) {
        uCurSize *= CalcArrayElements(pModule, &pDesc->ppElems[0]->type);
    }

    while (i < pDesc->cElems) {
        if (uCurSize && ((uCurSize % uAlignment) == 0)) {
            if (uTryAlignSize) {
                uSizeOfStruct += STRUCT_ALIGN_SIZE(uTryAlignSize, uAlignment);
                uTryAlignSize = 0;
            }
            uSizeOfStruct += uCurSize;
            i++;
            if (i < pDesc->cElems) {
                GetTypeFromIndex(pModule, pDesc, i, &orgTypeDesc);
                if (Type_struct == orgTypeDesc.type) {
                    uCurSize = SizeOfStruct(pModule, \
                                pModule->ppStructDir[orgTypeDesc.sIndex]->pDesc, NULL);
                }
                else {
                    uCurSize = STRUCT_TYPE_TO_SIZE(orgTypeDesc.type);
                }

                if (Type_Array == pDesc->ppElems[i]->type.type) {
                    uCurSize *= CalcArrayElements(pModule, &pDesc->ppElems[i]->type);
                }
            }
            continue;
        }

        if ((i + 1) >= pDesc->cElems) {
            if (uTryAlignSize && ((uTryAlignSize % uAlignment) == 0)) {
                uSizeOfStruct += uTryAlignSize;
                uTryAlignSize = 0;
            }
            uTryAlignSize += uCurSize;
            uSizeOfStruct += STRUCT_ALIGN_SIZE(uTryAlignSize, uAlignment);
            return uSizeOfStruct;
        }

        //Get the next element size
        GetTypeFromIndex(pModule, pDesc, i + 1, &orgTypeDesc);
        if (Type_struct == orgTypeDesc.type) {
            uNextSize = SizeOfStruct(pModule, pModule->ppStructDir[orgTypeDesc.sIndex]->pDesc, &uNextAlignment);
        }
        else {
            uNextSize = STRUCT_TYPE_TO_SIZE(orgTypeDesc.type);
            uNextAlignment = STRUCT_TYPE_ALIGNMENT(orgTypeDesc.type);
        }

        if (Type_Array == pDesc->ppElems[i + 1]->type.type) {
            uNextSize *= CalcArrayElements(pModule, &pDesc->ppElems[i+1]->type);
        }

        //align size of the next alignment
        uTryAlignSize += uCurSize;
        uTryAlignSize = STRUCT_ALIGN_SIZE(uTryAlignSize, uNextAlignment);
        if (uTryAlignSize && ((uTryAlignSize % uAlignment) == 0)) {
            uSizeOfStruct += uTryAlignSize;
            uTryAlignSize = 0;
        }

        uCurSize = uNextSize;
        i++;
    } //end while

    return uSizeOfStruct;
}

void CalcStructAlignedSize(const CLSModule *pModule, StructDescriptor *pDesc)
{
    if ((pDesc->nAlignSize != 0) || IsLocalStruct(pModule, pDesc)) return;
    pDesc->nAlignSize = SizeOfStruct(pModule, pDesc, NULL);
}

#define NAMESPACE_COPY(pSrcCLS, pSrc, pDest, bNameSpace)                \
    if (pSrc->pszNameSpace) {                                           \
        pDest->pszNameSpace = new char[strlen(pSrc->pszNameSpace) + 1]; \
        if (!pDest->pszNameSpace) _ReturnError (CLSError_OutOfMemory);  \
        strcpy(pDest->pszNameSpace, pSrc->pszNameSpace);                \
    }                                                                   \
    else if (bNameSpace) {                                              \
        pDest->pszNameSpace = new char[strlen(pSrcCLS->pszName) + 1];   \
        if (!pDest->pszNameSpace) _ReturnError (CLSError_OutOfMemory);  \
        strcpy(pDest->pszNameSpace, pSrcModule->pszName);               \
    }

int ClassDescriptorCopy(
    const CLSModule *pSrcModule,
    const ClassDescriptor *pSrc,
    CLSModule *pDestModule,
    ClassDescriptor *pDest,
    BOOL bNameSpace)
{
    int n, m, i;

    memcpy(&pDest->clsid, &pSrc->clsid, sizeof(CLSID));
    pDest->dwAttribs = pSrc->dwAttribs;

    if (pSrc->dwAttribs & ClassAttrib_hasparent) {
        m = ClassCopy(pSrcModule, pSrc->sParentIndex,
                    pDestModule, bNameSpace);
        if (m < 0) _Return (m);
        pDest->sParentIndex = m;
    }

    if (pSrc->dwAttribs & ClassAttrib_hasctor) {
        i = InterfaceCopy(pSrcModule,
                    pSrc->sCtorIndex, pDestModule, bNameSpace);
        if (i < 0) _Return (i);
        pDest->sCtorIndex = i;
    }

    for (n = 0; n < pSrc->cAggregates; n++) {
        m = ClassCopy(pSrcModule,
                    pSrc->pAggrIndexs[n], pDestModule, bNameSpace);
        if (m < 0) _Return (m);
        pDest->pAggrIndexs[pDest->cAggregates++] = m;
    }

    for (n = 0; n < pSrc->cAspects; n++) {
        m = ClassCopy(pSrcModule,
                    pSrc->pAspectIndexs[n], pDestModule, bNameSpace);
        if (m < 0) _Return (m);
        pDest->pAspectIndexs[pDest->cAspects++] = m;
    }

    for (n = 0; n < pSrc->cClasses; n++) {
        m = ClassCopy(pSrcModule,
                    pSrc->pClassIndexs[n], pDestModule, bNameSpace);
        if (m < 0) _Return (m);
        pDest->pClassIndexs[pDest->cClasses++] = m;
    }

    for (n = 0; n < pSrc->cInterfaces; n++) {
        i = InterfaceCopy(pSrcModule,
                    pSrc->ppInterfaces[n]->sIndex, pDestModule, bNameSpace);
        if (i < 0) _Return (i);

        m = CreateClassInterface(i, pDest);
        if (m < 0) _Return (m);
        assert(m == n);
        pDest->ppInterfaces[m]->wAttribs = pSrc->ppInterfaces[n]->wAttribs;
    }

    if (pSrc->dwAttribs & ClassAttrib_hascallback) {
        for (n = 0; n < pSrc->cInterfaces; n++) {
            if (pSrc->ppInterfaces[n]->wAttribs &
                                                ClassInterfaceAttrib_callback) {
                char szName[255];
                strcpy(szName,
                    pSrcModule->ppInterfaceDir[pSrc->ppInterfaces[n]->sIndex]->
                                                                    pszName);
                strcat(szName, "Handler");
                i = SelectInterfaceDirEntry(szName, pSrcModule);
                if (i < 0) _Return (i);

                i = InterfaceCopy(pSrcModule,
                            i, pDestModule, bNameSpace);
                if (i < 0) _Return (i);
            }
        }
    }

    _ReturnOK (CLS_NoError);
}

int ClassDescriptorXCopy(
    const CLSModule *pSrcModule,
    const ClassDescriptor *pSrc,
    CLSModule *pDestModule,
    ClassDescriptor *pDest,
    BOOL bNameSpace)
{
    int n, m;

    memcpy(&pDest->clsid, &pSrc->clsid, sizeof(CLSID));
    pDest->dwAttribs = pSrc->dwAttribs;

    if (pSrc->dwAttribs & ClassAttrib_hasparent) {
        pDest->sParentIndex = pSrc->sParentIndex;
    }

    if (pSrc->dwAttribs & ClassAttrib_hasctor) {
        pDest->sCtorIndex = pSrc->sCtorIndex;
    }

    for (n = 0; n < pSrc->cAggregates; n++) {
        pDest->pAggrIndexs[pDest->cAggregates++] = pSrc->pAggrIndexs[n];
    }

    for (n = 0; n < pSrc->cAspects; n++) {
        pDest->pAspectIndexs[pDest->cAspects++] = pSrc->pAspectIndexs[n];
    }

    for (n = 0; n < pSrc->cClasses; n++) {
        pDest->pClassIndexs[pDest->cClasses++] = pSrc->pClassIndexs[n];
    }

    for (n = 0; n < pSrc->cInterfaces; n++) {
        m = CreateClassInterface(pSrc->ppInterfaces[n]->sIndex, pDest);
        if (m < 0) _Return (m);
        assert(m == n);
        pDest->ppInterfaces[m]->wAttribs = pSrc->ppInterfaces[n]->wAttribs;
    }

    pDest->sParentIndex = pSrc->sParentIndex;

    _ReturnOK (CLS_NoError);
}

int ClassCopy(
    const CLSModule *pSrcModule,
    int nIndex,
    CLSModule *pDestModule,
    BOOL bNameSpace)
{
    int n, r;
    ClassDirEntry *pSrc, *pDest;
    char szClassObjName[64];

    pSrc = pSrcModule->ppClassDir[nIndex];
    n = SelectClassDirEntry(pSrc->pszName, pDestModule);
    if (n >= 0) {
        if (pDestModule->ppClassDir[n]->pDesc->cInterfaces > 0)
            _Return (n);
    }
    else {
        n = CreateClassDirEntry(pSrc->pszName,
                        pDestModule, pSrc->pDesc->dwAttribs);
        if (n < 0) _Return (n);
    }
    pDest = pDestModule->ppClassDir[n];

    NAMESPACE_COPY(pSrcModule, pSrc, pDest, bNameSpace);

    r = ClassDescriptorCopy(pSrcModule, pSrc->pDesc,
                    pDestModule, pDest->pDesc, bNameSpace);
    if (r < 0) _Return (r);

    if ((pDest->pDesc->dwAttribs & ClassAttrib_hasctor) &&
        !(pDest->pDesc->dwAttribs & ClassAttrib_t_clsobj)) {
        sprintf(szClassObjName, "_%sClassObject_", pDest->pszName);
        r = SelectClassDirEntry(szClassObjName, pSrcModule);
        if (r >= 0) {
            r = ClassCopy(pSrcModule, r, pDestModule, bNameSpace);
            if (r < 0) _Return (r);
        }
    }
    _Return (n);
}

int ClassXCopy(
    const CLSModule *pSrcModule,
    int nIndex,
    CLSModule *pDestModule,
    BOOL bNameSpace)
{
    int n, r;
    ClassDirEntry *pSrc, *pDest;

    pSrc = pSrcModule->ppClassDir[nIndex];
    n = SelectClassDirEntry(pSrc->pszName, pDestModule);
    if (n >= 0) {
        if (pDestModule->ppClassDir[n]->pDesc->cInterfaces > 0)
            _Return (n);
    }
    else {
        n = CreateClassDirEntry(pSrc->pszName,
                        pDestModule, pSrc->pDesc->dwAttribs);
        if (n < 0) _Return (n);
    }
    pDest = pDestModule->ppClassDir[n];

    NAMESPACE_COPY(pSrcModule, pSrc, pDest, bNameSpace);

    r = ClassDescriptorXCopy(pSrcModule, pSrc->pDesc,
                    pDestModule, pDest->pDesc, bNameSpace);
    if (r < 0) _Return (r);

    _Return (n);
}

int MethodCopy(
    const CLSModule *pSrcModule,
    const MethodDescriptor *pSrc,
    CLSModule *pDestModule,
    MethodDescriptor *pDest,
    BOOL bNameSpace)
{
    int n, m;

    n = TypeCopy(pSrcModule, &pSrc->type,
                pDestModule, &pDest->type, bNameSpace);
    if (n < 0) _Return (n);

    for (n = 0; n < pSrc->cParams; n++) {
        m = CreateMethodParam(pSrc->ppParams[n]->pszName, pDest);
        if (m < 0) _Return (m);
        assert(m == n);
        pDest->ppParams[m]->dwAttribs = pSrc->ppParams[n]->dwAttribs;
        m = TypeCopy(pSrcModule, &pSrc->ppParams[n]->type,
                    pDestModule, &pDest->ppParams[m]->type, bNameSpace);
        if (m < 0) _Return (m);
    }

    pDest->dwAttribs = pSrc->dwAttribs;

    _Return (CLS_NoError);
}

int MethodXCopy(
    const CLSModule *pSrcModule,
    const MethodDescriptor *pSrc,
    CLSModule *pDestModule,
    MethodDescriptor *pDest,
    BOOL bNameSpace)
{
    int n, m;

    n = TypeXCopy(pSrcModule, &pSrc->type,
                pDestModule, &pDest->type, bNameSpace);
    if (n < 0) _Return (n);

    for (n = 0; n < pSrc->cParams; n++) {
        m = CreateMethodParam(pSrc->ppParams[n]->pszName, pDest);
        if (m < 0) _Return (m);
        assert(m == n);
        pDest->ppParams[m]->dwAttribs = pSrc->ppParams[n]->dwAttribs;
        m = TypeXCopy(pSrcModule, &pSrc->ppParams[n]->type,
                    pDestModule, &pDest->ppParams[m]->type, bNameSpace);
        if (m < 0) _Return (m);
    }

    pDest->dwAttribs = pSrc->dwAttribs;

    _Return (CLS_NoError);
}

int InterfaceDescriptorCopy(
    const CLSModule *pSrcModule,
    const InterfaceDescriptor *pSrc,
    CLSModule *pDestModule,
    InterfaceDescriptor *pDest,
    BOOL bNameSpace)
{
    int n, m;

    pDest->dwAttribs = pSrc->dwAttribs;
    memcpy(&pDest->iid, &pSrc->iid, sizeof(IID));

    for (n = 0; n < pSrc->cMethods; n++) {
        m = CreateInterfaceMethod(pSrc->ppMethods[n]->pszName, pDest);
        if (m < 0) _Return (m);
        assert(m == n);
        m = MethodCopy(pSrcModule, pSrc->ppMethods[n],
                    pDestModule, pDest->ppMethods[m], bNameSpace);
        if (m < 0) _Return (m);
    }

    _ReturnOK (CLS_NoError);
}

int InterfaceDescriptorXCopy(
    const CLSModule *pSrcModule,
    const InterfaceDescriptor *pSrc,
    CLSModule *pDestModule,
    InterfaceDescriptor *pDest,
    BOOL bNameSpace)
{
    int n, m;

    pDest->dwAttribs = pSrc->dwAttribs;
    memcpy(&pDest->iid, &pSrc->iid, sizeof(IID));

    for (n = 0; n < pSrc->cMethods; n++) {
        m = CreateInterfaceMethod(pSrc->ppMethods[n]->pszName, pDest);
        if (m < 0) _Return (m);
        assert(m == n);
        m = MethodXCopy(pSrcModule, pSrc->ppMethods[n],
                    pDestModule, pDest->ppMethods[m], bNameSpace);
        if (m < 0) _Return (m);
    }

    _ReturnOK (CLS_NoError);
}

int InterfaceCopy(
    const CLSModule *pSrcModule,
    int nIndex,
    CLSModule *pDestModule,
    BOOL bNameSpace)
{
    int n, r, nParent;
    InterfaceDirEntry *pSrc, *pDest;

    pSrc = pSrcModule->ppInterfaceDir[nIndex];

    if (0 != pSrc->pDesc->sParentIndex) {
        nParent = InterfaceCopy(pSrcModule, pSrc->pDesc->sParentIndex,
                            pDestModule, bNameSpace);
        if (nParent < 0) _Return (nParent);
    }
    else {
        nParent = 0;
    }

    n = SelectInterfaceDirEntry(pSrc->pszName, pDestModule);
    if (n >= 0) {
        if (pDestModule->ppInterfaceDir[n]->pDesc->cMethods > 0)
            _Return (n);
    }
    else {
        n = CreateInterfaceDirEntry(
                pSrc->pszName, pDestModule, pSrc->pDesc->dwAttribs);
        if (n < 0) _Return (n);
        if (!bNameSpace) {
            pDestModule->pDefinedInterfaceIndex[pDestModule->cDefinedInterfaces++] = n;
        }
    }
    pDest = pDestModule->ppInterfaceDir[n];
    pDest->pDesc->sParentIndex = nParent;

    NAMESPACE_COPY(pSrcModule, pSrc, pDest, bNameSpace);

    r = InterfaceDescriptorCopy(pSrcModule, pSrc->pDesc,
                    pDestModule, pDest->pDesc, bNameSpace);
    if (r < 0) _Return (r);

    _Return (n);
}

int InterfaceXCopy(
    const CLSModule *pSrcModule,
    int nIndex,
    CLSModule *pDestModule,
    BOOL bNameSpace)
{
    int n, r, nParent;
    InterfaceDirEntry *pSrc, *pDest;

    pSrc = pSrcModule->ppInterfaceDir[nIndex];

    nParent = pSrc->pDesc->sParentIndex;

    n = SelectInterfaceDirEntry(pSrc->pszName, pDestModule);
    if (n >= 0) {
        if (pDestModule->ppInterfaceDir[n]->pDesc->cMethods > 0)
            _Return (n);
    }
    else {
        n = CreateInterfaceDirEntry(
                pSrc->pszName, pDestModule, pSrc->pDesc->dwAttribs);
        if (n < 0) _Return (n);
    }
    pDest = pDestModule->ppInterfaceDir[n];
    pDest->pDesc->sParentIndex = nParent;

    NAMESPACE_COPY(pSrcModule, pSrc, pDest, bNameSpace);

    r = InterfaceDescriptorXCopy(pSrcModule, pSrc->pDesc,
                    pDestModule, pDest->pDesc, bNameSpace);
    if (r < 0) _Return (r);

    _Return (n);
}

int ArrayCopy(
    const CLSModule *pSrcModule,
    int nIndex,
    CLSModule *pDestModule,
    BOOL bNameSpace)
{
    int n, m;
    ArrayDirEntry *pSrc, *pDest;

    pSrc = pSrcModule->ppArrayDir[nIndex];
    n = SelectArrayDirEntry(pSrc->nElements, pSrc->type, pDestModule);
    if (n >= 0) {
        _Return (n);
    }
    else {
        n = CreateArrayDirEntry(pDestModule);
        if (n < 0) _Return (n);
    }

    pDest = pDestModule->ppArrayDir[n];

    pDest->nElements = pSrc->nElements;
    m = TypeCopy(pSrcModule, &pSrc->type,
            pDestModule, &pDest->type, bNameSpace);
    if (m < 0) _Return (m);

    NAMESPACE_COPY(pSrcModule, pSrc, pDest, bNameSpace);

    _Return (n);
}

int ArrayXCopy(
    const CLSModule *pSrcModule,
    int nIndex,
    CLSModule *pDestModule,
    BOOL bNameSpace)
{
    int n, m;
    ArrayDirEntry *pSrc, *pDest;

    pSrc = pSrcModule->ppArrayDir[nIndex];
    n = SelectArrayDirEntry(pSrc->nElements, pSrc->type, pDestModule);
    if (n >= 0) {
        _Return (n);
    }
    else {
        n = CreateArrayDirEntry(pDestModule);
        if (n < 0) _Return (n);
    }

    pDest = pDestModule->ppArrayDir[n];

    pDest->nElements = pSrc->nElements;
    m = TypeXCopy(pSrcModule, &pSrc->type,
            pDestModule, &pDest->type, bNameSpace);
    if (m < 0) _Return (m);

    NAMESPACE_COPY(pSrcModule, pSrc, pDest, bNameSpace);

    _Return (n);
}

int StructDescriptorCopy(
    const CLSModule *pSrcModule,
    const StructDescriptor *pSrc,
    CLSModule *pDestModule,
    StructDescriptor *pDest,
    BOOL bNameSpace)
{
    int n, m;

    for (n = 0; n < pSrc->cElems; n++) {
        m = CreateStructElement(pSrc->ppElems[n]->pszName, pDest);
        if (m < 0) _Return (m);
        assert(m == n);
        m = TypeCopy(pSrcModule, &pSrc->ppElems[n]->type,
                    pDestModule, &pDest->ppElems[m]->type, bNameSpace);
        if (m < 0) _Return (m);
    }
    pDest->nAlignSize = pSrc->nAlignSize;

    _ReturnOK (CLS_NoError);
}

int StructDescriptorXCopy(
    const CLSModule *pSrcModule,
    const StructDescriptor *pSrc,
    CLSModule *pDestModule,
    StructDescriptor *pDest,
    BOOL bNameSpace)
{
    int n, m;

    for (n = 0; n < pSrc->cElems; n++) {
        m = CreateStructElement(pSrc->ppElems[n]->pszName, pDest);
        if (m < 0) _Return (m);
        assert(m == n);
        m = TypeXCopy(pSrcModule, &pSrc->ppElems[n]->type,
                    pDestModule, &pDest->ppElems[m]->type, bNameSpace);
        if (m < 0) _Return (m);
    }
    pDest->nAlignSize = pSrc->nAlignSize;

    _ReturnOK (CLS_NoError);
}

int StructCopy(
    const CLSModule *pSrcModule,
    int nIndex,
    CLSModule *pDestModule,
    BOOL bNameSpace)
{
    int n, r;
    StructDirEntry *pSrc, *pDest;

    pSrc = pSrcModule->ppStructDir[nIndex];
    n = SelectStructDirEntry(pSrc->pszName, pDestModule);
    if (n >= 0) {
        if (pDestModule->ppStructDir[n]->pDesc->cElems > 0)
            _Return (n);
    }
    else {
        n = CreateStructDirEntry(pSrc->pszName, pDestModule);
        if (n < 0) _Return (n);
    }
    pDest = pDestModule->ppStructDir[n];

    NAMESPACE_COPY(pSrcModule, pSrc, pDest, bNameSpace);

    r = StructDescriptorCopy(pSrcModule, pSrc->pDesc,
                    pDestModule, pDest->pDesc, bNameSpace);
    if (r < 0) _Return (r);

    _Return (n);
}

int StructXCopy(
    const CLSModule *pSrcModule,
    int nIndex,
    CLSModule *pDestModule,
    BOOL bNameSpace)
{
    int n, r;
    StructDirEntry *pSrc, *pDest;

    pSrc = pSrcModule->ppStructDir[nIndex];
    n = SelectStructDirEntry(pSrc->pszName, pDestModule);
    if (n >= 0) {
        if (pDestModule->ppStructDir[n]->pDesc->cElems > 0)
            _Return (n);
    }
    else {
        n = CreateStructDirEntry(pSrc->pszName, pDestModule);
        if (n < 0) _Return (n);
    }
    pDest = pDestModule->ppStructDir[n];

    NAMESPACE_COPY(pSrcModule, pSrc, pDest, bNameSpace);

    r = StructDescriptorXCopy(pSrcModule, pSrc->pDesc,
                    pDestModule, pDest->pDesc, bNameSpace);
    if (r < 0) _Return (r);

    _Return (n);
}

int EnumDescriptorCopy(
    const EnumDescriptor *pSrc,
    CLSModule *pDestModule,
    EnumDescriptor *pDest)
{
    int n, m;

    for (n = 0; n < pSrc->cElems; n++) {
        m = CreateEnumElement(pSrc->ppElems[n]->pszName, pDestModule, pDest);
        if (m < 0) _Return (m);
        assert(m == n);
        pDest->ppElems[m]->nValue = pSrc->ppElems[n]->nValue;
        pDest->ppElems[m]->bHexFormat = pSrc->ppElems[n]->bHexFormat;
    }
    _ReturnOK (CLS_NoError);
}

int EnumCopy(
    const CLSModule *pSrcModule,
    int nIndex,
    CLSModule *pDestModule,
    BOOL bNameSpace)
{
    int n, r;
    EnumDirEntry *pSrc, *pDest;

    pSrc = pSrcModule->ppEnumDir[nIndex];
    n = SelectEnumDirEntry(pSrc->pszName, pDestModule);
    if (n >= 0) {
        if (pDestModule->ppEnumDir[n]->pDesc->cElems > 0)
            _Return (n);
    }
    else {
        n = CreateEnumDirEntry(pSrc->pszName, pDestModule);
        if (n < 0) _Return (n);
    }
    pDest = pDestModule->ppEnumDir[n];

    NAMESPACE_COPY(pSrcModule, pSrc, pDest, bNameSpace);

    r = EnumDescriptorCopy(
                pSrc->pDesc, pDestModule, pDest->pDesc);
    if (r < 0) _Return (r);

    _Return (n);
}

int AliasCopy(
    const CLSModule *pSrcModule,
    int nIndex,
    CLSModule *pDestModule,
    BOOL bNameSpace)
{
    int n;
    TypeDescriptor orgType, destType;
    AliasDirEntry *pSrc, *pDest;

    pSrc = pSrcModule->ppAliasDir[nIndex];
    n = SelectAliasDirEntry(pSrc->pszName, pDestModule);
    if (n >= 0) _Return (n);

    GetOriginalType(pSrcModule, &pSrc->type, &orgType);
    n = TypeCopy(pSrcModule, &orgType, pDestModule, &destType, bNameSpace);
    if (n < 0) _Return (n);

    n = CreateAliasDirEntry(pSrc->pszName, pDestModule, &destType);
    if (n < 0) _Return (n);
    pDest = pDestModule->ppAliasDir[n];
    pDest->bDummyType = pSrc->bDummyType;

    NAMESPACE_COPY(pSrcModule, pSrc, pDest, bNameSpace);

    _Return (n);
}

int AliasXCopy(
    const CLSModule *pSrcModule,
    int nIndex,
    CLSModule *pDestModule,
    BOOL bNameSpace)
{
    int n;
    TypeDescriptor orgType, destType;
    AliasDirEntry *pSrc, *pDest;

    pSrc = pSrcModule->ppAliasDir[nIndex];
    n = SelectAliasDirEntry(pSrc->pszName, pDestModule);
    if (n >= 0) _Return (n);

    GetOriginalType(pSrcModule, &pSrc->type, &orgType);
    n = TypeXCopy(pSrcModule, &orgType, pDestModule, &destType, bNameSpace);
    if (n < 0) _Return (n);

    n = CreateAliasDirEntry(pSrc->pszName, pDestModule, &destType);
    if (n < 0) _Return (n);
    pDest = pDestModule->ppAliasDir[n];
    pDest->bDummyType = pSrc->bDummyType;

    NAMESPACE_COPY(pSrcModule, pSrc, pDest, bNameSpace);

    _Return (n);
}

int ConstCopy(
    const CLSModule *pSrcModule,
    int nIndex,
    CLSModule *pDestModule,
    BOOL bNameSpace)
{
    int n;
    ConstDirEntry *pSrc, *pDest;

    pSrc = pSrcModule->ppConstDir[nIndex];
    n = SelectConstDirEntry(pSrc->pszName, pDestModule);
    if (n >= 0) _Return (n);

    n = CreateConstDirEntry(pSrc->pszName, pDestModule);
    if (n < 0) _Return (n);
    pDest = pDestModule->ppConstDir[n];
    pDest->type = pSrc->type;
    if (pDest->type == INTEGER) {
        pDest->v.intValue.nValue = pSrc->v.intValue.nValue;
        pDest->v.intValue.bHexFormat = pSrc->v.intValue.bHexFormat;
    }
    else {
        assert(pDest->type == STRING);
        if (pSrc->v.strValue.pszValue != NULL) {
            pDest->v.strValue.pszValue = (char*)malloc(strlen(pSrc->v.strValue.pszValue) + 1);
            strcpy(pDest->v.strValue.pszValue, pSrc->v.strValue.pszValue);
        }
    }

    NAMESPACE_COPY(pSrcModule, pSrc, pDest, bNameSpace);

    _Return (n);
}

int TypeCopy(
    const CLSModule *pSrcModule,
    const TypeDescriptor *pSrcType,
    CLSModule *pDestModule,
    TypeDescriptor *pDestType,
    BOOL bNameSpace)
{
    int n;

    memcpy(pDestType, pSrcType, sizeof(TypeDescriptor));

    switch (pSrcType->type) {
        case Type_interface:
            n = InterfaceCopy(
                pSrcModule, pSrcType->sIndex, pDestModule, bNameSpace);
            if (n < 0) _Return (n);
            pDestType->sIndex = n;
            _Return (CLS_NoError);
        case Type_Array:
            n = ArrayCopy(pSrcModule, pSrcType->sIndex, pDestModule, bNameSpace);
            if (n < 0) _Return (n);
            pDestType->sIndex = n;
            _Return (CLS_NoError);

        case Type_struct:
            n = StructCopy(pSrcModule, pSrcType->sIndex, pDestModule, bNameSpace);
            if (n < 0) _Return (n);
            pDestType->sIndex = n;
            _Return (CLS_NoError);

        case Type_enum:
            n = EnumCopy(pSrcModule, pSrcType->sIndex, pDestModule, bNameSpace);
            if (n < 0) _Return (n);
            pDestType->sIndex = n;
            _Return (CLS_NoError);

        case Type_alias:
            n = AliasCopy(pSrcModule, pSrcType->sIndex, pDestModule, bNameSpace);
            if (n < 0) _Return (n);
            pDestType->sIndex = n;
            _Return (CLS_NoError);

        default:
            break;
    }

    if (pSrcType->pNestedType) {
        pDestType->pNestedType = new TypeDescriptor;
        if (!pDestType->pNestedType) _ReturnError (CLSError_OutOfMemory);
        n = TypeCopy(pSrcModule, pSrcType->pNestedType,
                    pDestModule, pDestType->pNestedType, bNameSpace);
        if (n < 0) {
            delete pDestType->pNestedType;
            pDestType->pNestedType = NULL;
            _Return (n);
        }
    }

    _ReturnOK (CLS_NoError);
}

int TypeXCopy(
    const CLSModule *pSrcModule,
    const TypeDescriptor *pSrcType,
    CLSModule *pDestModule,
    TypeDescriptor *pDestType,
    BOOL bNameSpace)
{
    int n;

    memcpy(pDestType, pSrcType, sizeof(TypeDescriptor));

    switch (pSrcType->type) {
        case Type_interface:
        case Type_Array:
        case Type_struct:
        case Type_enum:
        case Type_alias:
            pDestType->sIndex = pSrcType->sIndex;
            _Return (CLS_NoError);

        default:
            break;
    }

    if (pSrcType->pNestedType) {
        pDestType->pNestedType = new TypeDescriptor;
        if (!pDestType->pNestedType) _ReturnError (CLSError_OutOfMemory);
        n = TypeXCopy(pSrcModule, pSrcType->pNestedType,
                    pDestModule, pDestType->pNestedType, bNameSpace);
        if (n < 0) {
            delete pDestType->pNestedType;
            pDestType->pNestedType = NULL;
            _Return (n);
        }
    }

    _ReturnOK (CLS_NoError);
}

int TypeCopy(const TypeDescriptor *pSrc, TypeDescriptor *pDest)
{
    memcpy(pDest, pSrc, sizeof(TypeDescriptor));
    if (pSrc->pNestedType) {
        pDest->pNestedType = new TypeDescriptor;
        if (!pDest->pNestedType) _ReturnError (CLSError_OutOfMemory);
        memcpy(pDest->pNestedType, pSrc->pNestedType, sizeof(TypeDescriptor));
    }
    _ReturnOK (CLS_NoError);
}
