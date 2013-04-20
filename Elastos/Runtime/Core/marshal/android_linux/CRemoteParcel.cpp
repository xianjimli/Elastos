
#include <stdio.h>
#include "CRemoteParcel.h"

ECode LookupClassInfo(
    /* [in] */ REMuid rclsid,
    /* [out] */ CIClassInfo **ppClassInfo);

ECode GetRemoteClassInfo(
    /* [in] */ android::IBinder* pRemoteObj,
    /* [in] */ REMuid clsId,
    /* [out] */ CIClassInfo ** ppClassInfo);

#define ROUND4(n)       (((n)+3)&~3)   // round up to multiple of 4 bytes
#define ROUND8(n)       (((n)+7)&~7)   // round up to multiple of 8 bytes

enum Type {
    Type_Unknown = 0,

    Type_Byte, Type_Boolean, Type_Char8, Type_Char16,
    Type_Int16, Type_Int32, Type_Int64, Type_Float, Type_Double,
    Type_CString, Type_String, Type_Struct, Type_EMuid, Type_EGuid,
    Type_ArrayOf, Type_ArrayOfCString, Type_ArrayOfString,
    Type_BufferOf, Type_BufferOfCString, Type_BufferOfString,
    Type_StringBuf, Type_MemoryBuf, Type_InterfacePtr,

    Type_BytePtr, Type_BooleanPtr, Type_Char8Ptr,  Type_Char16Ptr,
    Type_Int16Ptr, Type_Int32Ptr,  Type_Int64Ptr, Type_FloatPtr,  Type_DoublePtr,
    Type_StringPtr, Type_StructPtr, Type_EMuidPtr, Type_EGuidPtr, Type_InterfacePtrPtr,
    Type_BufferOfPtr, Type_Fd, Type_Fd_Dup
};

CRemoteParcel::CRemoteParcel() :
    m_nRefs(1),
    m_freeDataTag(TRUE)
{
    m_pData = new android::Parcel();
    m_pData->writeInplace(sizeof(MarshalHeader));
}

CRemoteParcel::CRemoteParcel(
    /* [in] */ android::Parcel *pData) :
    m_nRefs(1),
    m_freeDataTag(FALSE)
{
    m_pData = pData;
//    m_pData->writeInplace(sizeof(MarshalHeader));
}

CRemoteParcel::~CRemoteParcel()
{
    if (m_freeDataTag) {
        delete m_pData;
    }
}

PInterface CRemoteParcel::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IParcel) {
        return (IParcel*)this;
    }

    return NULL;
}

UInt32 CRemoteParcel::AddRef()
{
    Int32 nRefs = atomic_inc(&m_nRefs);

    return (UInt32)nRefs;
}

UInt32 CRemoteParcel::Release()
{
    Int32 nRefs = atomic_dec(&m_nRefs);

    if (nRefs == 0) {
        this->~CRemoteParcel();
        free(this);
    }

    return (UInt32)nRefs;
}

