//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "CConstructorInfo.h"

UInt32 CConstructorInfo::AddRef()
{
    Int32 nRef = atomic_inc(&m_cRef);
    return (UInt32)nRef;
}

UInt32 CConstructorInfo::Release()
{
    Int32 nRef = atomic_dec(&m_cRef);

    if (0 == nRef) {
        delete this;
    }

    return nRef;
}

PInterface CConstructorInfo::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IConstructorInfo) {
        return (IConstructorInfo *)this;
    }
    else if (riid == EIID_IFunctionInfo) {
        return (IFunctionInfo *)this;
    }
    else {
        return NULL;
    }
}

ECode CConstructorInfo::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

CConstructorInfo::~CConstructorInfo()
{
    if (m_pMethodInfo) m_pMethodInfo->Release();
}

ECode CConstructorInfo::Init(
    /* [in] */ CClsModule * pCClsModule,
    /* [in] */ MethodDescriptor *pMethodDescriptor,
    /* [in] */ UInt32 uIndex,
    /* [in] */ ClassID *pClsId)
{
    m_nOutParamIndex = pMethodDescriptor->cParams - 1;

    m_instClsId.pUunm = m_szUrn2;
    m_clsId.pUunm = m_szUrn;

    m_clsId.clsid =  pClsId->clsid;
    strcpy(m_clsId.pUunm, pClsId->pUunm);

    m_pMethodInfo = NULL;
    return g_objInfoList.AcquireMethodInfo(pCClsModule, pMethodDescriptor,
                                          uIndex, (IInterface **)&m_pMethodInfo);
}

ECode CConstructorInfo::GetName(
    /* [out] */ StringBuf * pName)
{
    return m_pMethodInfo->GetName(pName);
}

ECode CConstructorInfo::GetParamCount(
    /* [out] */ Int32 * pCount)
{
    ECode ec = m_pMethodInfo->GetParamCount(pCount);
    if (ec == NOERROR && *pCount) {
        *pCount -= 1;
    }
    return ec;
}

ECode CConstructorInfo::GetAllParamInfos(
    /* [out] */ BufferOf<IParamInfo *> * pParamInfos)
{
    return m_pMethodInfo->GetAllParamInfos(pParamInfos);
}

ECode CConstructorInfo::GetParamInfoByIndex(
    /* [in] */ Int32 index,
    /* [out] */ IParamInfo ** ppParamInfo)
{
    if (!ppParamInfo || index < 0) {
        return E_INVALID_ARGUMENT;
    }

    if (index >= m_nOutParamIndex) {
        return E_DOES_NOT_EXIST;
    }

    return m_pMethodInfo->GetParamInfoByIndex(index, ppParamInfo);
}

ECode CConstructorInfo::GetParamInfoByName(
    /* [in] */ String name,
    /* [out] */ IParamInfo ** ppParamInfo)
{
    return m_pMethodInfo->GetParamInfoByName(name, ppParamInfo);
}

ECode CConstructorInfo::CreateArgumentList(
    /* [out] */ IArgumentList ** ppArgumentList)
{
    Int32 iCount = 0;
    ECode ec = GetParamCount(&iCount);
    if (FAILED(ec)) return ec;

    if (!iCount) {
        return E_INVALID_OPERATION;
    }

    return m_pMethodInfo->CreateFunctionArgumentList(this, FALSE, ppArgumentList);
}

ECode CConstructorInfo::CreateObjInRgm(
    /* [in] */ PRegime rgm,
    /* [in] */ IArgumentList * pArgumentList,
    /* [out] */ PInterface * pObject)
{
    IInterface* pClsObject;
    Int32 iCount = 0;
    ECode ec = GetParamCount(&iCount);
    if (FAILED(ec)) return ec;

    if (!pArgumentList) {
        if (iCount) {
            return E_INVALID_ARGUMENT;
        }
        else {
            ec = m_pMethodInfo->CreateFunctionArgumentList(this, FALSE, &pArgumentList);
            if (FAILED(ec)) return ec;
        }
    }

    ec = _CObject_AcquireClassFactory(m_instClsId, rgm, EIID_IClassObject, (IInterface**)&pClsObject);
    if (FAILED(ec)) return ec;
    ec = pArgumentList->SetOutputArgumentOfObjectPtrPtr(m_nOutParamIndex, pObject);
    if (SUCCEEDED(ec)) {
        ec = m_pMethodInfo->Invoke(pClsObject, pArgumentList);
    }

    if (!iCount) pArgumentList->Release();
    pClsObject->Release();
    return ec;
}

ECode CConstructorInfo::CreateObject(
    /* [in] */ IArgumentList * pArgumentList,
    /* [out] */ PInterface * pObject)
{
    if (!pObject) {
        return E_INVALID_ARGUMENT;
    }

    return CreateObjInRgm(RGM_SAME_DOMAIN, pArgumentList, pObject);
}

ECode CConstructorInfo::CreateObjectInRegime(
    /* [in] */ PRegime rgm,
    /* [in] */ IArgumentList * pArgumentList,
    /* [out] */ PInterface * pObject)
{
    if (IS_INVALID_REGIME(rgm) || !pObject) {
        return E_INVALID_ARGUMENT;
    }

    return CreateObjInRgm(rgm, pArgumentList, pObject);
}
