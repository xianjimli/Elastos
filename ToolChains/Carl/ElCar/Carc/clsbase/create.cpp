//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <clsbase.h>

int IsValidCLS(CLSModule *pModule, int nSize, const char *pszName)
{
    if (pModule->nCLSModuleVersion != c_nCLSModuleVersion) {
        ExtraMessage("CLSModule version is incompatable", pszName, "should be recompiled and relinked");
        _ReturnError(CLSError_CLSModuleVersion);
    }
    if (memcmp(pModule->szMagic, c_szMagic, c_nMagicSize)) {
        _ReturnError(CLSError_FormatMagic);
    }
    if (nSize < (int)sizeof(CLSModule) || pModule->nSize < nSize) {
        _ReturnError (CLSError_FormatSize);
    }
    _ReturnError (CLS_NoError);
}

ClassInterface *NewClassInterface(USHORT sIndex)
{
    ClassInterface *pClassInterface;

    pClassInterface = new ClassInterface;
    if (!pClassInterface) return NULL;
    memset(pClassInterface, 0, sizeof(ClassInterface));
    pClassInterface->sIndex = sIndex;

    return pClassInterface;
}

void DeleteClassInterface(ClassInterface *pClassInterface)
{
    assert(pClassInterface != NULL);

    delete pClassInterface;
}

ClassDirEntry *NewClassDirEntry(const char *pszName, const char *pszNamespace)
{
    ClassDirEntry *pClass;

    assert(pszName != NULL);

    pClass = new ClassDirEntry;
    if (!pClass) return NULL;
    memset(pClass, 0, sizeof(ClassDirEntry));

    pClass->pDesc = new ClassDescriptor;
    if (!pClass->pDesc) goto ErrorExit;
    memset(pClass->pDesc, 0, sizeof(ClassDescriptor));

    pClass->pDesc->ppInterfaces = \
        new ClassInterface *[c_nMaxClassInterfaces];
    if (!pClass->pDesc->ppInterfaces) goto ErrorExit;

    pClass->pDesc->pAggrIndexs = new USHORT[c_nMaxClassAspects];
    if (!pClass->pDesc->pAggrIndexs) goto ErrorExit;

    pClass->pDesc->pAspectIndexs = new USHORT[c_nMaxClassAspects];
    if (!pClass->pDesc->pAspectIndexs) goto ErrorExit;

    pClass->pDesc->pClassIndexs = new USHORT[c_nMaxAggrClassesOfAspect];
    if (!pClass->pDesc->pClassIndexs) goto ErrorExit;

    pClass->pszName = new char[strlen(pszName) + 1];
    if (!pClass->pszName) goto ErrorExit;
    strcpy(pClass->pszName, pszName);
    if (pszNamespace != NULL) {
        pClass->pszNameSpace = new char[strlen(pszNamespace) + 1];
        if (!pClass->pszNameSpace) goto ErrorExit;
        strcpy(pClass->pszNameSpace, pszNamespace);
    }

    return pClass;

ErrorExit:
    if (pClass->pDesc) {
        if (pClass->pDesc->pClassIndexs)
            delete [] pClass->pDesc->pClassIndexs;
        if (pClass->pDesc->pAspectIndexs)
            delete [] pClass->pDesc->pAspectIndexs;
        if (pClass->pDesc->pAggrIndexs)
            delete [] pClass->pDesc->pAggrIndexs;
        if (pClass->pDesc->ppInterfaces)
            delete [] pClass->pDesc->ppInterfaces;
        delete pClass->pDesc;
    }
    delete pClass;
    return NULL;
}

void DeleteClassDirEntry(ClassDirEntry *pClass)
{
    assert(pClass != NULL);
    assert(pClass->pDesc != NULL);
    assert(pClass->pDesc->ppInterfaces != NULL);
    assert(pClass->pDesc->pAggrIndexs != NULL);
    assert(pClass->pDesc->pAspectIndexs != NULL);

    for (int n = 0; n < pClass->pDesc->cInterfaces; n++) {
        DeleteClassInterface(pClass->pDesc->ppInterfaces[n]);
    }
    delete [] pClass->pDesc->ppInterfaces;
    delete [] pClass->pDesc->pAggrIndexs;
    delete [] pClass->pDesc->pAspectIndexs;
    delete pClass->pDesc;
    if (pClass->pszName) delete pClass->pszName;
    if (pClass->pszNameSpace) delete pClass->pszNameSpace;
    delete pClass;
}