ECode CRemoteParcel::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IParcel *)this) {
        *pIID = EIID_IParcel;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode CRemoteParcel::GrowElemBuffer(
    /* [in] */ Int32 minSize)
{
    return NOERROR;
}

ECode CRemoteParcel::ReadValue(PVoid pValue, Int32 type)
{
    switch(type) {
        case Type_Byte:
            *(Byte*)pValue = (Byte)m_pData->readInt32();
            break;

        case Type_Boolean:
            *(Boolean*)pValue = (Boolean)m_pData->readInt32();
            break;

        case Type_Char8:
            *(Char8*)pValue = (Char8)m_pData->readInt32();
            break;

        case Type_Char16:
            *(Char16*)pValue = (Char16)m_pData->readInt32();
            break;

        case Type_Int16:
            *(Int16*)pValue = (Int16)m_pData->readInt32();
            break;

        case Type_Int32:
            *(Int32*)pValue = (Int32)m_pData->readInt32();
            break;

        case Type_Int64:
        case Type_Double:
            *(Int64*)pValue = (Int64)m_pData->readInt64();
            break;

        case Type_Float:
            *(Float*)pValue = (Float)m_pData->readFloat();
            break;

        case Type_CString:
            {
                Int32 tag = m_pData->readInt32();
                if (tag != MSH_NULL) {
                    const char* str = m_pData->readCString();
                    *(CString*)pValue = CString::Duplicate(str);
                }
                else {
                    *(CString*)pValue = NULL;
                }
            }
            break;

        case Type_String:
            {
                Int32 tag = m_pData->readInt32();
                if (tag != MSH_NULL) {
                    const char* str = m_pData->readCString();
                    *(String*)pValue = str;
                }
                else {
                    *(String*)pValue = NULL;
                }
            }
            break;

        case Type_StringPtr:
            {
                Int32 tag = m_pData->readInt32();
                if (tag != MSH_NULL) {
                    const char* cstr = m_pData->readCString();
                    String* str = new String(cstr);
                    *(String**)pValue = str;
                }
                else {
                    String* str = new String(NULL);
                    *(String**)pValue = str;
                }
            }
            break;

        case Type_Struct:
            {
                Int32 len = (Int32)m_pData->readInt32();

                m_pData->read(pValue, len);
            }
            break;

        case Type_StructPtr:
            {
                Int32 tag = (Int32)m_pData->readInt32();
                if (tag != MSH_NULL) {
                    Int32 len = (Int32)m_pData->readInt32();
                    *(UInt32**)pValue = (UInt32*)m_pData->readInplace(len);
                }
                else {
                    *(UInt32**)pValue = NULL;
                }
            }
            break;

        case Type_EMuid:
            m_pData->read(pValue, sizeof(EMuid));

            break;

        case Type_EGuid:
            {
                const char* str;

                m_pData->read(pValue, sizeof(EGuid));
                str = m_pData->readCString();
                (*(EGuid*)pValue).pUunm = (char*)malloc(strlen(str) + 1);
                strcpy((*(EGuid*)pValue).pUunm, str);
            }
            break;

        case Type_EMuidPtr:
            {
                Int32 tag = (Int32)m_pData->readInt32();
                if (tag != MSH_NULL) {
                    *(EMuid**)pValue = (EMuid*)m_pData->readInplace(sizeof(EMuid));
                }
                else {
                    *(EMuid**)pValue = NULL;
                }
            }
            break;

        case Type_EGuidPtr:
            {
                Int32 tag = (Int32)m_pData->readInt32();
                if (tag != MSH_NULL) {
                    EGuid* guid = new EGuid();
                    m_pData->read((void*)guid, sizeof(EGuid));
                    guid->pUunm = (char*)m_pData->readCString();
                    *(EGuid**)pValue = guid;
                }
                else {
                    *(EGuid**)pValue = NULL;
                }
            }
            break;

        case Type_BufferOf:
            {
                Int32 tag = (Int32)m_pData->readInt32();
                if (tag != MSH_NULL) {
                    CarQuintet buf;

                    m_pData->read((void*)&buf, sizeof(buf));
                    ((PCARQUINTET)(UInt32*)pValue)->m_used = buf.m_used;
                    m_pData->read(((PCARQUINTET)(UInt32*)pValue)->m_pBuf, \
                        buf.m_size);
                }
            }
            break;

        case Type_BufferOfPtr:
            {
                Int32 tag = (Int32)m_pData->readInt32();
                if (tag != MSH_NULL) {
                    PCARQUINTET q = new CarQuintet();
                    *(PCARQUINTET*)pValue = q;
                    m_pData->read((void*)q, sizeof(CarQuintet));
                    Int32 size = (*(PCARQUINTET*)pValue)->m_size;
                    if (size == 0) {
                        (*(PCARQUINTET*)pValue)->m_pBuf = NULL;
                    }
                    else {
                        if (CarQuintetFlag_Type_IObject
                            != ((*(PCARQUINTET*)pValue)->m_flags
                                    & CarQuintetFlag_TypeMask)) {
                            Byte* buf = (Byte*)malloc(size);
                            m_pData->read((void*)buf, size);
                            (*(PCARQUINTET*)pValue)->m_pBuf = buf;
                        }
                        else {
                            Int32 used = (*(PCARQUINTET*)pValue)->m_used
                                / sizeof(IInterface *);
                            IInterface** pBuf = (IInterface**)calloc(size, sizeof(IInterface*));
                            (*(PCARQUINTET*)pValue)->m_pBuf = pBuf;
                            for (int i = 0; i < used; i++) {
                                tag = (Int32)m_pData->readInt32();
                                if (tag != MSH_NULL) {
                                    InterfacePack ipack;
                                    CIClassInfo *pClassInfo = NULL;
                                    IParcelable *pParcelable = NULL;
                                    ClassID classId;
                                    InterfaceID iid;
                                    ECode ec;

                                    android::sp<android::IBinder> binder = m_pData->readStrongBinder();
                                    m_pData->read((void *)&ipack, sizeof(InterfacePack));
                                    ipack.m_pBinder = binder;
                                    if (IsParcelable(&ipack, &pClassInfo)) {
                                        classId.clsid = ipack.m_clsid;
                                        classId.pUunm = pClassInfo->pszUunm;

                                        ec = _CObject_CreateInstance(classId, RGM_SAME_DOMAIN,
                                                EIID_IParcelable, (IInterface**)&pParcelable);
                                        if (FAILED(ec)) return ec;

                                        pParcelable->ReadFromParcel(this);
                                        iid = pClassInfo->interfaces[ipack.m_uIndex]->iid;
                                        *((IInterface**)pBuf + i) = pParcelable->Probe(iid);
                                    }
                                    else {
                                        ec = StdUnmarshalInterface(
                                                UnmarshalFlag_Noncoexisting,
                                                &ipack,
                                                (IInterface **)&pBuf[i]);
                                        if (FAILED(ec)) {
                                            MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                                                    "MshProc: unmsh interface, ec = %x\n", ec));
                                            return ec;
                                        }
                                    }
                                }
                                else {
                                    pBuf[i] = NULL;
                                }
                            }
                        }
                    }

                }
                else {
                    *(UInt32*)pValue = NULL;
                }
            }
            break;

        case Type_InterfacePtr:
            {
                Int32 tag = (Int32)m_pData->readInt32();
                if (tag != MSH_NULL) {
                    InterfacePack ipack;
                    CIClassInfo *pClassInfo = NULL;
                    IParcelable *pParcelable = NULL;
                    ClassID classId;
                    InterfaceID iid;
                    ECode ec;

                    android::sp<android::IBinder> binder = m_pData->readStrongBinder();
                    (InterfacePack *)m_pData->read((void *)&ipack, sizeof(InterfacePack));
                    ipack.m_pBinder = binder;
                    if (IsParcelable(&ipack, &pClassInfo)) {
                        classId.clsid = ipack.m_clsid;
                        classId.pUunm = pClassInfo->pszUunm;

                        ec = _CObject_CreateInstance(classId, RGM_SAME_DOMAIN,
                                EIID_IParcelable, (IInterface**)&pParcelable);
                        if (FAILED(ec)) return ec;

                        pParcelable->ReadFromParcel(this);
                        iid = pClassInfo->interfaces[ipack.m_uIndex]->iid;
                        *(IInterface**)pValue = pParcelable->Probe(iid);
                    }
                    else {
                        ec = StdUnmarshalInterface(
                                UnmarshalFlag_Noncoexisting,
                                &ipack,
                                (IInterface **)pValue);
                        if (FAILED(ec)) {
                            MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                                    "MshProc: unmsh interface, ec = %x\n", ec));
                            return ec;
                        }
                    }
                }
                else {
                    *(IInterface**)pValue = NULL;
                }
            }
            break;

        case Type_Int32Ptr:
            {
                Int32 tag = (Int32)m_pData->readInt32();
                if (tag != MSH_NULL) {
                    Int32* pv = (Int32*)malloc(sizeof(Int32));
                    *pv = m_pData->readInt32();
                    *(Int32**)pValue = pv;
                }
                else {
                    *(Int32**)pValue = NULL;
                }
            }
            break;

        case Type_Int64Ptr:
            {
                Int32 tag = (Int32)m_pData->readInt32();
                if (tag != MSH_NULL) {
                    Int64* pv = (Int64*)malloc(sizeof(Int64));
                    *pv = m_pData->readInt64();
                    *(Int64**)pValue = pv;
                }
                else {
                    *(Int64**)pValue = NULL;
                }
            }
            break;

        case Type_InterfacePtrPtr:
            {
                Int32 tag = (Int32)m_pData->readInt32();
                if (tag != MSH_NULL) {
                    tag = (Int32)m_pData->readInt32();
                    if (tag != MSH_NULL) {
                        InterfacePack ipack;
                        CIClassInfo *pClassInfo = NULL;
                        IParcelable *pParcelable = NULL;
                        ClassID classId;
                        InterfaceID iid;
                        ECode ec;

                        android::sp<android::IBinder> binder = m_pData->readStrongBinder();
                        m_pData->read((void *)&ipack, sizeof(InterfacePack));
                        ipack.m_pBinder = binder;
                        if (IsParcelable(&ipack, &pClassInfo)) {
                            classId.clsid = ipack.m_clsid;
                            classId.pUunm = pClassInfo->pszUunm;
                            iid = pClassInfo->interfaces[ipack.m_uIndex]->iid;
                            ec = _CObject_CreateInstance(classId, RGM_SAME_DOMAIN,
                                    EIID_IParcelable, (IInterface**)&pParcelable);
                            if (FAILED(ec)) return ec;

                            ((IParcelable*)pParcelable)->ReadFromParcel(this);
                            *((IInterface**)&pParcelable) = ((IParcelable*)pParcelable)->Probe(iid);
                        }
                        else {
                            ec = StdUnmarshalInterface(
                                    UnmarshalFlag_Noncoexisting,
                                    &ipack,
                                    (IInterface **)pParcelable);
                            if (FAILED(ec)) {
                                MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                                        "MshProc: unmsh interface, ec = %x\n", ec));
                                return ec;
                            }
                        }
                        *(UInt32*)pValue = (UInt32)pParcelable;
                    }
                    else {
                        *(UInt32*)pValue = NULL;
                    }
                }
                else {
                    *(IInterface**)pValue = NULL;
                }
            }
            break;

        case Type_Fd:
            *(Int32*)pValue = (Int32)m_pData->readFileDescriptor();
            break;

        default:
            assert(0);
            break;
    }

    return NOERROR;
}

