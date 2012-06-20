//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <lube.h>

int LubeContext::ModuleMember(MemberType member, char *pszBuffer)
{
    const char *pszOutput;

    switch (member) {
        case Member_Type:
            if (m_pModule->dwAttribs & CARAttrib_library) {
                pszOutput = "library";
            }
            else {
                pszOutput = "module";
            }
            break;
        case Member_Name:
            pszOutput = m_pModule->pszName;
            break;
        case Member_Attrib:
            sprintf(pszBuffer, "%08x", m_pModule->dwAttribs);
            return LUBE_OK;
        case Member_Version:
            sprintf(pszBuffer, "%d.%d", m_pModule->cMajorVersion, m_pModule->cMinorVersion);
            return LUBE_OK;
        case Member_Major_Version:
            sprintf(pszBuffer, "%d", m_pModule->cMajorVersion);
            return LUBE_OK;
        case Member_Minor_Version:
            sprintf(pszBuffer, "%d", m_pModule->cMinorVersion);
            return LUBE_OK;
        case Member_CarCode:
            sprintf(pszBuffer, "%08x", m_pModule->nChecksum);
            return LUBE_OK;
        case Member_BuildDate:
            sprintf(pszBuffer, "%08x", m_pModule->nBarcode);
            return LUBE_OK;
        case Member_Uuid:
            pszOutput = Uuid2CString(&m_pModule->uuid, TRUE);
            break;
        case Member_Uunm:
            if (m_pModule->pszUunm) {
                sprintf(pszBuffer, "\"%s\"",
                    m_pModule->pszUunm);
                return LUBE_OK;
            }
            pszOutput = "(null uunm)";
            break;
        default:
            assert(TRUE == FALSE);
            return LUBE_FAIL;
    }
    strcpy(pszBuffer, pszOutput);
    return LUBE_OK;
}

int LubeContext::LibraryMember(MemberType member, char *pszBuffer)
{
    char *p;

    assert(NULL != m_pLibrary);

    p = m_pLibrary + strlen(m_pLibrary);
    while (p != m_pLibrary && !IS_PATH_SEPARATOR(*(p - 1)))
        p--;

    if (Member_Name == member) {
        while (*p && '.' != *p) *pszBuffer++= *p++;
        *pszBuffer = 0;
    }
    else if (Member_Type == member) {
        while (*p && '.' != *p) p++;
        if (*p) while (*++p) *pszBuffer = *p;
        *pszBuffer = 0;
    }
    else {
        return LUBE_FAIL;
    }

    return LUBE_OK;
}

int LubeContext::ClassMember(MemberType member, char *pszBuffer)
{
    const char *pszOutput;

    assert(NULL != m_pClass);

    switch (member) {
        case Member_Type:
            if (m_pClass->pDesc->dwAttribs & ClassAttrib_t_generic) {
                pszOutput = "generic";
            }
            else if (m_pClass->pDesc->dwAttribs & ClassAttrib_t_aspect) {
                pszOutput = "aspect";
            }
            else if (m_pClass->pDesc->dwAttribs & ClassAttrib_t_regime) {
                pszOutput = "regime";
            }
            else {
                pszOutput = "class";
            }
            break;
        case Member_Name:
            pszOutput = m_pClass->pszName;
            break;
        case Member_Attrib:
            sprintf(pszBuffer, "%08x", m_pClass->pDesc->dwAttribs);
            return LUBE_OK;
        case Member_Uuid:
            pszOutput = Uuid2CString(&m_pClass->pDesc->clsid, TRUE);
            break;
        case Member_Clsid:
            if (m_pModule->pszUunm) {
                sprintf(pszBuffer,
                    "    {%s, \\\n"
                    "    \"%s\"}",
                    Uuid2CString(&m_pClass->pDesc->clsid, TRUE),
                    m_pModule->pszUunm);
                return LUBE_OK;
            }
            pszOutput = "(null uunm)";
            break;
        default:
            assert(TRUE == FALSE);
            return LUBE_FAIL;
    }
    strcpy(pszBuffer, pszOutput);
    return LUBE_OK;
}