ParamDescriptor *NewParam(const char *pszName)
{
    ParamDescriptor *pParam;

    assert(pszName != NULL);

    pParam = new ParamDescriptor;
    if (!pParam) return NULL;
    memset(pParam, 0, sizeof(ParamDescriptor));

    pParam->pszName = new char[strlen(pszName) + 1];
    if (!pParam->pszName) {
        delete pParam;
        return NULL;
    }
    strcpy(pParam->pszName, pszName);
    return pParam;
}

void DeleteParam(ParamDescriptor *pParam)
{
    assert(pParam != NULL);
    assert(pParam->pszName != NULL);

    if (pParam->type.pNestedType) delete pParam->type.pNestedType;
    delete pParam->pszName;
    delete pParam;
}

MethodDescriptor *NewMethod(const char *pszName)
{
    MethodDescriptor *pMethod;

    assert(pszName != NULL);

    pMethod = new MethodDescriptor;
    if (!pMethod) return NULL;
    memset(pMethod, 0, sizeof(MethodDescriptor));

    pMethod->ppParams = \
        new ParamDescriptor *[c_nMaxParams];
    if (!pMethod->ppParams) goto ErrorExit;

    pMethod->pszName = new char[strlen(pszName) + 1];
    if (!pMethod->pszName) goto ErrorExit;
    strcpy(pMethod->pszName, pszName);

    return pMethod;

ErrorExit:
    if (pMethod->ppParams)
        delete [] pMethod->ppParams;
    delete pMethod;
    return NULL;
}

void DeleteMethods(MethodDescriptor *pMethod)
{
    assert(pMethod != NULL);
    assert(pMethod->pszName != NULL);
    assert(pMethod->ppParams != NULL);

    for (int n = 0; n < pMethod->cParams; n++) {
        DeleteParam(pMethod->ppParams[n]);
    }
    delete [] pMethod->ppParams;
    delete pMethod->pszName;
    delete pMethod;
}

InterfaceDirEntry *NewInterfaceDirEntry(const char *pszName, const char *pszNamespace)
{
    assert(pszName != NULL);

    InterfaceDirEntry *pInterface;

    pInterface = new InterfaceDirEntry;
    if (!pInterface) return NULL;
    memset(pInterface, 0, sizeof(InterfaceDirEntry));

    pInterface->pDesc = new InterfaceDescriptor;
    if (!pInterface->pDesc) goto ErrorExit;
    memset(pInterface->pDesc, 0, sizeof(InterfaceDescriptor));

    pInterface->pDesc->ppMethods = \
        new MethodDescriptor *[c_nMaxMethods];
    if (!pInterface->pDesc->ppMethods) goto ErrorExit;

    pInterface->pszName = new char[strlen(pszName) + 1];
    if (!pInterface->pszName) goto ErrorExit;
    strcpy(pInterface->pszName, pszName);
    if (pszNamespace != NULL) {
        pInterface->pszNameSpace = new char[strlen(pszNamespace) + 1];
        if (!pInterface->pszNameSpace) goto ErrorExit;
        strcpy(pInterface->pszNameSpace, pszNamespace);
    }

    return pInterface;

ErrorExit:
    if (pInterface->pDesc) {
        if (pInterface->pDesc->ppMethods)
            delete [] pInterface->pDesc->ppMethods;
        delete pInterface->pDesc;
    }
    delete pInterface;
    return NULL;
}

void DeleteInterfaceDirEntry(InterfaceDirEntry *pInterface)
{
    assert(pInterface != NULL);
    assert(pInterface->pszName != NULL);
    assert(pInterface->pDesc != NULL);
    assert(pInterface->pDesc->ppMethods != NULL);

    for (int n = 0; n < pInterface->pDesc->cMethods; n++) {
        DeleteMethods(pInterface->pDesc->ppMethods[n]);
    }
    delete [] pInterface->pDesc->ppMethods;
    delete pInterface->pDesc;
    delete pInterface->pszName;
    if (pInterface->pszNameSpace) delete pInterface->pszNameSpace;
    delete pInterface;
}