ECode CRemoteParcel::WriteValue(PVoid pValue, Int32 type, Int32 size)
{
    ECode ec;

    switch(type) {
        case Type_Byte:
        case Type_Boolean:
        case Type_Char8:
            m_pData->writeInt32(*((Byte*)pValue));
            break;

        case Type_Char16:
        case Type_Int16:
            m_pData->writeInt32(*((Int16*)pValue));
            break;

        case Type_Int32:
            m_pData->writeInt32(*((Int32*)pValue));
            break;

        case Type_Float:
            m_pData->writeFloat(*((Float*)pValue));
            break;

        case Type_Int64:
            m_pData->writeInt64(*((Int64*)pValue));
            break;

        case Type_Double:
            m_pData->writeDouble(*((Double*)pValue));
            break;

        case Type_CString:
            m_pData->writeInt32(pValue ? MSH_NOT_NULL : MSH_NULL);

            if (pValue) {
                m_pData->writeCString((const char*)pValue);
            }
            break;

        case Type_String:
            m_pData->writeInt32(pValue ? MSH_NOT_NULL : MSH_NULL);

            if (pValue) {
                m_pData->writeCString((const char*)pValue);
            }
            break;

        case Type_InterfacePtr:
            m_pData->writeInt32(pValue ? MSH_NOT_NULL : MSH_NULL);

            if (pValue) {
                InterfacePack itfPack;
                ec = StdMarshalInterface(
                        (IInterface*)pValue,
                        &itfPack);
                if (FAILED(ec)) return ec;

                m_pData->writeStrongBinder(itfPack.m_pBinder);
                itfPack.m_pBinder = NULL;
                m_pData->write((const void *)&itfPack, (int32_t)sizeof(InterfacePack));

                IParcelable *pParcelable = \
                        (IParcelable*)((IInterface*)pValue)->Probe(EIID_IParcelable);
                if (pParcelable != NULL) pParcelable->WriteToParcel(this);
            }
            break;

        case Type_BytePtr:
        case Type_BooleanPtr:
        case Type_Char8Ptr:
        case Type_Char16Ptr:
        case Type_Int16Ptr:
        case Type_Int32Ptr:
        case Type_FloatPtr:
            m_pData->writeInt32(pValue ? MSH_NOT_NULL : MSH_NULL);

            if (pValue) {
                m_pData->writeInt32(*(UInt32*)pValue);
            }
            break;

        case Type_Int64Ptr:
        case Type_DoublePtr:
            m_pData->writeInt32(pValue ? MSH_NOT_NULL : MSH_NULL);

            if (pValue) {
                m_pData->writeInt64(*(Int64*)pValue);
            }
            break;

        case Type_StringPtr:
            m_pData->writeInt32(pValue ? MSH_NOT_NULL : MSH_NULL);

            if (pValue) {
                m_pData->writeCString((const char*)pValue);
            }
            break;

        case Type_InterfacePtrPtr:
            m_pData->writeInt32(pValue ? MSH_NOT_NULL : MSH_NULL);

            if (*(IInterface**)pValue) {
                InterfacePack itfPack;
                ec = StdMarshalInterface(
                        *(IInterface**)pValue,
                        &itfPack);
                if (FAILED(ec)) return ec;

                m_pData->writeInt32(MSH_NOT_NULL);
                m_pData->writeStrongBinder(itfPack.m_pBinder);
                itfPack.m_pBinder = NULL;
                m_pData->write((void*)&itfPack, sizeof(itfPack));

                IParcelable *pParcelable = \
                        (IParcelable*)(*(IInterface**)pValue)->Probe(EIID_IParcelable);
                if (pParcelable != NULL) pParcelable->WriteToParcel(this);
            }
            else {
                m_pData->writeInt32(MSH_NULL);
            }
            break;

        case Type_Struct:
            m_pData->writeInt32(pValue ? MSH_NOT_NULL : MSH_NULL);

            if (pValue) {
                m_pData->writeInt32(size);
                m_pData->write(pValue, size);
            }
            break;

        case Type_EMuid:
            m_pData->write(pValue, sizeof(EMuid));

            break;

        case Type_EGuid:
            {
                m_pData->write(pValue, sizeof(EGuid));
                m_pData->writeCString(((EGuid *)pValue)->pUunm);
            }
            break;

        case Type_EMuidPtr:
            m_pData->writeInt32(pValue ? MSH_NOT_NULL : MSH_NULL);

            if (pValue) {
                m_pData->write(pValue, sizeof(EMuid));
            }
            break;

        case Type_EGuidPtr:
            m_pData->writeInt32(pValue ? MSH_NOT_NULL : MSH_NULL);

            if (pValue) {
                m_pData->write(pValue, sizeof(EGuid));
                m_pData->writeCString(((EGuid *)pValue)->pUunm);
            }
            break;

        case Type_ArrayOf:
        case Type_BufferOf:
        case Type_MemoryBuf:
        case Type_StringBuf:
            m_pData->writeInt32(pValue ? MSH_NOT_NULL : MSH_NULL);

            if (pValue) {
                m_pData->write(pValue, sizeof(CarQuintet));

                if (CarQuintetFlag_Type_IObject
                    != (((PCARQUINTET)pValue)->m_flags
                            & CarQuintetFlag_TypeMask)) {
                    // copy the storaged data
                    //
                    m_pData->write(((PCARQUINTET)pValue)->m_pBuf,
                            ((PCARQUINTET)pValue)->m_size);
                }
                else {
                    Int32 used = ((PCARQUINTET)pValue)->m_used
                                / sizeof(IInterface *);
                    Int32 *pBuf = (Int32*)((PCARQUINTET)pValue)->m_pBuf;
                    for (Int32 i = 0; i < used; i++) {
                        if (pBuf[i]) {
                            m_pData->writeInt32(MSH_NOT_NULL);

                            InterfacePack itfPack;
                            ec = StdMarshalInterface(
                                    (IInterface *)pBuf[i],
                                    &itfPack);
                            if (FAILED(ec)) {
                                MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                                        "MshProc: marshal interface, ec = %x\n", ec));
                                return ec;
                            }

                            m_pData->writeStrongBinder(itfPack.m_pBinder);
                            itfPack.m_pBinder = NULL;
                            m_pData->write((void*)&itfPack, sizeof(itfPack));

                            IParcelable *pParcelable = \
                                    (IParcelable*)((IInterface*)pBuf[i])->Probe(EIID_IParcelable);
                            if (pParcelable != NULL) pParcelable->WriteToParcel(this);
                        }
                        else {  // null pointer
                            m_pData->writeInt32(MSH_NULL);
                        }
                    }
                }
            }
            break;

        case Type_ArrayOfString:
        case Type_BufferOfString:
            assert(0);
            break;

        case Type_Fd:
            m_pData->writeFileDescriptor(*((Int32*)pValue));
            break;

        case Type_Fd_Dup:
            m_pData->writeDupFileDescriptor(*((Int32*)pValue));
            break;

        default:
            assert(0);
            break;
    }

    return NOERROR;
}

