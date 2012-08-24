//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <lube.h>

PVOID ModuleArg(PLUBECTX pCtx, MemberType member)
{
    switch (member) {
        case Member_None:
            return (PVOID)pCtx->m_pModule;
        case Member_Type:
        case Member_Attrib:
            return (PVOID)&pCtx->m_pModule->dwAttribs;
        case Member_Name:
            return (PVOID)pCtx->m_pModule->pszName;
        case Member_Major_Version:
            return (PVOID)&pCtx->m_pModule->cMajorVersion;
        case Member_Minor_Version:
            return (PVOID)&pCtx->m_pModule->cMinorVersion;
        case Member_Version:
            return NULL; // version is not a true member, so it should be used as argument
        case Member_Uuid:
            return (PVOID)&pCtx->m_pModule->uuid;
        case Member_Uunm:
            return (PVOID)pCtx->m_pModule->pszUunm;
        case Member_CarCode:
            return (PVOID)pCtx->m_pModule->nChecksum;
        case Member_BuildDate:
            return (PVOID)pCtx->m_pModule->nBarcode;
        default:
            return NULL;
    }
}

PVOID LibraryArg(PLUBECTX pCtx, MemberType member)
{
    char *p;

    p = pCtx->m_pLibrary + strlen(pCtx->m_pLibrary);
    while (p != pCtx->m_pLibrary && !IS_PATH_SEPARATOR(*(p - 1)))
        p--;

    switch (member) {
        case Member_None:
            return (PVOID)pCtx->m_pLibrary;
        case Member_Name:
            return p;
        case Member_Type:
            p = pCtx->m_pLibrary;
            while (0 != *p) {
                if ('.' == *p) return (PVOID)(p + 1);
                p++;
            }
        default:
            return NULL;
    }
}

PVOID ClassArg(PLUBECTX pCtx, MemberType member)
{
    switch (member) {
        case Member_None:
            return (PVOID)pCtx->m_pClass;
        case Member_Type:
        case Member_Attrib:
            return (PVOID)&pCtx->m_pClass->pDesc->dwAttribs;
        case Member_Name:
            return (PVOID)pCtx->m_pClass->pszName;
        case Member_Uuid:
        case Member_Clsid:
            return (PVOID)&pCtx->m_pClass->pDesc->clsid;
        default:
            return NULL;
    }
}

PVOID ClsIntfArg(PLUBECTX pCtx, MemberType member)
{
    switch (member) {
        case Member_None:
            return (PVOID)pCtx->m_pClsIntf;
        case Member_Type:
            return (PVOID)&pCtx->m_pInterface->pDesc->dwAttribs;
        case Member_Attrib:
            return (PVOID)&pCtx->m_pClsIntf->wAttribs;
        case Member_Name:
            return (PVOID)pCtx->m_pInterface->pszName;
        case Member_Uuid:
        case Member_Iid:
            return (PVOID)&pCtx->m_pInterface->pDesc->iid;
        default:
            return NULL;
    }
}

PVOID InterfaceArg(PLUBECTX pCtx, MemberType member)
{
    switch (member) {
        case Member_None:
            return (PVOID)pCtx->m_pInterface;
        case Member_Type:
        case Member_Attrib:
            return (PVOID)&pCtx->m_pInterface->pDesc->dwAttribs;
        case Member_Name:
            return (PVOID)pCtx->m_pInterface->pszName;
        case Member_Uuid:
        case Member_Iid:
            return (PVOID)&pCtx->m_pInterface->pDesc->iid;
        default:
            return NULL;
    }
}

PVOID StructArg(PLUBECTX pCtx, MemberType member)
{
    switch (member) {
        case Member_None:
            return (PVOID)pCtx->m_pStruct;
        case Member_Name:
            return (PVOID)pCtx->m_pStruct->pszName;
        default:
            return NULL;
    }
}

PVOID EnumArg(PLUBECTX pCtx, MemberType member)
{
    switch (member) {
        case Member_None:
            return (PVOID)pCtx->m_pEnum;
        case Member_Name:
            return (PVOID)pCtx->m_pEnum->pszName;
        default:
            return NULL;
    }
}

PVOID TypedefArg(PLUBECTX pCtx, MemberType member)
{
    switch (member) {
        case Member_None:
            return (PVOID)pCtx->m_pTypedef;
        case Member_Name:
            return (PVOID)pCtx->m_pTypedef->pszName;
        case Member_Type:
            return (PVOID)&pCtx->m_pTypedef->type;
        case Member_Attrib:
            return (PVOID)&pCtx->m_pTypedef->bDummyType;
        default:
            return NULL;
    }
}

PVOID MethodArg(PLUBECTX pCtx, MemberType member)
{
    switch (member) {
        case Member_None:
            return (PVOID)pCtx->m_pMethod;
        case Member_Name:
            return (PVOID)pCtx->m_pMethod->pszName;
        case Member_Type:
            return (PVOID)&pCtx->m_pMethod->type;
        case Member_Attrib:
            return (PVOID)&pCtx->m_pMethod->dwAttribs;
        default:
            return NULL;
    }
}

PVOID ParamArg(PLUBECTX pCtx, MemberType member)
{
    switch (member) {
        case Member_None:
            return (PVOID)pCtx->m_pParam;
        case Member_Name:
            return (PVOID)pCtx->m_pParam->pszName;
        case Member_Type:
            return (PVOID)&pCtx->m_pParam->type;
        case Member_Attrib:
            return (PVOID)&pCtx->m_pParam->dwAttribs;
        default:
            return NULL;
    }
}