StructElement *NewStructElement(const char *pszName)
{
    StructElement *pElement;

    assert(pszName != NULL);

    pElement = new StructElement;
    if (!pElement) return NULL;
    memset(pElement, 0, sizeof(StructElement));

    pElement->pszName = new char[strlen(pszName) + 1];
    if (!pElement->pszName) {
        delete pElement;
        return NULL;
    }
    strcpy(pElement->pszName, pszName);
    return pElement;
}

void DeleteStructElement(StructElement *pElement)
{
    assert(pElement != NULL);
    assert(pElement->pszName != NULL);

    if (pElement->type.pNestedType) delete pElement->type.pNestedType;
    delete pElement->pszName;
    delete pElement;
}

StructDirEntry *NewStructDirEntry(const char *pszName)
{
    StructDirEntry *pStruct;

    assert(pszName != NULL);

    pStruct = new StructDirEntry;
    if (!pStruct) return NULL;
    memset(pStruct, 0, sizeof(StructDirEntry));

    pStruct->pDesc = new StructDescriptor;
    if (!pStruct->pDesc) goto ErrorExit;
    memset(pStruct->pDesc, 0, sizeof(StructDescriptor));

    pStruct->pDesc->ppElems = new StructElement *[c_nMaxStructElements];
    if (!pStruct->pDesc->ppElems) goto ErrorExit;

    pStruct->pszName = new char[strlen(pszName) + 1];
    if (!pStruct->pszName) goto ErrorExit;
    strcpy(pStruct->pszName, pszName);

    return pStruct;

ErrorExit:
    if (pStruct->pDesc) {
        if (pStruct->pDesc->ppElems)
            delete [] pStruct->pDesc->ppElems;
        delete pStruct->pDesc;
    }
    delete pStruct;
    return NULL;
}

void DeleteStructDirEntry(StructDirEntry *pStruct)
{
    assert(pStruct != NULL);
    assert(pStruct->pszName != NULL);
    assert(pStruct->pDesc != NULL);
    assert(pStruct->pDesc->ppElems != NULL);

    for (int n = 0; n < pStruct->pDesc->cElems; n++) {
        DeleteStructElement(pStruct->pDesc->ppElems[n]);
    }
    delete [] pStruct->pDesc->ppElems;
    delete pStruct->pDesc;
    delete pStruct->pszName;
    if (pStruct->pszNameSpace) delete pStruct->pszNameSpace;
    delete pStruct;
}

ArrayDirEntry *NewArrayDirEntry()
{
    ArrayDirEntry *pArray = new ArrayDirEntry;
    if (NULL == pArray)
        return NULL;

    memset(pArray, 0, sizeof(ArrayDirEntry));

    return pArray;
}

void DeleteArrayDirEntry(ArrayDirEntry *pArray)
{
    assert(pArray);

    if (pArray->pszNameSpace) {
        delete pArray->pszNameSpace;
        pArray->pszNameSpace = NULL;
    }

    if (pArray->type.pNestedType) {
        delete pArray->type.pNestedType;
        pArray->type.pNestedType = NULL;
    }

    delete pArray;
}

ConstDirEntry *NewConstDirEntry(const char *pszName)
{
    assert(pszName != NULL);

    ConstDirEntry *pConst = new ConstDirEntry;
    if (NULL == pConst)
        return NULL;

    memset(pConst, 0, sizeof(ConstDirEntry));

    pConst->pszName = new char[strlen(pszName) + 1];
    if (!pConst->pszName) goto ErrorExit;
    strcpy(pConst->pszName, pszName);

    return pConst;

ErrorExit:
    delete pConst;
    return NULL;
}

void DeleteConstDirEntry(ConstDirEntry *pConst)
{
    assert(pConst != NULL);
    assert(pConst->pszName != NULL);

    if (pConst->pszNameSpace) {
        delete pConst->pszNameSpace;
        pConst->pszNameSpace = NULL;
    }

    delete pConst->pszName;
    if (pConst->type == STRING && pConst->v.strValue.pszValue != NULL) {
        free(pConst->v.strValue.pszValue);
    }
    delete pConst;
}