ECode CRemoteParcel::ReadByte(
    /* [out] */ Byte *pValue)
{
    if (pValue == NULL) return E_INVALID_ARGUMENT;

    return ReadValue((PVoid)pValue, Type_Byte);
}

ECode CRemoteParcel::ReadBoolean(
    /* [out] */ Boolean *pValue)
{
    if (pValue == NULL) return E_INVALID_ARGUMENT;

    return ReadValue((PVoid)pValue, Type_Boolean);
}

ECode CRemoteParcel::ReadChar8(
    /* [out] */ Char8 *pValue)
{
    if (pValue == NULL) return E_INVALID_ARGUMENT;

    return ReadValue((PVoid)pValue, Type_Char8);
}

ECode CRemoteParcel::ReadChar16(
    /* [out] */ Char16 *pValue)
{
    if (pValue == NULL) return E_INVALID_ARGUMENT;

    return ReadValue((PVoid)pValue, Type_Char16);
}

ECode CRemoteParcel::ReadInt16(
    /* [out] */ Int16 *pValue)
{
    if (pValue == NULL) return E_INVALID_ARGUMENT;

    return ReadValue((PVoid)pValue, Type_Int16);
}

ECode CRemoteParcel::ReadInt32(
    /* [out] */ Int32 *pValue)
{
    if (pValue == NULL) return E_INVALID_ARGUMENT;

    return ReadValue((PVoid)pValue, Type_Int32);
}