PVOID StructMemberArg(PLUBECTX pCtx, MemberType member)
{
    switch (member) {
        case Member_None:
            return (PVOID)pCtx->m_pStructMember;
        case Member_Name:
            return (PVOID)pCtx->m_pStructMember->pszName;
        case Member_Type:
            return (PVOID)&pCtx->m_pStructMember->type;
        default:
            return NULL;
    }
}

PVOID EnumMemberArg(PLUBECTX pCtx, MemberType member)
{
    switch (member) {
        case Member_None:
            return (PVOID)pCtx->m_pEnumMember;
        case Member_Name:
            return (PVOID)pCtx->m_pEnumMember->pszName;
        case Member_Value:
            return (PVOID)&pCtx->m_pEnumMember->nValue;
        default:
            return NULL;
    }
}

PVOID ClassParentArg(PLUBECTX pCtx, MemberType member)
{
    ClassDirEntry *pOrigClass;
    PVOID pvArg;

    if (!(pCtx->m_pClass->pDesc->dwAttribs & ClassAttrib_hasparent)) {
        return NULL;
    }

    pOrigClass = pCtx->m_pClass;
    pCtx->m_pClass = pCtx->m_pClassParent;

    pvArg = ClassArg(pCtx, member);

    pCtx->m_pClass = pOrigClass;
    return pvArg;
}

PVOID AspectArg(PLUBECTX pCtx, MemberType member)
{
    ClassDirEntry *pOrigClass;
    PVOID pvArg;

    pOrigClass = pCtx->m_pClass;
    pCtx->m_pClass = pCtx->m_pAspect;

    pvArg = ClassArg(pCtx, member);

    pCtx->m_pClass = pOrigClass;
    return pvArg;
}

PVOID AggregateArg(PLUBECTX pCtx, MemberType member)
{
    ClassDirEntry *pOrigClass;
    PVOID pvArg;

    pOrigClass = pCtx->m_pClass;
    pCtx->m_pClass = pCtx->m_pAggregate;

    pvArg = ClassArg(pCtx, member);

    pCtx->m_pClass = pOrigClass;
    return pvArg;
}

PVOID ClassForAspectArg(PLUBECTX pCtx, MemberType member)
{
    ClassDirEntry *pOrigClass;
    PVOID pvArg;

    pOrigClass = pCtx->m_pClass;
    pCtx->m_pClass = pCtx->m_pForClass;

    pvArg = ClassArg(pCtx, member);

    pCtx->m_pClass = pOrigClass;
    return pvArg;
}

PVOID IntfParentArg(PLUBECTX pCtx, MemberType member)
{
    InterfaceDirEntry *pOrigInterface;
    PVOID pvArg;

    pOrigInterface = pCtx->m_pInterface;
    pCtx->m_pInterface = pCtx->m_pIntfParent;

    pvArg = InterfaceArg(pCtx, member);

    pCtx->m_pInterface = pOrigInterface;
    return pvArg;
}

PVOID GetFunctionArg(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    switch (pDesc->object) {
        case Object_Module:
            return ModuleArg(pCtx, pDesc->member);
        case Object_Library:
            return LibraryArg(pCtx, pDesc->member);
        case Object_Class:
            return ClassArg(pCtx, pDesc->member);
        case Object_ClsIntf:
        case Object_ClsIntfAndParents:
            return ClsIntfArg(pCtx, pDesc->member);
        case Object_Aspect:
            return AspectArg(pCtx, pDesc->member);
        case Object_Aggregate:
            return AggregateArg(pCtx, pDesc->member);
        case Object_ClassForAspect:
            return ClassForAspectArg(pCtx, pDesc->member);
        case Object_Interface:
            return InterfaceArg(pCtx, pDesc->member);
        case Object_Struct:
            return StructArg(pCtx, pDesc->member);
        case Object_Enum:
            return EnumArg(pCtx, pDesc->member);
        case Object_Typedef:
            return TypedefArg(pCtx, pDesc->member);
        case Object_IntfMethod:
        case Object_ClassMethod:
            return MethodArg(pCtx, pDesc->member);
        case Object_Param:
            return ParamArg(pCtx, pDesc->member);
        case Object_StcMember:
            return StructMemberArg(pCtx, pDesc->member);
        case Object_EnumMember:
            return EnumMemberArg(pCtx, pDesc->member);
        case Object_Super:
            return ClassParentArg(pCtx, pDesc->member);
        case Object_Parent:
            return IntfParentArg(pCtx, pDesc->member);
        default:
            assert(TRUE == FALSE);
            return NULL;
    }
}

IMPL_STATEMENT(Func)(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    char szBuf[c_nStrBufSize];
    PVOID pvArg;
    UserFn_t fn;

    if (pDesc->pvData) {
        if (pCtx->ParseStringEmbed((char *)pDesc->pvData, szBuf) < 0) {
            return LUBE_OK;
        }
        pvArg = (PVOID)szBuf;
    }
    else {
        pvArg = GetFunctionArg(pCtx, pDesc);
        if (!pvArg) return LUBE_OK;
    }

    fn = GetUserFunction(pDesc->dwExtra);
    return (*fn)(pCtx, pDesc, pvArg);
}