EnumElement *NewEnumElement(const char *pszName)
{
    EnumElement *pElement;

    assert(pszName != NULL);

    pElement = new EnumElement;
    if (!pElement) return NULL;
    memset(pElement, 0, sizeof(EnumElement));

    pElement->pszName = new char[strlen(pszName) + 1];
    if (!pElement->pszName) {
        delete pElement;
        return NULL;
    }
    strcpy(pElement->pszName, pszName);
    return pElement;
}

void DeleteEnumElement(EnumElement *pElement)
{
    assert(pElement != NULL);
    assert(pElement->pszName != NULL);

    delete pElement->pszName;
    delete pElement;
}

EnumDirEntry *NewEnumDirEntry(const char *pszName)
{
    EnumDirEntry *pEnum;

    assert(pszName != NULL);

    pEnum = new EnumDirEntry;
    if (!pEnum) return NULL;
    memset(pEnum, 0, sizeof(EnumDirEntry));

    pEnum->pDesc = new EnumDescriptor;
    if (!pEnum->pDesc) goto ErrorExit;
    memset(pEnum->pDesc, 0, sizeof(EnumDescriptor));

    pEnum->pDesc->ppElems = new EnumElement *[c_nMaxEnumElements];
    if (!pEnum->pDesc->ppElems) goto ErrorExit;

    pEnum->pszName = new char[strlen(pszName) + 1];
    if (!pEnum->pszName) goto ErrorExit;
    strcpy(pEnum->pszName, pszName);

    return pEnum;

ErrorExit:
    if (pEnum->pDesc) {
        if (pEnum->pDesc->ppElems)
            delete [] pEnum->pDesc->ppElems;
        delete pEnum->pDesc;
    }
    delete pEnum;
    return NULL;
}

void DeleteEnumDirEntry(EnumDirEntry *pEnum)
{
    assert(pEnum != NULL);
    assert(pEnum->pszName != NULL);
    assert(pEnum->pDesc != NULL);
    assert(pEnum->pDesc->ppElems != NULL);

    for (int n = 0; n < pEnum->pDesc->cElems; n++) {
        DeleteEnumElement(pEnum->pDesc->ppElems[n]);
    }
    delete [] pEnum->pDesc->ppElems;
    delete pEnum->pDesc;
    delete pEnum->pszName;
    if (pEnum->pszNameSpace) delete pEnum->pszNameSpace;
    delete pEnum;
}

AliasDirEntry *NewAliasDirEntry(const char *pszName)
{
    assert(pszName != NULL);

    AliasDirEntry *pAlias;

    pAlias = new AliasDirEntry;
    if (!pAlias) return NULL;
    memset(pAlias, 0, sizeof(AliasDirEntry));

    pAlias->pszName = new char[strlen(pszName) + 1];
    if (!pAlias->pszName) {
        delete pAlias;
        return NULL;
    }
    strcpy(pAlias->pszName, pszName);

    return pAlias;
}

void DeleteAliasDirEntry(AliasDirEntry *pAlias)
{
    assert(pAlias != NULL);
    assert(pAlias->pszName != NULL);

    if (pAlias->type.pNestedType) delete pAlias->type.pNestedType;
    delete pAlias->pszName;
    if (pAlias->pszNameSpace) delete pAlias->pszNameSpace;
    delete pAlias;
}

CLSModule *CreateCLS()
{
    CLSModule *pModule;

    pModule = new CLSModule;
    if (!pModule) return NULL;
    memset(pModule, '\0', sizeof(CLSModule));

    memcpy(pModule->szMagic, c_szMagic, c_nMagicSize);
    pModule->nCLSModuleVersion = c_nCLSModuleVersion;

    pModule->ppClassDir = new ClassDirEntry *[c_nMaxClassNumber];
    pModule->ppInterfaceDir = new InterfaceDirEntry *[c_nMaxInterfaceNumber];
    pModule->pDefinedInterfaceIndex = new int[c_nMaxDefinedInterfaceNumber];
    pModule->ppStructDir = new StructDirEntry *[c_nMaxStructNumber];
    pModule->ppEnumDir = new EnumDirEntry *[c_nMaxEnumNumber];
    pModule->ppAliasDir = new AliasDirEntry *[c_nMaxAliasNumber];
    pModule->ppLibNames = new char *[c_nMaxLibNumber];
    pModule->ppArrayDir = new ArrayDirEntry *[c_nMaxArrayNumber];
    pModule->ppConstDir = new ConstDirEntry *[c_nMaxConstNumber];

    if (!pModule->ppClassDir || !pModule->ppInterfaceDir ||
        !pModule->ppStructDir || !pModule->ppEnumDir ||
        !pModule->ppAliasDir || !pModule->ppLibNames ||
        !pModule->ppArrayDir || !pModule->ppConstDir ||
        !pModule->pDefinedInterfaceIndex) {
        DestroyCLS(pModule);
        return NULL;
    }

    return pModule;
}