ECode CRemoteParcel::ReadInt64(
    /* [out] */ Int64 *pValue)
{
    if (pValue == NULL) return E_INVALID_ARGUMENT;

    return ReadValue((PVoid)pValue, Type_Int64);
}

ECode CRemoteParcel::ReadFloat(
    /* [out] */ Float *pValue)
{
    if (pValue == NULL) return E_INVALID_ARGUMENT;

    return ReadValue((PVoid)pValue, Type_Float);
}

ECode CRemoteParcel::ReadDouble(
    /* [out] */ Double *pValue)
{
    if (pValue == NULL) return E_INVALID_ARGUMENT;

    return ReadValue((PVoid)pValue, Type_Double);
}

ECode CRemoteParcel::ReadCString(
    /* [out] */ CString* pStr)
{
    if (pStr == NULL) return E_INVALID_ARGUMENT;

    return ReadValue((PVoid)pStr, Type_CString);
}

ECode CRemoteParcel::ReadString(
    /* [out] */ String* pStr)
{
    if (pStr == NULL) return E_INVALID_ARGUMENT;

    return ReadValue((PVoid)pStr, Type_String);
}

ECode CRemoteParcel::ReadStruct(
    /* [out] */ Handle32 *paddr)
{
    return ReadValue((PVoid)paddr, Type_Struct);
}