int LubeContext::InterfaceMember(MemberType member, char *pszBuffer)
{
    const char *pszOutput;

    assert(NULL != m_pInterface);

    switch (member) {
        case Member_Type:
            pszOutput = "interface";
            break;
        case Member_Name:
            pszOutput = m_pInterface->pszName;
            break;
        case Member_Attrib:
            sprintf(pszBuffer, "%08x", m_pInterface->pDesc->dwAttribs);
            return LUBE_OK;
        case Member_Iid:
        case Member_Uuid:
            pszOutput = Uuid2CString(&m_pInterface->pDesc->iid, TRUE);
            break;
        default:
            assert(TRUE == FALSE);
            return LUBE_FAIL;
    }
    strcpy(pszBuffer, pszOutput);
    return LUBE_OK;
}

int LubeContext::StructMember(MemberType member, char *pszBuffer)
{
    const char *pszOutput;

    assert(NULL != m_pStruct);

    switch (member) {
        case Member_Type:
            pszOutput = "struct";
            break;
        case Member_Name:
            pszOutput = m_pStruct->pszName;
            break;
        case Member_Attrib:
            return LUBE_OK;
        default:
            assert(TRUE == FALSE);
            return LUBE_FAIL;
    }
    strcpy(pszBuffer, pszOutput);
    return LUBE_OK;
}

int LubeContext::ConstMember(MemberType member, char *pszBuffer)
{
    const char *pszOutput;

    assert(NULL != m_pConst);

    switch (member) {
        case Member_Type:
            pszOutput = "const";
            break;
        case Member_Name:
            pszOutput = m_pConst->pszName;
            break;
        case Member_Value:
            if (m_pConst->type == INTEGER) {
                sprintf(pszBuffer, (m_pConst->v.intValue.bHexFormat ? "0x%08x":"%d"),
                    m_pConst->v.intValue.nValue);
            }
            else {
                assert(m_pConst->type == STRING);
                sprintf(pszBuffer, "\"%s\"", m_pConst->v.strValue.pszValue);
            }
            return LUBE_OK;
        case Member_Attrib:
            return LUBE_OK;
        default:
            assert(TRUE == FALSE);
            return LUBE_FAIL;
    }
    strcpy(pszBuffer, pszOutput);
    return LUBE_OK;
}

int LubeContext::EnumMember(MemberType member, char *pszBuffer)
{
    const char *pszOutput;

    assert(NULL != m_pEnum);

    switch (member) {
        case Member_Type:
            pszOutput = "enum";
            break;
        case Member_Name:
            pszOutput = m_pEnum->pszName;
            break;
        case Member_Attrib:
            return LUBE_OK;
        default:
            assert(TRUE == FALSE);
            return LUBE_FAIL;
    }
    strcpy(pszBuffer, pszOutput);
    return LUBE_OK;
}

int LubeContext::TypedefMember(MemberType member, char *pszBuffer)
{
    const char *pszOutput;

    assert(NULL != m_pTypedef);

    switch (member) {
        case Member_Type:
            pszOutput = Type2CString(m_pModule, &m_pTypedef->type);
            break;
        case Member_Name:
            pszOutput = m_pTypedef->pszName;
            break;
        case Member_Attrib:
            if (m_pTypedef->bDummyType) {
                strcpy(pszBuffer, "dummytype");
            }
            return LUBE_OK;
        default:
            assert(TRUE == FALSE);
            return LUBE_FAIL;
    }
    strcpy(pszBuffer, pszOutput);
    return LUBE_OK;
}

int LubeContext::MethodMember(MemberType member, char *pszBuffer)
{
    const char *pszOutput;

    assert(NULL != m_pMethod);

    switch (member) {
        case Member_Type:
            pszOutput = Type2CString(m_pModule, &m_pMethod->type);
            break;
        case Member_Name:
            pszOutput = m_pMethod->pszName;
            break;
        case Member_Attrib:
            return LUBE_OK;
        default:
            assert(TRUE == FALSE);
            return LUBE_FAIL;
    }
    strcpy(pszBuffer, pszOutput);
    return LUBE_OK;
}