void DestroyCLS(CLSModule *pModule)
{
    int n;

    assert(pModule != NULL);

    if (pModule->ppClassDir) {
        for (n = 0; n < pModule->cClasses; n++) {
            DeleteClassDirEntry(pModule->ppClassDir[n]);
        }
        delete [] pModule->ppClassDir;
    }

    if (pModule->ppInterfaceDir) {
        for (n = 0; n < pModule->cInterfaces; n++) {
            DeleteInterfaceDirEntry(pModule->ppInterfaceDir[n]);
        }
        delete [] pModule->ppInterfaceDir;
    }

    if (pModule->pDefinedInterfaceIndex) {
        delete [] pModule->pDefinedInterfaceIndex;
    }

    if (pModule->ppArrayDir) {
        for (n = 0; n < pModule->cArrays; n++) {
            DeleteArrayDirEntry(pModule->ppArrayDir[n]);
        }
        delete [] pModule->ppArrayDir;
    }

    if (pModule->ppStructDir) {
        for (n = 0; n < pModule->cStructs; n++) {
            DeleteStructDirEntry(pModule->ppStructDir[n]);
        }
        delete [] pModule->ppStructDir;
    }

    if (pModule->ppEnumDir) {
        for (n = 0; n < pModule->cEnums; n++) {
            DeleteEnumDirEntry(pModule->ppEnumDir[n]);
        }
        delete [] pModule->ppEnumDir;
    }

    if (pModule->ppAliasDir) {
        for (n = 0; n < pModule->cAliases; n++) {
            DeleteAliasDirEntry(pModule->ppAliasDir[n]);
        }
        delete [] pModule->ppAliasDir;
    }

    if (pModule->ppConstDir) {
        for (n = 0; n < pModule->cConsts; n++) {
            DeleteConstDirEntry(pModule->ppConstDir[n]);
        }
        delete [] pModule->ppConstDir;
    }

    if (pModule->ppLibNames) {
        for (n = 0; n < pModule->cLibraries; n++) {
            delete pModule->ppLibNames[n];
        }
        delete [] pModule->ppLibNames;
    }

    if (pModule->pszUunm) delete pModule->pszUunm;
    if (pModule->pszName) delete pModule->pszName;

    delete pModule;
}

int CreateClassDirEntry(
    const char *pszName, CLSModule *pModule, unsigned long dwAttribs)
{
    int n;
    ClassDirEntry *pClass;
    ClassDescriptor *pDesc;

    char *pszNamespace = NULL;
    const char *dot = strrchr(pszName, '.');
    if (dot != NULL) {
        pszNamespace = (char*)malloc(dot - pszName + 1);
        memset(pszNamespace, 0, dot - pszName + 1);
        memcpy(pszNamespace, pszName, dot - pszName);
        pszName = dot + 1;
    }
    n = SelectClassDirEntry(pszName, pszNamespace, pModule);
    if (n >= 0) {
        pDesc = pModule->ppClassDir[n]->pDesc;
        if (pszNamespace != NULL) free(pszNamespace);

        if (CLASS_TYPE(dwAttribs) != CLASS_TYPE(pDesc->dwAttribs)) {
            ExtraMessage(pModule->ppClassDir[n]->pszNameSpace,
                        "class", pszName);
            _ReturnError (CLSError_NameConflict);
        }
        if (pDesc->cInterfaces > 0
            || (pDesc->dwAttribs & ClassAttrib_hasparent) > 0
            || IsValidUUID(&pDesc->clsid)) {
            ExtraMessage(pModule->ppClassDir[n]->pszNameSpace,
                        "class", pszName);
            _ReturnError (CLSError_DupEntry);
        }
        _ReturnOK (n);
    }

    n = GlobalSelectSymbol(pszName, pszNamespace, pModule, GType_Class, NULL);
    if (n >= 0) {
        if (pszNamespace != NULL) free(pszNamespace);
        _ReturnError (CLSError_NameConflict);
    }
    if (pModule->cClasses >= c_nMaxClassNumber) {
        if (pszNamespace != NULL) free(pszNamespace);
        _ReturnError (CLSError_FullEntry);
    }

    pClass = NewClassDirEntry(pszName, pszNamespace);
    if (pszNamespace != NULL) free(pszNamespace);
    if (!pClass) _ReturnError (CLSError_OutOfMemory);
    pModule->ppClassDir[pModule->cClasses] = pClass;

    _ReturnOK (pModule->cClasses++);
}