ECode CRemoteParcel::ReadEMuid(EMuid *pId)
{
    return ReadValue((PVoid)pId, Type_EMuid);
}

ECode CRemoteParcel::ReadEGuid(EGuid *pId)
{
    return ReadValue((PVoid)pId, Type_EGuid);
}

ECode CRemoteParcel::ReadInterfacePtr(
    /* [out] */ Handle32 *pItfPtr)
{
    assert(pItfPtr != NULL);

    return ReadValue((PVoid)pItfPtr, Type_InterfacePtr);
}

ECode CRemoteParcel::ReadArrayOf(
    /* [out] */ Handle32 *ppArray)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::ReadArrayOfCString(
    /* [out] */ Handle32 *ppArray)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::ReadArrayOfString(
    /* [out] */ Handle32 *ppArray)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::ReadBufferOf(
    /* [out] */ Handle32 *ppBuffer)
{
    return ReadValue((PVoid)ppBuffer, Type_BufferOf);
}

ECode CRemoteParcel::ReadBufferOfCString(
    /* [out] */ Handle32 *ppBuffer)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::ReadBufferOfString(
    /* [out] */ Handle32 *ppBuffer)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::ReadMemoryBuf(
    /* [out] */ Handle32 *ppBuffer)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::ReadStringBuf(
    /* [out] */ Handle32 *ppBuffer)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::ReadBytePtr(
    /* [out] */ Handle32 *bypp)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::ReadBooleanPtr(
    /* [out] */ Handle32 *bopp)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::ReadChar8Ptr(
    /* [out] */ Handle32 *c8pp)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::ReadChar16Ptr(
    /* [out] */ Handle32 *c16pp)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::ReadInt16Ptr(
    /* [out] */ Handle32 *i16pp)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::ReadInt32Ptr(
    /* [out] */ Handle32 *i32pp)
{
    return ReadValue((PVoid)i32pp, Type_Int32Ptr);
}

ECode CRemoteParcel::ReadInt64Ptr(
    /* [out] */ Handle32 *i64pp)
{
    return ReadValue((PVoid)i64pp, Type_Int64Ptr);
}

ECode CRemoteParcel::ReadFloatPtr(
    /* [out] */ Handle32 *fpp)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::ReadDoublePtr(
    /* [out] */ Handle32 *dpp)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::ReadStringPtr(
    /* [out] */ Handle32 *spp)
{
    if (spp == NULL) return E_INVALID_ARGUMENT;

    return ReadValue((PVoid)spp, Type_StringPtr);
}

ECode CRemoteParcel::ReadStructPtr(
    /* [out] */ Handle32 *paddr)
{
    return ReadValue((PVoid)paddr, Type_StructPtr);
}

ECode CRemoteParcel::ReadEMuidPtr(
    /* [out] */ Handle32 *idpp)
{
    return ReadValue((PVoid)idpp, Type_EMuidPtr);
}

ECode CRemoteParcel::ReadEGuidPtr(
    /* [out] */ Handle32 *idpp)
{
    return ReadValue((PVoid)idpp, Type_EGuidPtr);
}

ECode CRemoteParcel::ReadInterfacePtrPtr(
    /* [out] */ Handle32 *itfpp)
{
    return ReadValue((PVoid)itfpp, Type_InterfacePtrPtr);
}

ECode CRemoteParcel::ReadBufferOfPtr(
    /* [out] */ Handle32 *ppBuffer)
{
    return ReadValue((PVoid)ppBuffer, Type_BufferOfPtr);
}

// Retrieve a file descriptor from the parcel.  This returns the raw fd
// in the parcel, which you do not own -- use dup() to get your own copy.
ECode CRemoteParcel::ReadFileDescriptor(
    /* [out] */ Int32* fd)
{
    if (fd == NULL) return E_INVALID_ARGUMENT;

    return ReadValue((PVoid)fd, Type_Fd);
}

ECode CRemoteParcel::WriteByte(Byte value)
{
    return WriteValue((PVoid)&value, Type_Byte, 4);
}

ECode CRemoteParcel::WriteBoolean(
    /* [in] */ Boolean value)
{
    return WriteValue((PVoid)&value, Type_Boolean, 4);
}

ECode CRemoteParcel::WriteChar8(
    /* [in] */ Char8 value)
{
    return WriteValue((PVoid)&value, Type_Char8, 4);
}

ECode CRemoteParcel::WriteChar16(
    /* [in] */ Char16 value)
{
    return WriteValue((PVoid)&value, Type_Char16, 4);
}