int LubeContext::ParamMember(MemberType member, char *pszBuffer)
{
    const char *pszOutput = NULL;
    TypeDescriptor type;

    assert(NULL != m_pParam);

    switch (member) {
        case Member_Type:
            if (Type_struct == m_pParam->type.type
                    || Type_EMuid == m_pParam->type.type
                    || Type_EGuid == m_pParam->type.type
                    || Type_DateTime == m_pParam->type.type) {
                pszOutput = StructType2CString(m_pModule, &m_pParam->type);
            }
            else if (Type_alias == m_pParam->type.type) {
                GetOriginalType(m_pModule, &m_pParam->type, &type);
                if ((Type_EMuid == type.type)
                        || (Type_EGuid == type.type)
                        || (Type_DateTime == type.type)
                        || (Type_struct == type.type)) {
                    pszOutput = StructType2CString(m_pModule, &m_pParam->type);
                }
                else if (Type_ArrayOf == type.type
                        || Type_BufferOf == type.type
                        || Type_MemoryBuf == type.type
                        || Type_StringBuf == type.type) {
                    if (m_pParam->dwAttribs & ParamAttrib_in) {
                        if (0 == type.nPointer) {
                            strcpy(pszBuffer, "const ");
                            strcat(pszBuffer, Type2CString(m_pModule, &m_pParam->type));
                            strcat(pszBuffer, " &");
                            return LUBE_OK;
                        }
                        else {
                            assert(1 == type.nPointer);
                            strcpy(pszBuffer, Type2CString(m_pModule, &m_pParam->type));
                            return LUBE_OK;
                        }
                    }
                    if (m_pParam->dwAttribs & ParamAttrib_out) {
                        strcpy(pszBuffer, Type2CString(m_pModule, &m_pParam->type));
                        strcpy(pszBuffer, " *");
                        return LUBE_OK;
                    }
                }
                else {
                    pszOutput = Type2CString(m_pModule, &m_pParam->type);
                }
            }
            else if (Type_ArrayOf == m_pParam->type.type) {
                if ((m_pParam->dwAttribs & ParamAttrib_in)
                    && (0 == m_pParam->type.nPointer)) pszOutput = "const ArrayOf";
                else pszOutput = "ArrayOf";
                strcpy(pszBuffer, pszOutput);
                strcat(pszBuffer, "<");
                strcat(pszBuffer, Type2CString(m_pModule, m_pParam->type.pNestedType));
                strcat(pszBuffer, ">");
                if (m_pParam->dwAttribs & ParamAttrib_in) {
                    if (0 == m_pParam->type.nPointer) strcat(pszBuffer, " &");
                    else {
                       assert (1 == m_pParam->type.nPointer);
                       strcat(pszBuffer, " *");
                    }
                }
                else if (0 == m_pParam->type.nPointer) strcat(pszBuffer, " *");
                else strcat(pszBuffer, " **");
                return LUBE_OK;
            }
            else if (Type_BufferOf == m_pParam->type.type) {
                if ((m_pParam->dwAttribs & ParamAttrib_in)
                    && (0 == m_pParam->type.nPointer)) pszOutput = "const BufferOf";
                else pszOutput = "BufferOf";
                strcpy(pszBuffer, pszOutput);
                strcat(pszBuffer, "<");
                strcat(pszBuffer, Type2CString(m_pModule, m_pParam->type.pNestedType));
                strcat(pszBuffer, ">");
                if (m_pParam->dwAttribs & ParamAttrib_in) {
                    if (0 == m_pParam->type.nPointer) strcat(pszBuffer, " &");
                    else {
                       assert (1 == m_pParam->type.nPointer);
                       strcat(pszBuffer, " *");
                    }
                }
                else if (0 == m_pParam->type.nPointer) strcat(pszBuffer, " *");
                else strcat(pszBuffer, " **");
                return LUBE_OK;
            }
            else if (Type_MemoryBuf == m_pParam->type.type) {
                if ((m_pParam->dwAttribs & ParamAttrib_in)
                    && (0 == m_pParam->type.nPointer)) pszOutput = "const MemoryBuf";
                else pszOutput = "MemoryBuf";
                strcpy(pszBuffer, pszOutput);
                if (m_pParam->dwAttribs & ParamAttrib_in) {
                    if (0 == m_pParam->type.nPointer) strcat(pszBuffer, " &");
                    else {
                       assert (1 == m_pParam->type.nPointer);
                       strcat(pszBuffer, " *");
                    }
                }
                else if (0 == m_pParam->type.nPointer) strcat(pszBuffer, " *");
                else strcat(pszBuffer, " **");
                return LUBE_OK;
            }
            else if (Type_StringBuf == m_pParam->type.type) {
                if (0 == m_pParam->type.nPointer) pszOutput = "StringBuf *";
                else pszOutput = "StringBuf **";
            }
            else {
                pszOutput = Type2CString(m_pModule, &m_pParam->type);
            }
            break;
        case Member_Name:
            pszOutput = m_pParam->pszName;
            break;
        case Member_Attrib:
            pszOutput = ParamAttrib2String(m_pParam->dwAttribs);
            break;
        default:
            assert(TRUE == FALSE);
            return LUBE_FAIL;
    }
    strcpy(pszBuffer, pszOutput);
    return LUBE_OK;
}