int CreateInterfaceDirEntry(
    const char *pszName, CLSModule *pModule, unsigned long dwAttribs)
{
    int n;
    InterfaceDirEntry *pInterface;
    InterfaceDescriptor *pDesc;

    char *pszNamespace = NULL;
    const char *dot = strrchr(pszName, '.');
    if (dot != NULL) {
        pszNamespace = (char*)malloc(dot - pszName + 1);
        memset(pszNamespace, 0, dot - pszName + 1);
        memcpy(pszNamespace, pszName, dot - pszName);
        pszName = dot + 1;
    }
    n = SelectInterfaceDirEntry(pszName, pszNamespace, pModule);
    if (n >= 0) {
        pDesc = pModule->ppInterfaceDir[n]->pDesc;
        if (pszNamespace != NULL) free(pszNamespace);

        if (INTERFACE_TYPE(dwAttribs) != \
            INTERFACE_TYPE(pDesc->dwAttribs)) {
            ExtraMessage(pModule->ppInterfaceDir[n]->pszNameSpace,
                        "interface", pModule->ppInterfaceDir[n]->pszName);
            _ReturnError (CLSError_NameConflict);
        }
        if (pDesc->cMethods > 0 || pDesc->sParentIndex != 0
            || IsValidUUID(&pDesc->iid)) {
            ExtraMessage(pModule->ppInterfaceDir[n]->pszNameSpace,
                        "interface", pModule->ppInterfaceDir[n]->pszName);
            _ReturnError (CLSError_DupEntry);
        }
        _ReturnOK (n);
    }

    n = GlobalSelectSymbol(pszName, pszNamespace, pModule, GType_Interface, NULL);
    if (n >= 0) {
        if (pszNamespace != NULL) free(pszNamespace);
        _ReturnError (CLSError_NameConflict);
    }
    if (pModule->cInterfaces >= c_nMaxInterfaceNumber) {
        if (pszNamespace != NULL) free(pszNamespace);
        _ReturnError (CLSError_FullEntry);
    }

    pInterface = NewInterfaceDirEntry(pszName, pszNamespace);
    if (pszNamespace != NULL) free(pszNamespace);
    if (!pInterface) _ReturnError (CLSError_OutOfMemory);
    pModule->ppInterfaceDir[pModule->cInterfaces] = pInterface;

    _ReturnOK (pModule->cInterfaces++);
}

int CreateArrayDirEntry(CLSModule *pModule)
{
    ArrayDirEntry *pArray;

    pArray = NewArrayDirEntry();
    if (!pArray) _ReturnError (CLSError_OutOfMemory);

    pModule->ppArrayDir[pModule->cArrays] = pArray;

    _ReturnOK (pModule->cArrays++);
}

int CreateStructDirEntry(
    const char *pszName, CLSModule *pModule)
{
    int n;
    StructDirEntry *pStruct;

    n = SelectStructDirEntry(pszName, pModule);
    if (n >= 0) {
        if (pModule->ppStructDir[n]->pDesc->cElems > 0) {
            ExtraMessage(pModule->ppStructDir[n]->pszNameSpace,
                        "struct", pModule->ppStructDir[n]->pszName);
            _ReturnError (CLSError_DupEntry);
        }
        _ReturnOK (n);
    }

    n = GlobalSelectSymbol(pszName, NULL, pModule, GType_Struct, NULL);
    if (n >= 0) _ReturnError (CLSError_NameConflict);
    if (pModule->cStructs >= c_nMaxStructNumber)
        _ReturnError (CLSError_FullEntry);

    pStruct = NewStructDirEntry(pszName);
    if (!pStruct) _ReturnError (CLSError_OutOfMemory);
    pModule->ppStructDir[pModule->cStructs] = pStruct;

    _ReturnOK (pModule->cStructs++);
}