ECode CRemoteParcel::WriteInt16(Int16 value)
{
    return WriteValue((PVoid)&value, Type_Int16, 4);
}

ECode CRemoteParcel::WriteInt32(Int32 value)
{
    return WriteValue((PVoid)&value, Type_Int32, 4);
}

ECode CRemoteParcel::WriteInt64(
    /* [in] */ Int64 value)
{
    return WriteValue((PVoid)&value, Type_Int64, 8);
}

ECode CRemoteParcel::WriteFloat(
    /* [in] */ Float value)
{
    return WriteValue((PVoid)&value, Type_Float, 4);
}

ECode CRemoteParcel::WriteDouble(
    /* [in] */ Double value)
{
    return WriteValue((PVoid)&value, Type_Double, 8);
}

ECode CRemoteParcel::WriteCString(CString str)
{
    Int32 size = sizeof(UInt32);

    if (!str.IsNull()) {
        size += MSH_ALIGN_4(strlen((const char*)(str)) + 1) + sizeof(UInt32);
    }

    return WriteValue((PVoid)(const char*)(str), Type_CString, size);
}

ECode CRemoteParcel::WriteString(const String& str)
{
    Int32 size = sizeof(UInt32);

    if (!str.IsNull()) {
        size += MSH_ALIGN_4(str.GetLength()+ 1) + sizeof(UInt32);
    }

    return WriteValue((PVoid)str.string(), Type_String, size);
}

ECode CRemoteParcel::WriteInterfacePtr(IInterface* pValue)
{
    return WriteValue((PVoid)pValue, Type_InterfacePtr, sizeof(UInt32) + sizeof(InterfacePack));
}

ECode CRemoteParcel::WriteStruct(Handle32 pValue, Int32 size)
{
    return WriteValue((PVoid)pValue, Type_Struct, size + sizeof(UInt32));
}

ECode CRemoteParcel::WriteEMuid(const EMuid& id)
{
    return WriteValue((PVoid)&id, Type_EMuid, sizeof(EMuid));
}

ECode CRemoteParcel::WriteEGuid(const EGuid& id)
{
    return WriteValue((PVoid)&id, Type_EGuid, sizeof(EGuid) +
                      MSH_ALIGN_4(strlen(id.pUunm) + 1) + sizeof(UInt32) * 2);
}

ECode CRemoteParcel::WriteArrayOf(
    /* [in] */ Handle32 pArray)
{
    Int32 size = sizeof(UInt32) + sizeof(CarQuintet) + ((CarQuintet*)pArray)->m_size;
    return WriteValue((PVoid)pArray, Type_ArrayOf, size);
}

ECode CRemoteParcel::WriteArrayOfCString(
    /* [in] */ const ArrayOf<CString> & array)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::WriteArrayOfString(
    /* [in] */ const ArrayOf<String> & array)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::WriteBufferOf(
    /* [in] */ Handle32 pBuffer)
{
    Int32 size = sizeof(UInt32) + sizeof(CarQuintet) + ((CarQuintet*)pBuffer)->m_size;
    return WriteValue((PVoid)pBuffer, Type_BufferOf, size);
}

ECode CRemoteParcel::WriteBufferOfCString(
    /* [in] */ const BufferOf<CString> & pBuffer)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::WriteBufferOfString(
    /* [in] */ const BufferOf<String> & pBuffer)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::WriteMemoryBuf(
    /* [in] */ Handle32 pBuffer)
{
    Int32 size = sizeof(UInt32) + sizeof(CarQuintet) + ((CarQuintet*)pBuffer)->m_size;
    return WriteValue((PVoid)pBuffer, Type_MemoryBuf, size);
}

ECode CRemoteParcel::WriteStringBuf(
    /* [in] */ Handle32 pBuffer)
{
    Int32 size = sizeof(UInt32) + sizeof(CarQuintet) + ((CarQuintet*)pBuffer)->m_size;
    return WriteValue((PVoid)pBuffer, Type_StringBuf, size);
}

ECode CRemoteParcel::WriteBytePtr(
    /* [in] */ Handle32 addr)
{
    return WriteValue((PVoid)addr, Type_BytePtr, sizeof(Byte*));
}

ECode CRemoteParcel::WriteBooleanPtr(
    /* [in] */ Handle32 addr)
{
    return WriteValue((PVoid)addr, Type_BooleanPtr, sizeof(Boolean*));
}

ECode CRemoteParcel::WriteChar8Ptr(
    /* [in] */ Handle32 addr)
{
    return WriteValue((PVoid)addr, Type_Char8Ptr, sizeof(Char8*));
}

ECode CRemoteParcel::WriteChar16Ptr(
    /* [in] */ Handle32 addr)
{
    return WriteValue((PVoid)addr, Type_Char16Ptr, sizeof(Char16*));
}