int LubeContext::StructMemMember(MemberType member, char *pszBuffer)
{
    const char *pszOutput;
    const TypeDescriptor *pType;
    TypeDescriptor baseType;

    assert(NULL != m_pStructMember);

    switch (member) {
        case Member_Type:
            pType = &m_pStructMember->type;
            if (Type_Array == pType->type) {
                //Get base type of Array
                memset(&baseType, 0, sizeof(TypeDescriptor));
                GetArrayBaseType(m_pModule, pType, &baseType);
                pszOutput = Type2CString(m_pModule, &baseType);
            }
            else {
                pszOutput = Type2CString(m_pModule, &m_pStructMember->type);
            }
            break;
        case Member_Name:
            pszOutput = m_pStructMember->pszName;
            break;
        case Member_Dimention:
            //Handle the case of [m][n]...
            if (Type_Array == m_pStructMember->type.type) {
                pszOutput = Dims2CString(m_pModule,  &m_pStructMember->type);
                break;
            }
            else {
                pszBuffer[0] = 0;
                return LUBE_OK;
            }
        case Member_Attrib:
            return LUBE_OK;
        default:
            assert(TRUE == FALSE);
            return LUBE_FAIL;
    }
    strcpy(pszBuffer, pszOutput);
    return LUBE_OK;
}

int LubeContext::EnumMemMember(MemberType member, char *pszBuffer)
{
    const char *pszOutput;

    assert(NULL != m_pEnumMember);

    switch (member) {
        case Member_Type:
        case Member_Attrib:
            return LUBE_OK;
        case Member_Name:
            pszOutput = m_pEnumMember->pszName;
            break;
        case Member_Value:
            sprintf(pszBuffer, (m_pEnumMember->bHexFormat ? "0x%08x":"%d"),
                m_pEnumMember->nValue);
            return LUBE_OK;
        default:
            assert(TRUE == FALSE);
            return LUBE_FAIL;
    }
    strcpy(pszBuffer, pszOutput);
    return LUBE_OK;
}

int LubeContext::ClassParentMember(MemberType member, char *pszBuffer)
{
    assert(NULL != m_pClass);

    int nRet;
    ClassDirEntry *pOrigClass;

    if (!(m_pClass->pDesc->dwAttribs & ClassAttrib_hasparent)) return LUBE_OK;

    pOrigClass = m_pClass;
    m_pClass = m_pClassParent;
    nRet = ClassMember(member, pszBuffer);

    m_pClass = pOrigClass;
    return nRet;
}

int LubeContext::AspectMember(MemberType member, char *pszBuffer)
{
    assert(NULL != m_pAspect);

    int nRet;
    ClassDirEntry *pOrigClass;

    pOrigClass = m_pClass;
    m_pClass = m_pAspect;
    nRet = ClassMember(member, pszBuffer);

    m_pClass = pOrigClass;
    return nRet;
}