int CreateEnumDirEntry(
    const char *pszName, CLSModule *pModule)
{
    int n;
    EnumDirEntry *pEnum;

    n = SelectEnumDirEntry(pszName, pModule);
    if (n >= 0) {
        if (pModule->ppEnumDir[n]->pDesc->cElems > 0) {
            ExtraMessage(pModule->ppEnumDir[n]->pszNameSpace,
                        "enum", pModule->ppEnumDir[n]->pszName);
            _ReturnError (CLSError_DupEntry);
        }
        _ReturnOK (n);
    }

    n = GlobalSelectSymbol(pszName, NULL, pModule, GType_Enum, NULL);
    if (n >= 0) _ReturnError (CLSError_NameConflict);
    if (pModule->cEnums >= c_nMaxEnumNumber) _ReturnError (CLSError_FullEntry);

    pEnum = NewEnumDirEntry(pszName);
    if (!pEnum) _ReturnError (CLSError_OutOfMemory);
    pModule->ppEnumDir[pModule->cEnums] = pEnum;

    _ReturnOK (pModule->cEnums++);
}

int CreateConstDirEntry(
    const char *pszName, CLSModule *pModule)
{
    int n;
    ConstDirEntry *pConst;

    n = SelectConstDirEntry(pszName, pModule);
    if (n >= 0) {
        _ReturnError (CLSError_DupEntry);
    }

    n = GlobalSelectSymbol(pszName, NULL, pModule, GType_Const, NULL);
    if (n >= 0) _ReturnError (CLSError_NameConflict);
    if (pModule->cConsts >= c_nMaxConstNumber) _ReturnError (CLSError_FullEntry);

    pConst = NewConstDirEntry(pszName);
    if (!pConst) _ReturnError (CLSError_OutOfMemory);
    pModule->ppConstDir[pModule->cConsts] = pConst;

    _ReturnOK (pModule->cConsts++);
}

int CreateAliasDirEntry(
    const char *pszName, CLSModule *pModule, TypeDescriptor *pType)
{
    int n;
    AliasDirEntry *pAlias;
    GlobalSymbolType gType;
    TypeDescriptor type;

    n = GlobalSelectSymbol(pszName, NULL, pModule, GType_None, &gType);
    if (n >= 0) {
        if (gType == GType_Class) {
            _ReturnError (CLSError_NameConflict);
        }

        memset(&type, 0, sizeof(type));
        type.type = (CARDataType)gType;
        type.sIndex = n;
        if (!IsEqualType(pModule, &type, pType)) {
            _ReturnError (CLSError_NameConflict);
        }
    }
    if (pModule->cAliases >= c_nMaxAliasNumber)
        _ReturnError (CLSError_FullEntry);

    pAlias = NewAliasDirEntry(pszName);
    if (!pAlias) _ReturnError (CLSError_OutOfMemory);
    memcpy(&pAlias->type, pType, sizeof(TypeDescriptor));
    pModule->ppAliasDir[pModule->cAliases] = pAlias;

    _ReturnOK (pModule->cAliases++);
}

int CreateClassInterface(USHORT sIndex, ClassDescriptor *pDesc)
{
    int n;
    ClassInterface *pClassInterface;

    assert(pDesc != NULL);

    n = SelectClassInterface(sIndex, pDesc);
    if (n >= 0) _ReturnError (CLSError_DupEntry);

    if (pDesc->cInterfaces >= c_nMaxClassInterfaces)
        _ReturnError (CLSError_FullEntry);

    pClassInterface = NewClassInterface(sIndex);
    if (!pClassInterface) _ReturnError (CLSError_OutOfMemory);
    pDesc->ppInterfaces[pDesc->cInterfaces] = pClassInterface;

    _ReturnOK (pDesc->cInterfaces++);
}

int CreateInterfaceMethod(
    const char *pszName, InterfaceDescriptor *pInterface)
{
    int n;
    MethodDescriptor *pMethod;

    assert(pInterface != NULL);
    assert(pszName != NULL);

    n = SelectInterfaceMethod(pszName, pInterface);
    if (n >= 0) {
        ExtraMessage("interface", pszName);
        _ReturnError (CLSError_DupEntry);
    }

    if (pInterface->cMethods >= c_nMaxMethods)
        _ReturnError (CLSError_FullEntry);

    pMethod = NewMethod(pszName);
    if (!pMethod) _ReturnError (CLSError_OutOfMemory);
    pInterface->ppMethods[pInterface->cMethods] = pMethod;

    _ReturnOK (pInterface->cMethods++);
}