ECode CRemoteParcel::WriteInt16Ptr(
    /* [in] */ Handle32 addr)
{
    return WriteValue((PVoid)addr, Type_Int16Ptr, sizeof(Int16*));
}

ECode CRemoteParcel::WriteInt32Ptr(
    /* [in] */ Handle32 addr)
{
    return WriteValue((PVoid)addr, Type_Int32Ptr, sizeof(Int32*) + sizeof(Int32));
}

ECode CRemoteParcel::WriteInt64Ptr(
    /* [in] */ Handle32 addr)
{
    return WriteValue((PVoid)addr, Type_Int64Ptr, sizeof(Int64*) + sizeof(Int64));
}

ECode CRemoteParcel::WriteFloatPtr(
    /* [in] */ Handle32 addr)
{
    return WriteValue((PVoid)addr, Type_FloatPtr, sizeof(float*));
}

ECode CRemoteParcel::WriteDoublePtr(
    /* [in] */ Handle32 addr)
{
    return WriteValue((PVoid)addr, Type_DoublePtr, sizeof(double*));
}

ECode CRemoteParcel::WriteStringPtr(
    /* [in] */ Handle32 addr)
{
    return WriteValue((PVoid)addr, Type_StringPtr, sizeof(String*));
}

ECode CRemoteParcel::WriteEMuidPtr(
    /* [in] */ Handle32 idPtr)
{
    Int32 size = sizeof(UInt32*);

    if (idPtr) size += sizeof(EMuid);

    return WriteValue((PVoid)idPtr, Type_EMuidPtr, size);
}

ECode CRemoteParcel::WriteEGuidPtr(
    /* [in] */ Handle32 idPtr)
{
    Int32 size = sizeof(UInt32*);

    if (idPtr) size += sizeof(EMuid) + MSH_ALIGN_4(strlen(((EGuid*)idPtr)->pUunm) + 1) +
                       sizeof(UInt32) * 2;

    return WriteValue((PVoid)idPtr, Type_EGuidPtr, size);
}

ECode CRemoteParcel::WriteInterfacePtrPtr(
    /* [in] */ Handle32 itfpp)
{
    Int32 size = sizeof(UInt32);

    if (itfpp) {
        if (*(IInterface**)itfpp) size += sizeof(InterfacePack);
        else size += sizeof(UInt32);
    }

    return WriteValue((PVoid)itfpp, Type_InterfacePtrPtr, size);
}

// Place a file descriptor into the parcel.  The given fd must remain
// valid for the lifetime of the parcel.
ECode CRemoteParcel::WriteFileDescriptor(
    /* [in] */ Int32 fd)
{
    return WriteValue((PVoid)&fd, Type_Fd, 4);
}

// Place a file descriptor into the parcel.  A dup of the fd is made, which
// will be closed once the parcel is destroyed.
ECode CRemoteParcel::WriteDupFileDescriptor(
    /* [in] */ Int32 fd)
{
    return WriteValue((PVoid)&fd, Type_Fd_Dup, 4);
}

ECode CRemoteParcel::Clone(
    /* [in] */ IParcel* pSrcParcel)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::GetDataPosition(
    /* [out] */ Int32* position)
{
    if (position == NULL) return E_NOT_IMPLEMENTED;

    *position = m_pData->dataPosition();
    return NOERROR;
}

ECode CRemoteParcel::SetDataPosition(
    /* [in] */ Int32 position)
{
    m_pData->setDataPosition(position);
    return NOERROR;
}

ECode CRemoteParcel::GetElementPayload(
    /* [out] */ Handle32* pBuffer)
{
    *pBuffer = (Handle32)m_pData;

    return NOERROR;
}

ECode CRemoteParcel::GetElementSize(
    /* [in] */ Int32* pSize)
{
    *pSize = (Int32)m_pData->ipcDataSize();

    return NOERROR;
}

MarshalHeader* CRemoteParcel::GetMarshalHeader()
{
    return (MarshalHeader*)m_pData->data();
}

Boolean CRemoteParcel::IsParcelable(
    /* [in] */ InterfacePack *pInterfacePack,
    /* [out] */ CIClassInfo **ppClassInfo)
{
    CIInterfaceInfo *pInterfaceInfo = NULL;
    ECode ec;

    ec = LookupClassInfo(pInterfacePack->m_clsid, ppClassInfo);
    if (FAILED(ec)) {
        ec = GetRemoteClassInfo(pInterfacePack->m_pBinder.get(),
                                pInterfacePack->m_clsid,
                                ppClassInfo);
        if (FAILED(ec)) return FALSE;
    }

    for (UInt16 i = 0; i < (*ppClassInfo)->interfaceNum; i++) {
        pInterfaceInfo = (*ppClassInfo)->interfaces[i];
        if (pInterfaceInfo->iid == EIID_IParcelable) {
            return TRUE;
        }
    }

    return FALSE;
}