int LubeContext::AggregateMember(MemberType member, char *pszBuffer)
{
    assert(NULL != m_pAggregate);

    int nRet;
    ClassDirEntry *pOrigClass;

    pOrigClass = m_pClass;
    m_pClass = m_pAggregate;
    nRet = ClassMember(member, pszBuffer);

    m_pClass = pOrigClass;
    return nRet;
}

int LubeContext::ClassForAspectMember(MemberType member, char *pszBuffer)
{
    assert(NULL != m_pClass);

    int nRet;
    ClassDirEntry *pOrigClass;

    pOrigClass = m_pClass;
    m_pClass = m_pForClass;
    nRet = ClassMember(member, pszBuffer);

    m_pClass = pOrigClass;
    return nRet;
}

int LubeContext::IntfParentMember(MemberType member, char *pszBuffer)
{
    assert(NULL != m_pInterface);

    int nRet;
    InterfaceDirEntry *pOrigInterface;

    pOrigInterface = m_pInterface;
    m_pInterface = m_pIntfParent;
    nRet = InterfaceMember(member, pszBuffer);

    m_pInterface = pOrigInterface;
    return nRet;
}

int LubeContext::IntfParentParentMember(MemberType member, char *pszBuffer)
{
    assert(NULL != m_pInterface);

    int nRet;
    InterfaceDirEntry *pOrigInterface;

    pOrigInterface = m_pInterface;
    if (0 != m_pInterface->pDesc->sParentIndex) {
        m_pInterface = this->m_pModule->ppInterfaceDir[m_pIntfParent->pDesc->sParentIndex];
    }
    nRet = InterfaceMember(member, pszBuffer);

    m_pInterface = pOrigInterface;
    return nRet;
}

int LubeContext::IntfChildMember(MemberType member, char *pszBuffer)
{
    assert(NULL != m_pInterface);

    int nRet;
    InterfaceDirEntry *pOrigInterface;

    pOrigInterface = m_pInterface;
    m_pInterface = m_pIntfChild;
    nRet = InterfaceMember(member, pszBuffer);

    m_pInterface = pOrigInterface;
    return nRet;
}

int LubeContext::GetMemberString(
    ObjectType object, MemberType member, char *pszBuffer)
{
    *pszBuffer = 0;

    switch (object) {
        case Object_Module:
            return this->ModuleMember(member, pszBuffer);
        case Object_Library:
            return this->LibraryMember(member, pszBuffer);
        case Object_Class:
            return this->ClassMember(member, pszBuffer);
        case Object_Aspect:
            return this->AspectMember(member, pszBuffer);
        case Object_Aggregate:
            return this->AggregateMember(member, pszBuffer);
        case Object_ClassForAspect:
            return this->ClassForAspectMember(member, pszBuffer);
        case Object_ClsIntf:
        case Object_Interface:
        case Object_ClsIntfAndParents:
            return this->InterfaceMember(member, pszBuffer);
        case Object_Struct:
            return this->StructMember(member, pszBuffer);
        case Object_Enum:
            return this->EnumMember(member, pszBuffer);
        case Object_Const:
            return this->ConstMember(member, pszBuffer);
        case Object_Typedef:
            return this->TypedefMember(member, pszBuffer);
        case Object_ClassMethod:
        case Object_ClassConstructor:
        case Object_CbCoalescence:
        case Object_IntfMethod:
            return this->MethodMember(member, pszBuffer);
        case Object_Param:
            return this->ParamMember(member, pszBuffer);
        case Object_StcMember:
            return this->StructMemMember(member, pszBuffer);
        case Object_EnumMember:
            return this->EnumMemMember(member, pszBuffer);
        case Object_Super:
            return this->ClassParentMember(member, pszBuffer);
        case Object_Parent:
            return this->IntfParentMember(member, pszBuffer);
        case Object_Child:
            return this->IntfChildMember(member, pszBuffer);
        case Object_IntfParentParent:
            return this->IntfParentParentMember(member, pszBuffer);
        default:
            assert(TRUE == FALSE);
            return LUBE_FAIL;
    }
}
