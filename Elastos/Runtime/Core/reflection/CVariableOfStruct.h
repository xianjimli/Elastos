//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __CSTRUCTVARIABLE_H__
#define __CSTRUCTVARIABLE_H__

#include "refutil.h"

class CVariableOfStruct :
    public IVariableOfStruct,
    public IStructSetter,
    public IStructGetter
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI GetTypeInfo(
        /* [out] */ IDataTypeInfo ** ppTypeInfo);

    CARAPI GetPayload(
        /* [out] */ PVoid * pPayload);

    CARAPI Rebox(
        /* [in] */ PVoid localVariablePtr);

    CARAPI GetSetter(
        /* [out] */ IStructSetter ** ppSetter);

    CARAPI GetGetter(
        /* [out] */ IStructGetter ** ppGetter);

    CARAPI Init(
        /* [in] */ IStructInfo *pStructInfo,
        /* [in] */ PVoid pVarBuf);

//--------------Setter----------------------------------------------------------

    CARAPI ZeroAllFields();

    CARAPI SetInt16Field(
        /* [in] */ String name,
        /* [in] */ Int16 value);

    CARAPI SetInt32Field(
        /* [in] */ String name,
        /* [in] */ Int32 value);

    CARAPI SetInt64Field(
        /* [in] */ String name,
        /* [in] */ Int64 value);

    CARAPI SetByteField(
        /* [in] */ String name,
        /* [in] */ Byte value);

    CARAPI SetFloatField(
        /* [in] */ String name,
        /* [in] */ Float value);

    CARAPI SetDoubleField(
        /* [in] */ String name,
        /* [in] */ Double value);

    CARAPI SetChar8Field(
        /* [in] */ String name,
        /* [in] */ Char8 value);

    CARAPI SetChar16Field(
        /* [in] */ String name,
        /* [in] */ Char16 value);

    CARAPI SetBooleanField(
        /* [in] */ String name,
        /* [in] */ Boolean value);

    CARAPI SetEMuidField(
        /* [in] */ String name,
        /* [in] */ EMuid * pValue);

    CARAPI SetEGuidField(
        /* [in] */ String name,
        /* [in] */ EGuid * pValue);

    CARAPI SetECodeField(
        /* [in] */ String name,
        /* [in] */ ECode value);

    CARAPI SetLocalPtrField(
        /* [in] */ String name,
        /* [in] */ LocalPtr value);

    CARAPI SetLocalTypeField(
        /* [in] */ String name,
        /* [in] */ PVoid value);

    CARAPI SetEnumField(
        /* [in] */ String name,
        /* [in] */ Int32 value);

    CARAPI GetStructFieldSetter(
        /* [in] */ String name,
        /* [out] */ IStructSetter ** ppSetter);

    CARAPI GetCppVectorFieldSetter(
        /* [in] */ String name,
        /* [out] */ ICppVectorSetter ** ppSetter);

    CARAPI SetFieldValueByName(
        /* [in] */ String name,
        /* [in] */ void *pParam,
        /* [in] */ CarDataType type);

//--------------Getter----------------------------------------------------------

    CARAPI GetInt16Field(
        /* [in] */ String name,
        /* [out] */ Int16 * pValue);

    CARAPI GetInt32Field(
        /* [in] */ String name,
        /* [out] */ Int32 * pValue);

    CARAPI GetInt64Field(
        /* [in] */ String name,
        /* [out] */ Int64 * pValue);

    CARAPI GetByteField(
        /* [in] */ String name,
        /* [out] */ Byte * pValue);

    CARAPI GetFloatField(
        /* [in] */ String name,
        /* [out] */ Float * pValue);

    CARAPI GetDoubleField(
        /* [in] */ String name,
        /* [out] */ Double * pValue);

    CARAPI GetChar8Field(
        /* [in] */ String name,
        /* [out] */ Char8 * pValue);

    CARAPI GetChar16Field(
        /* [in] */ String name,
        /* [out] */ Char16 * pValue);

    CARAPI GetBooleanField(
        /* [in] */ String name,
        /* [out] */ Boolean * pValue);

    CARAPI GetEMuidField(
        /* [in] */ String name,
        /* [out] */ EMuid * pValue);

    CARAPI GetEGuidField(
        /* [in] */ String name,
        /* [out] */ EGuid * pValue);

    CARAPI GetECodeField(
        /* [in] */ String name,
        /* [out] */ ECode * pValue);

    CARAPI GetLocalPtrField(
        /* [in] */ String name,
        /* [out] */ LocalPtr * pValue);

    CARAPI GetLocalTypeField(
        /* [in] */ String name,
        /* [out] */ PVoid value);

    CARAPI GetEnumField(
        /* [in] */ String name,
        /* [out] */ Int32 * pValue);

    CARAPI GetStructFieldGetter(
        /* [in] */ String name,
        /* [out] */ IStructGetter ** ppGetter);

    CARAPI GetCppVectorFieldGetter(
        /* [in] */ String name,
        /* [out] */ ICppVectorGetter ** ppGetter);

    CARAPI GetFieldValueByName(
        /* [in] */ String name,
        /* [in] */ void *pParam,
        /* [in] */ CarDataType type);

    CARAPI GetIndexByName(
        /* [in] */ String name,
        /* [out] */ Int32 *pIndex);

    CARAPI AcquireCppVectorFieldSGetter(
        /* [in] */ String name,
        /* [in] */ Boolean bSetter,
        /* [out] */ IInterface ** ppSGetter);

    CVariableOfStruct();

    virtual ~CVariableOfStruct();

private:
    IStructInfo     *m_pStructInfo;
    StructFieldDesc  *m_pStructFieldDesc;
    PByte   m_pVarBuf;
    Boolean m_bAlloc;
    Int32  m_iCount;
    UInt32  m_uVarSize;

    IInterface **m_pCppVectorSGetters;

    Int32 m_cRef;
};

#endif // __CSTRUCTVARIABLE_H__
