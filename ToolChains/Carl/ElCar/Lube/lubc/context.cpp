//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <windows.h>
#include <assert.h>

#include "parser.h"

ParserContext::ParserContext(char *pBuffer)
{
    memset(this, 0, sizeof(ParserContext));
    m_curObject = Object_Module;
    m_pTextBuffer = pBuffer;
}

ParserContext::~ParserContext()
{
    if (m_pLube) DestroyLube(m_pLube);
}

PSTATEDESC ParserContext::CreateBlockette(StatementType type)
{
    PSTATEDESC pDesc, pTemp;

    pDesc = new StateDesc;
    if (!pDesc) return NULL;
    memset(pDesc, 0, sizeof(StateDesc));
    pDesc->type = type;
    pDesc->uStateIndex = (UINT)GetStatementIndex(type);

    if (NULL == m_pCurrentState->pBlockette) {
        m_pCurrentState->pBlockette = pDesc;
    }
    else {
        pTemp = m_pCurrentState->pBlockette;
        while (NULL != pTemp->pNext) pTemp = pTemp->pNext;
        pTemp->pNext = pDesc;
    }
    pDesc->pOwner = m_pCurrentState;

    return pDesc;
}

PSTATEDESC ParserContext::CreateStatement(StatementType type)
{
    PSTATEDESC pDesc;

    pDesc = this->CreateBlockette(type);
    if (!pDesc) return NULL;
    m_pCurrentState = pDesc;
    return pDesc;
}

void ParserContext::LeaveStatement()
{
    m_pCurrentState = m_pCurrentState->pOwner;
}

ObjectType ParserContext::EnterObject(ObjectType object)
{
    ObjectType origObject = m_curObject;

    switch (object) {
        case Object_Library:
            m_nLibrary++;
            break;

        case Object_Super:
            object = Object_Class;
        case Object_Class:
            m_nClass++;
            break;

        case Object_Aspect:
            m_nAspect++;
            break;

        case Object_Aggregate:
            m_nAggregate++;
            break;

        case Object_ClassForAspect:
            m_nForClass++;
            break;

        case Object_ClsIntf:
        case Object_ClsIntfAndParents:
            m_nClsIntf++;
            m_nInterface++;
            break;

        case Object_Parent:
            object = Object_Interface;
        case Object_Interface:
            m_nInterface++;
            break;

        case Object_Struct:
            m_nStruct++;
            break;

        case Object_Enum:
            m_nEnum++;
            break;

        case Object_Const:
            m_nConst++;
            break;

        case Object_Typedef:
            m_nTypedef++;
            break;

        case Object_IParentMtd:
            object = Object_IntfMethod;
        case Object_IntfMethod:
            m_nIntfMethod++;
            break;

        case Object_CParentMtd:
            object = Object_ClassMethod;
        case Object_CbCoalescence:
        case Object_ClassMethod:
        case Object_ClassConstructor:
            m_nClassMethod++;
            m_nInterface++;
            break;

        case Object_Param:
            m_nParameter++;
            break;

        case Object_StcMember:
            m_nStructMember++;
            break;

        case Object_EnumMember:
            m_nEnumMember++;
            break;

        default:
            assert(TRUE == FALSE);
            break;
    }

    m_curObject = object;
    return origObject;
}

void ParserContext::LeaveObject(ObjectType dwOrigObject)
{
    switch (m_curObject) {
        case Object_Library:
            m_nLibrary--;
            break;

        case Object_Class:
            m_nClass--;
            break;

        case Object_ClsIntf:
        case Object_ClsIntfAndParents:
            m_nClsIntf--;
        case Object_Interface:
            m_nInterface--;
            break;

        case Object_Super:
        case Object_Parent:
            assert(0);
            break;

        case Object_Aspect:
            m_nAspect--;
            break;

        case Object_Aggregate:
            m_nAggregate--;
            break;

        case Object_ClassForAspect:
            m_nForClass--;
            break;

        case Object_Struct:
            m_nStruct--;
            break;

        case Object_Enum:
            m_nEnum--;
            break;

        case Object_Const:
            m_nConst--;
            break;

        case Object_Typedef:
            m_nTypedef--;
            break;

        case Object_CbCoalescence:
        case Object_ClassMethod:
        case Object_ClassConstructor:
            m_nClassMethod--;
            m_nInterface--;
            break;

        case Object_IntfMethod:
            m_nIntfMethod--;
            break;

        case Object_Param:
            m_nParameter--;
            break;

        case Object_StcMember:
            m_nStructMember--;
            break;

        case Object_EnumMember:
            m_nEnumMember--;
            break;

        default:
            assert(TRUE == FALSE);
            break;
    }
    m_curObject = dwOrigObject;
}