int CreateMethodParam(
    const char *pszName, MethodDescriptor *pMethod)
{
    int n;
    ParamDescriptor *pParam;

    assert(pMethod != NULL);
    assert(pszName != NULL);

    n = SelectMethodParam(pszName, pMethod);
    if (n >= 0) {
        ExtraMessage("method parameter", pszName);
        _ReturnError (CLSError_DupEntry);
    }

    if (pMethod->cParams >= c_nMaxParams) _ReturnError (CLSError_FullEntry);
    pParam = NewParam(pszName);
    if (!pParam) _ReturnError (CLSError_OutOfMemory);
    pMethod->ppParams[pMethod->cParams] = pParam;

    _ReturnOK (pMethod->cParams++);
}

int CreateStructElement(
    const char *pszName, StructDescriptor *pStruct)
{
    int n;
    StructElement *pElement;

    assert(pStruct != NULL);
    assert(pszName != NULL);

    n = SelectStructElement(pszName, pStruct);
    if (n >= 0) {
        ExtraMessage("struct member", pszName);
        _ReturnError (CLSError_DupEntry);
    }

    if (pStruct->cElems >= c_nMaxStructElements)
        _ReturnError (CLSError_FullEntry);

    pElement = NewStructElement(pszName);
    if (!pElement) _ReturnError (CLSError_OutOfMemory);
    pStruct->ppElems[pStruct->cElems] = pElement;

    _ReturnOK (pStruct->cElems++);
}

int CreateEnumElement(
    const char *pszName, CLSModule *pModule, EnumDescriptor *pEnum)
{
    int n;
    EnumElement *pElement;

    assert(pModule != NULL);
    assert(pEnum != NULL);
    assert(pszName != NULL);

    n = SelectEnumElement(pszName, pEnum);
    if (n >= 0) {
        ExtraMessage("enum member", pszName);
        _ReturnError (CLSError_DupEntry);
    }

    pElement = GlobalSelectEnumElement(pszName, pModule);
    if (pElement) _ReturnError (CLSError_NameConflict);

    if (pEnum->cElems >= c_nMaxEnumElements)
        _ReturnError (CLSError_FullEntry);

    pElement = NewEnumElement(pszName);
    if (!pElement) _ReturnError (CLSError_OutOfMemory);
    pEnum->ppElems[pEnum->cElems] = pElement;

    _ReturnOK (pEnum->cElems++);
}

int MethodAppend(
    const MethodDescriptor *pSrc, InterfaceDescriptor *pDesc)
{
    int n, m;
    MethodDescriptor *pDest;
    ParamDescriptor *pParam;

    n = CreateInterfaceMethod(pSrc->pszName, pDesc);
    if (n < 0) _Return (n);

    pDest = pDesc->ppMethods[n];
    TypeCopy(&pSrc->type, &pDest->type);

    for (n = 0; n < pSrc->cParams; n++) {
        m = CreateMethodParam(pSrc->ppParams[n]->pszName, pDest);
        if (m < 0) _Return (m);
        pParam = pDest->ppParams[m];
        pParam->dwAttribs = pSrc->ppParams[n]->dwAttribs;
        TypeCopy(&pSrc->ppParams[n]->type, &pParam->type);
    }

    _ReturnOK (CLS_NoError);
}

int InterfaceMethodsAppend(const CLSModule *pModule,
    const InterfaceDescriptor *pSrc, InterfaceDescriptor *pDest)
{
    int n, m;

    if (0 != pSrc->sParentIndex) {
        n = InterfaceMethodsAppend(pModule,
                pModule->ppInterfaceDir[pSrc->sParentIndex]->pDesc,
                pDest);
        if (n < 0) _Return (n);
    }

    for (n = 0; n < pSrc->cMethods; n++) {
        m = MethodAppend(pSrc->ppMethods[n], pDest);
        if (m < 0) _Return (m);
    }

    _ReturnOK (CLS_NoError);
}
