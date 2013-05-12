
#include "os/CParcel.h"
#include "ext/frameworkdef.h"


#define MSH_NULL        ((UInt32)0)
#define MSH_NOT_NULL    ((UInt32)-2)

#define MSH_ALIGN_4(n)  RoundUp2((n), 4)

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

CParcel::CParcel()
    : mData(new android::Parcel())
{}

CParcel::~CParcel()
{
    delete mData;
}

ECode CParcel::constructor()
{
    return NOERROR;
}

ECode CParcel::ReadByte(
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value);

    return ReadValue((PVoid)value, Type_Byte);
}

ECode CParcel::ReadBoolean(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);

    return ReadValue((PVoid)value, Type_Boolean);
}

ECode CParcel::ReadChar8(
    /* [out] */ Char8* value)
{
    VALIDATE_NOT_NULL(value);

    return ReadValue((PVoid)value, Type_Char8);
}

ECode CParcel::ReadChar16(
    /* [out] */ Char16* value)
{
    VALIDATE_NOT_NULL(value);

    return ReadValue((PVoid)value, Type_Char16);
}

ECode CParcel::ReadInt16(
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);

    return ReadValue((PVoid)value, Type_Int16);
}

ECode CParcel::ReadInt32(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return ReadValue((PVoid)value, Type_Int32);
}

ECode CParcel::ReadInt64(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    return ReadValue((PVoid)value, Type_Int64);
}

ECode CParcel::ReadFloat(
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);

    return ReadValue((PVoid)value, Type_Float);
}

ECode CParcel::ReadDouble(
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);

    return ReadValue((PVoid)value, Type_Double);
}

ECode CParcel::ReadCString(
    /* [out] */ CString* str)
{
    VALIDATE_NOT_NULL(str);

    return ReadValue((PVoid)str, Type_CString);
}

ECode CParcel::ReadString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    return ReadValue((PVoid)str, Type_String);
}

ECode CParcel::ReadStruct(
    /* [out] */ Handle32* addr)
{
    VALIDATE_NOT_NULL(addr);

    return ReadValue((PVoid)addr, Type_Struct);
}

ECode CParcel::ReadEMuid(
    /* [out] */ EMuid* id)
{
    VALIDATE_NOT_NULL(id);

    return ReadValue((PVoid)id, Type_EMuid);
}

ECode CParcel::ReadEGuid(
    /* [out] */ EGuid* id)
{
    VALIDATE_NOT_NULL(id);

    return ReadValue((PVoid)id, Type_EGuid);
}

ECode CParcel::ReadInterfacePtr(
    /* [out] */ Handle32* itfPtr)
{
    VALIDATE_NOT_NULL(itfPtr);

    return ReadValue((PVoid)itfPtr, Type_InterfacePtr);
}

ECode CParcel::ReadArrayOf(
    /* [out] */ Handle32* array)
{
    return E_NOT_IMPLEMENTED;
}

ECode CParcel::ReadArrayOfCString(
    /* [out, callee] */ ArrayOf<CString>** array)
{
    return E_NOT_IMPLEMENTED;
}

ECode CParcel::ReadArrayOfString(
    /* [out, callee] */ ArrayOf<String>** array)
{
    VALIDATE_NOT_NULL(array);

    return ReadValue((PVoid)array, Type_ArrayOfString);
}

ECode CParcel::ReadBufferOf(
    /* [out] */ Handle32* buffer)
{
    VALIDATE_NOT_NULL(buffer);

    return ReadValue((PVoid)buffer, Type_BufferOf);
}

ECode CParcel::ReadBufferOfCString(
    /* [out] */ Handle32* buffer)
{
    return E_NOT_IMPLEMENTED;
}

ECode CParcel::ReadBufferOfString(
    /* [out] */ Handle32* buffer)
{
    return E_NOT_IMPLEMENTED;
}

ECode CParcel::ReadMemoryBuf(
    /* [out] */ Handle32* buffer)
{
    return E_NOT_IMPLEMENTED;
}

ECode CParcel::ReadStringBuf(
    /* [out] */ Handle32* buffer)
{
    return E_NOT_IMPLEMENTED;
}

ECode CParcel::ReadBytePtr(
    /* [out] */ Handle32* bypp)
{
    return E_NOT_IMPLEMENTED;
}

ECode CParcel::ReadBooleanPtr(
    /* [out] */ Handle32* bopp)
{
    return E_NOT_IMPLEMENTED;
}

ECode CParcel::ReadChar8Ptr(
    /* [out] */ Handle32* c8pp)
{
    return E_NOT_IMPLEMENTED;
}

ECode CParcel::ReadChar16Ptr(
    /* [out] */ Handle32* c16pp)
{
    return E_NOT_IMPLEMENTED;
}

ECode CParcel::ReadInt16Ptr(
    /* [out] */ Handle32* i16pp)
{
    return E_NOT_IMPLEMENTED;
}

ECode CParcel::ReadInt32Ptr(
    /* [out] */ Handle32* i32pp)
{
    VALIDATE_NOT_NULL(i32pp);

    return ReadValue((PVoid)i32pp, Type_Int32Ptr);
}

ECode CParcel::ReadInt64Ptr(
    /* [out] */ Handle32* i64pp)
{
    VALIDATE_NOT_NULL(i64pp);

    return ReadValue((PVoid)i64pp, Type_Int64Ptr);
}

ECode CParcel::ReadFloatPtr(
    /* [out] */ Handle32* fpp)
{
    return E_NOT_IMPLEMENTED;
}

ECode CParcel::ReadDoublePtr(
    /* [out] */ Handle32* dpp)
{
    return E_NOT_IMPLEMENTED;
}

ECode CParcel::ReadStringPtr(
    /* [out] */ Handle32* spp)
{
    VALIDATE_NOT_NULL(spp);

    return ReadValue((PVoid)spp, Type_StringPtr);
}

ECode CParcel::ReadStructPtr(
    /* [out] */ Handle32* addr)
{
    VALIDATE_NOT_NULL(addr);

    return ReadValue((PVoid)addr, Type_StructPtr);
}

ECode CParcel::ReadEMuidPtr(
    /* [out] */ Handle32* idpp)
{
    VALIDATE_NOT_NULL(idpp);

    return ReadValue((PVoid)idpp, Type_EMuidPtr);
}

ECode CParcel::ReadEGuidPtr(
    /* [out] */ Handle32* idpp)
{
    VALIDATE_NOT_NULL(idpp);

    return ReadValue((PVoid)idpp, Type_EGuidPtr);
}

ECode CParcel::ReadInterfacePtrPtr(
    /* [out] */ Handle32* itfpp)
{
    VALIDATE_NOT_NULL(itfpp);

    return ReadValue((PVoid)itfpp, Type_InterfacePtrPtr);
}

ECode CParcel::ReadBufferOfPtr(
    /* [out] */ Handle32* buffer)
{
    VALIDATE_NOT_NULL(buffer);

    return ReadValue((PVoid)buffer, Type_BufferOfPtr);
}

// Retrieve a file descriptor from the parcel.  This returns the raw fd
// in the parcel, which you do not own -- use dup() to get your own copy.
ECode CParcel::ReadFileDescriptor(
    /* [out] */ Int32* fd)
{
    VALIDATE_NOT_NULL(fd);

    return ReadValue((PVoid)fd, Type_Fd);
}

ECode CParcel::WriteByte(
    /* [in] */ Byte value)
{
    return WriteValue((PVoid)&value, Type_Byte, 4);
}

ECode CParcel::WriteBoolean(
    /* [in] */ Boolean value)
{
    return WriteValue((PVoid)&value, Type_Boolean, 4);
}

ECode CParcel::WriteChar8(
    /* [in] */ Char8 value)
{
    return WriteValue((PVoid)&value, Type_Char8, 4);
}

ECode CParcel::WriteChar16(
    /* [in] */ Char16 value)
{
    return WriteValue((PVoid)&value, Type_Char16, 4);
}

ECode CParcel::WriteInt16(
    /* [in] */ Int16 value)
{
    return WriteValue((PVoid)&value, Type_Int16, 4);
}

ECode CParcel::WriteInt32(
    /* [in] */ Int32 value)
{
    return WriteValue((PVoid)&value, Type_Int32, 4);
}

ECode CParcel::WriteInt64(
    /* [in] */ Int64 value)
{
    return WriteValue((PVoid)&value, Type_Int64, 8);
}

ECode CParcel::WriteFloat(
    /* [in] */ Float value)
{
    return WriteValue((PVoid)&value, Type_Float, 4);
}

ECode CParcel::WriteDouble(
    /* [in] */ Double value)
{
    return WriteValue((PVoid)&value, Type_Double, 8);
}

ECode CParcel::WriteCString(
    /* [in] */ CString str)
{
    // Int32 size = sizeof(UInt32);

    // if (!str.IsNull()) {
    //     size += MSH_ALIGN_4(strlen((const char*)(str)) + 1) + sizeof(UInt32);
    // }

    // return WriteValue((PVoid)(const char*)(str), Type_CString, size);
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CParcel::WriteString(
    /* [in] */ const String& str)
{
    Int32 size = sizeof(UInt32);

    if (!str.IsNull()) {
        size += MSH_ALIGN_4(str.GetLength()+ 1) + sizeof(UInt32);
    }

    return WriteValue((PVoid)str.string(), Type_String, size);
}

ECode CParcel::WriteInterfacePtr(
    /* [in] */ IInterface* value)
{
    // return WriteValue((PVoid)value, Type_InterfacePtr, sizeof(UInt32) + sizeof(InterfacePack));
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CParcel::WriteStruct(
    /* [in] */ Handle32 value,
    /* [in] */ Int32 size)
{
    return WriteValue((PVoid)value, Type_Struct, size + sizeof(UInt32));
}

ECode CParcel::WriteEMuid(
    /* [in] */ const EMuid& id)
{
    return WriteValue((PVoid)&id, Type_EMuid, sizeof(EMuid));
}

ECode CParcel::WriteEGuid(
    /* [in] */ const EGuid& id)
{
    // return WriteValue((PVoid)&id, Type_EGuid, sizeof(EGuid) +
    //                   MSH_ALIGN_4(strlen(id.pUunm) + 1) + sizeof(UInt32) * 2);
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CParcel::WriteArrayOf(
    /* [in] */ Handle32 array)
{
    Int32 size = sizeof(UInt32) + sizeof(CarQuintet) + ((CarQuintet*)array)->m_size;
    return WriteValue((PVoid)array, Type_ArrayOf, size);
}

ECode CParcel::WriteArrayOfCString(
    /* [in] */ const ArrayOf<CString>& array)
{
    return E_NOT_IMPLEMENTED;
}

ECode CParcel::WriteArrayOfString(
    /* [in] */ const ArrayOf<String>& array)
{
    Int32 size = sizeof(UInt32) + sizeof(CarQuintet) + array.m_size;
    return WriteValue((PVoid)&array, Type_ArrayOfString, size);
}

ECode CParcel::WriteBufferOf(
    /* [in] */ Handle32 buffer)
{
    Int32 size = sizeof(UInt32) + sizeof(CarQuintet) + ((CarQuintet*)buffer)->m_size;
    return WriteValue((PVoid)buffer, Type_BufferOf, size);
}

ECode CParcel::WriteBufferOfCString(
    /* [in] */ const BufferOf<CString>& buffer)
{
    return E_NOT_IMPLEMENTED;
}

ECode CParcel::WriteBufferOfString(
    /* [in] */ const BufferOf<String>& buffer)
{
    return E_NOT_IMPLEMENTED;
}

ECode CParcel::WriteMemoryBuf(
    /* [in] */ Handle32 buffer)
{
    Int32 size = sizeof(UInt32) + sizeof(CarQuintet) + ((CarQuintet*)buffer)->m_size;
    return WriteValue((PVoid)buffer, Type_MemoryBuf, size);
}

ECode CParcel::WriteStringBuf(
    /* [in] */ Handle32 buffer)
{
    Int32 size = sizeof(UInt32) + sizeof(CarQuintet) + ((CarQuintet*)buffer)->m_size;
    return WriteValue((PVoid)buffer, Type_StringBuf, size);
}

ECode CParcel::WriteBytePtr(
    /* [in] */ Handle32 addr)
{
    return WriteValue((PVoid)addr, Type_BytePtr, sizeof(Byte*));
}

ECode CParcel::WriteBooleanPtr(
    /* [in] */ Handle32 addr)
{
    return WriteValue((PVoid)addr, Type_BooleanPtr, sizeof(Boolean*));
}

ECode CParcel::WriteChar8Ptr(
    /* [in] */ Handle32 addr)
{
    return WriteValue((PVoid)addr, Type_Char8Ptr, sizeof(Char8*));
}

ECode CParcel::WriteChar16Ptr(
    /* [in] */ Handle32 addr)
{
    return WriteValue((PVoid)addr, Type_Char16Ptr, sizeof(Char16*));
}

ECode CParcel::WriteInt16Ptr(
    /* [in] */ Handle32 addr)
{
    return WriteValue((PVoid)addr, Type_Int16Ptr, sizeof(Int16*));
}

ECode CParcel::WriteInt32Ptr(
    /* [in] */ Handle32 addr)
{
    return WriteValue((PVoid)addr, Type_Int32Ptr, sizeof(Int32*) + sizeof(Int32));
}

ECode CParcel::WriteInt64Ptr(
    /* [in] */ Handle32 addr)
{
    return WriteValue((PVoid)addr, Type_Int64Ptr, sizeof(Int64*) + sizeof(Int64));
}

ECode CParcel::WriteFloatPtr(
    /* [in] */ Handle32 addr)
{
    return WriteValue((PVoid)addr, Type_FloatPtr, sizeof(float*));
}

ECode CParcel::WriteDoublePtr(
    /* [in] */ Handle32 addr)
{
    return WriteValue((PVoid)addr, Type_DoublePtr, sizeof(double*));
}

ECode CParcel::WriteStringPtr(
    /* [in] */ Handle32 addr)
{
    return WriteValue((PVoid)addr, Type_StringPtr, sizeof(String*));
}

ECode CParcel::WriteEMuidPtr(
    /* [in] */ Handle32 idPtr)
{
    Int32 size = sizeof(UInt32*);

    if (idPtr) size += sizeof(EMuid);

    return WriteValue((PVoid)idPtr, Type_EMuidPtr, size);
}

ECode CParcel::WriteEGuidPtr(
    /* [in] */ Handle32 idPtr)
{
    // Int32 size = sizeof(UInt32*);

    // if (idPtr) size += sizeof(EMuid) + MSH_ALIGN_4(strlen(((EGuid*)idPtr)->pUunm) + 1) +
    //                    sizeof(UInt32) * 2;

    // return WriteValue((PVoid)idPtr, Type_EGuidPtr, size);
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CParcel::WriteInterfacePtrPtr(
    /* [in] */ Handle32 itfpp)
{
    // Int32 size = sizeof(UInt32);

    // if (itfpp) {
    //     if (*(IInterface**)itfpp) size += sizeof(InterfacePack);
    //     else size += sizeof(UInt32);
    // }

    // return WriteValue((PVoid)itfpp, Type_InterfacePtrPtr, size);
    assert(0);
    return E_NOT_IMPLEMENTED;
}

// Place a file descriptor into the parcel.  The given fd must remain
// valid for the lifetime of the parcel.
ECode CParcel::WriteFileDescriptor(
    /* [in] */ Int32 fd)
{
    return WriteValue((PVoid)&fd, Type_Fd, 4);
}

// Place a file descriptor into the parcel.  A dup of the fd is made, which
// will be closed once the parcel is destroyed.
ECode CParcel::WriteDupFileDescriptor(
    /* [in] */ Int32 fd)
{
    return WriteValue((PVoid)&fd, Type_Fd_Dup, 4);
}

ECode CParcel::Clone(
    /* [in] */ IParcel* srcParcel)
{
    return E_NOT_IMPLEMENTED;
}

ECode CParcel::GetDataPosition(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);

    *position = mData->dataPosition();
    return NOERROR;
}

ECode CParcel::SetDataPosition(
    /* [in] */ Int32 position)
{
    mData->setDataPosition(position);
    return NOERROR;
}

ECode CParcel::GetElementPayload(
    /* [out] */ Handle32* buffer)
{
    VALIDATE_NOT_NULL(buffer);

    *buffer = (Handle32)mData;

    return NOERROR;
}

ECode CParcel::GetElementSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);

    *size = (Int32)mData->ipcDataSize();

    return NOERROR;
}

ECode CParcel::ReadValue(
    /* [in] */ PVoid value,
    /* [in] */ Int32 type)
{
        switch(type) {
        case Type_Byte:
            *(Byte*)value = (Byte)mData->readInt32();
            break;

        case Type_Boolean:
            *(Boolean*)value = (Boolean)mData->readInt32();
            break;

        case Type_Char8:
            *(Char8*)value = (Char8)mData->readInt32();
            break;

        case Type_Char16:
            *(Char16*)value = (Char16)mData->readInt32();
            break;

        case Type_Int16:
            *(Int16*)value = (Int16)mData->readInt32();
            break;

        case Type_Int32:
            *(Int32*)value = (Int32)mData->readInt32();
            break;

        case Type_Int64:
        case Type_Double:
            *(Int64*)value = (Int64)mData->readInt64();
            break;

        case Type_Float:
            *(Float*)value = (Float)mData->readFloat();
            break;

        case Type_CString:
            {
                Int32 tag = mData->readInt32();
                if (tag != MSH_NULL) {
                    const char* str = mData->readCString();
                    *(CString*)value = CString::Duplicate(str);
                }
                else {
                    *(CString*)value = NULL;
                }
            }
            break;

        case Type_String:
            {
                Int32 tag = mData->readInt32();
                if (tag != MSH_NULL) {
                    const char* str = mData->readCString();
                    *(String*)value = str;
                }
                else {
                    *(String*)value = NULL;
                }
            }
            break;

        case Type_StringPtr:
            {
                Int32 tag = mData->readInt32();
                if (tag != MSH_NULL) {
                    const char* cstr = mData->readCString();
                    String* str = new String(cstr);
                    *(String**)value = str;
                }
                else {
                    String* str = new String(NULL);
                    *(String**)value = str;
                }
            }
            break;

        case Type_Struct:
            {
                Int32 len = (Int32)mData->readInt32();

                mData->read(value, len);
            }
            break;

        case Type_StructPtr:
            {
                Int32 tag = (Int32)mData->readInt32();
                if (tag != MSH_NULL) {
                    Int32 len = (Int32)mData->readInt32();
                    *(UInt32**)value = (UInt32*)mData->readInplace(len);
                }
                else {
                    *(UInt32**)value = NULL;
                }
            }
            break;

        case Type_EMuid:
            mData->read(value, sizeof(EMuid));

            break;

        case Type_EGuid:
            {
                const char* str;

                mData->read(value, sizeof(EGuid));
                str = mData->readCString();
                (*(EGuid*)value).pUunm = (char*)malloc(strlen(str) + 1);
                strcpy((*(EGuid*)value).pUunm, str);
            }
            break;

        case Type_EMuidPtr:
            {
                Int32 tag = (Int32)mData->readInt32();
                if (tag != MSH_NULL) {
                    *(EMuid**)value = (EMuid*)mData->readInplace(sizeof(EMuid));
                }
                else {
                    *(EMuid**)value = NULL;
                }
            }
            break;

        case Type_EGuidPtr:
            {
                Int32 tag = (Int32)mData->readInt32();
                if (tag != MSH_NULL) {
                    EGuid* guid = new EGuid();
                    mData->read((void*)guid, sizeof(EGuid));
                    guid->pUunm = (char*)mData->readCString();
                    *(EGuid**)value = guid;
                }
                else {
                    *(EGuid**)value = NULL;
                }
            }
            break;

        case Type_BufferOf:
            {
                Int32 tag = (Int32)mData->readInt32();
                if (tag != MSH_NULL) {
                    CarQuintet buf;

                    mData->read((void*)&buf, sizeof(buf));
                    ((PCARQUINTET)(UInt32*)value)->m_used = buf.m_used;
                    mData->read(((PCARQUINTET)(UInt32*)value)->m_pBuf, \
                        buf.m_size);
                }
            }
            break;

        case Type_BufferOfPtr:
            // {
            //     Int32 tag = (Int32)mData->readInt32();
            //     if (tag != MSH_NULL) {
            //         PCARQUINTET q = new CarQuintet();
            //         *(PCARQUINTET*)pValue = q;
            //         mData->read((void*)q, sizeof(CarQuintet));
            //         Int32 size = (*(PCARQUINTET*)pValue)->m_size;
            //         if (size == 0) {
            //             (*(PCARQUINTET*)pValue)->m_pBuf = NULL;
            //         }
            //         else {
            //             if (CarQuintetFlag_Type_IObject
            //                 != ((*(PCARQUINTET*)pValue)->m_flags
            //                         & CarQuintetFlag_TypeMask)) {
            //                 Byte* buf = (Byte*)malloc(size);
            //                 mData->read((void*)buf, size);
            //                 (*(PCARQUINTET*)pValue)->m_pBuf = buf;
            //             }
            //             else {
            //                 Int32 used = (*(PCARQUINTET*)pValue)->m_used
            //                     / sizeof(IInterface *);
            //                 IInterface** pBuf = (IInterface**)calloc(size, sizeof(IInterface*));
            //                 (*(PCARQUINTET*)pValue)->m_pBuf = pBuf;
            //                 for (int i = 0; i < used; i++) {
            //                     tag = (Int32)mData->readInt32();
            //                     if (tag != MSH_NULL) {
            //                         InterfacePack ipack;
            //                         CIClassInfo *pClassInfo = NULL;
            //                         IParcelable *pParcelable = NULL;
            //                         ClassID classId;
            //                         InterfaceID iid;
            //                         ECode ec;

            //                         android::sp<android::IBinder> binder = mData->readStrongBinder();
            //                         mData->read((void *)&ipack, sizeof(InterfacePack));
            //                         ipack.m_pBinder = binder;
            //                         if (IsParcelable(&ipack, &pClassInfo)) {
            //                             classId.clsid = ipack.m_clsid;
            //                             classId.pUunm = pClassInfo->pszUunm;

            //                             ec = _CObject_CreateInstance(classId, RGM_SAME_DOMAIN,
            //                                     EIID_IParcelable, (IInterface**)&pParcelable);
            //                             if (FAILED(ec)) return ec;

            //                             pParcelable->ReadFromParcel(this);
            //                             iid = pClassInfo->interfaces[ipack.m_uIndex]->iid;
            //                             *((IInterface**)pBuf + i) = pParcelable->Probe(iid);
            //                         }
            //                         else {
            //                             ec = StdUnmarshalInterface(
            //                                     UnmarshalFlag_Noncoexisting,
            //                                     &ipack,
            //                                     (IInterface **)&pBuf[i]);
            //                             if (FAILED(ec)) {
            //                                 MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
            //                                         "MshProc: unmsh interface, ec = %x\n", ec));
            //                                 return ec;
            //                             }
            //                         }
            //                     }
            //                     else {
            //                         pBuf[i] = NULL;
            //                     }
            //                 }
            //             }
            //         }

            //     }
            //     else {
            //         *(UInt32*)pValue = NULL;
            //     }
            // }
            assert(0);
            break;

        case Type_ArrayOfString:
            {
                Int32 tag = (Int32)mData->readInt32();
                if (tag != MSH_NULL) {
                    CarQuintet buf;

                    mData->read((void*)&buf, sizeof(buf));
                    Int32 size = buf.m_size / sizeof(String);
                    *(ArrayOf<String>**)value = ArrayOf<String>::Alloc(size);
                    for (Int32 i = 0; i < size; i++) {
                        Int32 tag = mData->readInt32();
                        if (tag != MSH_NULL) {
                            const char* str = mData->readCString();
                            (**(ArrayOf<String>**)value)[i] = str;
                        }
                    }
                }
            }
            break;

        case Type_InterfacePtr:
            // {
            //     Int32 tag = (Int32)mData->readInt32();
            //     if (tag != MSH_NULL) {
            //         InterfacePack ipack;
            //         CIClassInfo *pClassInfo = NULL;
            //         IParcelable *pParcelable = NULL;
            //         ClassID classId;
            //         InterfaceID iid;
            //         ECode ec;

            //         android::sp<android::IBinder> binder = mData->readStrongBinder();
            //         (InterfacePack *)mData->read((void *)&ipack, sizeof(InterfacePack));
            //         ipack.m_pBinder = binder;
            //         if (IsParcelable(&ipack, &pClassInfo)) {
            //             classId.clsid = ipack.m_clsid;
            //             classId.pUunm = pClassInfo->pszUunm;

            //             ec = _CObject_CreateInstance(classId, RGM_SAME_DOMAIN,
            //                     EIID_IParcelable, (IInterface**)&pParcelable);
            //             if (FAILED(ec)) return ec;

            //             pParcelable->ReadFromParcel(this);
            //             iid = pClassInfo->interfaces[ipack.m_uIndex]->iid;
            //             *(IInterface**)pValue = pParcelable->Probe(iid);
            //         }
            //         else {
            //             ec = StdUnmarshalInterface(
            //                     UnmarshalFlag_Noncoexisting,
            //                     &ipack,
            //                     (IInterface **)pValue);
            //             if (FAILED(ec)) {
            //                 MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
            //                         "MshProc: unmsh interface, ec = %x\n", ec));
            //                 return ec;
            //             }
            //         }
            //     }
            //     else {
            //         *(IInterface**)pValue = NULL;
            //     }
            // }
            assert(0);
            break;

        case Type_Int32Ptr:
            {
                Int32 tag = (Int32)mData->readInt32();
                if (tag != MSH_NULL) {
                    Int32* pv = (Int32*)malloc(sizeof(Int32));
                    *pv = mData->readInt32();
                    *(Int32**)value = pv;
                }
                else {
                    *(Int32**)value = NULL;
                }
            }
            break;

        case Type_Int64Ptr:
            {
                Int32 tag = (Int32)mData->readInt32();
                if (tag != MSH_NULL) {
                    Int64* pv = (Int64*)malloc(sizeof(Int64));
                    *pv = mData->readInt64();
                    *(Int64**)value = pv;
                }
                else {
                    *(Int64**)value = NULL;
                }
            }
            break;

        case Type_InterfacePtrPtr:
            // {
            //     Int32 tag = (Int32)mData->readInt32();
            //     if (tag != MSH_NULL) {
            //         tag = (Int32)mData->readInt32();
            //         if (tag != MSH_NULL) {
            //             InterfacePack ipack;
            //             CIClassInfo *pClassInfo = NULL;
            //             IParcelable *pParcelable = NULL;
            //             ClassID classId;
            //             InterfaceID iid;
            //             ECode ec;

            //             android::sp<android::IBinder> binder = mData->readStrongBinder();
            //             mData->read((void *)&ipack, sizeof(InterfacePack));
            //             ipack.m_pBinder = binder;
            //             if (IsParcelable(&ipack, &pClassInfo)) {
            //                 classId.clsid = ipack.m_clsid;
            //                 classId.pUunm = pClassInfo->pszUunm;
            //                 iid = pClassInfo->interfaces[ipack.m_uIndex]->iid;
            //                 ec = _CObject_CreateInstance(classId, RGM_SAME_DOMAIN,
            //                         EIID_IParcelable, (IInterface**)&pParcelable);
            //                 if (FAILED(ec)) return ec;

            //                 ((IParcelable*)pParcelable)->ReadFromParcel(this);
            //                 *((IInterface**)&pParcelable) = ((IParcelable*)pParcelable)->Probe(iid);
            //             }
            //             else {
            //                 ec = StdUnmarshalInterface(
            //                         UnmarshalFlag_Noncoexisting,
            //                         &ipack,
            //                         (IInterface **)pParcelable);
            //                 if (FAILED(ec)) {
            //                     MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
            //                             "MshProc: unmsh interface, ec = %x\n", ec));
            //                     return ec;
            //                 }
            //             }
            //             *(UInt32*)pValue = (UInt32)pParcelable;
            //         }
            //         else {
            //             *(UInt32*)pValue = NULL;
            //         }
            //     }
            //     else {
            //         *(IInterface**)pValue = NULL;
            //     }
            // }
            assert(0);
            break;

        case Type_Fd:
            *(Int32*)value = (Int32)mData->readFileDescriptor();
            break;

        default:
            assert(0);
            break;
    }

    return NOERROR;
}

ECode CParcel::WriteValue(
    /* [in] */ PVoid value,
    /* [in] */ Int32 type,
    /* [in] */ Int32 size)
{
    ECode ec;

    switch(type) {
        case Type_Byte:
        case Type_Boolean:
        case Type_Char8:
            mData->writeInt32(*((Byte*)value));
            break;

        case Type_Char16:
        case Type_Int16:
            mData->writeInt32(*((Int16*)value));
            break;

        case Type_Int32:
            mData->writeInt32(*((Int32*)value));
            break;

        case Type_Float:
            mData->writeFloat(*((Float*)value));
            break;

        case Type_Int64:
            mData->writeInt64(*((Int64*)value));
            break;

        case Type_Double:
            mData->writeDouble(*((Double*)value));
            break;

        case Type_CString:
            mData->writeInt32(value ? MSH_NOT_NULL : MSH_NULL);

            if (value) {
                mData->writeCString((const char*)value);
            }
            break;

        case Type_String:
            mData->writeInt32(value ? MSH_NOT_NULL : MSH_NULL);

            if (value) {
                mData->writeCString((const char*)value);
            }
            break;

        case Type_InterfacePtr:
            // mData->writeInt32(pValue ? MSH_NOT_NULL : MSH_NULL);

            // if (pValue) {
            //     InterfacePack itfPack;
            //     ec = StdMarshalInterface(
            //             (IInterface*)pValue,
            //             &itfPack);
            //     if (FAILED(ec)) return ec;

            //     mData->writeStrongBinder(itfPack.m_pBinder);
            //     itfPack.m_pBinder = NULL;
            //     mData->write((const void *)&itfPack, (int32_t)sizeof(InterfacePack));

            //     IParcelable *pParcelable = \
            //             (IParcelable*)((IInterface*)pValue)->Probe(EIID_IParcelable);
            //     if (pParcelable != NULL) pParcelable->WriteToParcel(this);
            // }
            assert(0);
            break;

        case Type_BytePtr:
        case Type_BooleanPtr:
        case Type_Char8Ptr:
        case Type_Char16Ptr:
        case Type_Int16Ptr:
        case Type_Int32Ptr:
        case Type_FloatPtr:
            mData->writeInt32(value ? MSH_NOT_NULL : MSH_NULL);

            if (value) {
                mData->writeInt32(*(UInt32*)value);
            }
            break;

        case Type_Int64Ptr:
        case Type_DoublePtr:
            mData->writeInt32(value ? MSH_NOT_NULL : MSH_NULL);

            if (value) {
                mData->writeInt64(*(Int64*)value);
            }
            break;

        case Type_StringPtr:
            mData->writeInt32(value ? MSH_NOT_NULL : MSH_NULL);

            if (value) {
                mData->writeCString((const char*)value);
            }
            break;

        case Type_InterfacePtrPtr:
            // mData->writeInt32(pValue ? MSH_NOT_NULL : MSH_NULL);

            // if (*(IInterface**)pValue) {
            //     InterfacePack itfPack;
            //     ec = StdMarshalInterface(
            //             *(IInterface**)pValue,
            //             &itfPack);
            //     if (FAILED(ec)) return ec;

            //     mData->writeInt32(MSH_NOT_NULL);
            //     mData->writeStrongBinder(itfPack.m_pBinder);
            //     itfPack.m_pBinder = NULL;
            //     mData->write((void*)&itfPack, sizeof(itfPack));

            //     IParcelable *pParcelable = \
            //             (IParcelable*)(*(IInterface**)pValue)->Probe(EIID_IParcelable);
            //     if (pParcelable != NULL) pParcelable->WriteToParcel(this);
            // }
            // else {
            //     mData->writeInt32(MSH_NULL);
            // }
            assert(0);
            break;

        case Type_Struct:
            mData->writeInt32(value ? MSH_NOT_NULL : MSH_NULL);

            if (value) {
                mData->writeInt32(size);
                mData->write(value, size);
            }
            break;

        case Type_EMuid:
            mData->write(value, sizeof(EMuid));

            break;

        case Type_EGuid:
            {
                mData->write(value, sizeof(EGuid));
                mData->writeCString(((EGuid *)value)->pUunm);
            }
            break;

        case Type_EMuidPtr:
            mData->writeInt32(value ? MSH_NOT_NULL : MSH_NULL);

            if (value) {
                mData->write(value, sizeof(EMuid));
            }
            break;

        case Type_EGuidPtr:
            mData->writeInt32(value ? MSH_NOT_NULL : MSH_NULL);

            if (value) {
                mData->write(value, sizeof(EGuid));
                mData->writeCString(((EGuid *)value)->pUunm);
            }
            break;

        case Type_ArrayOf:
        case Type_BufferOf:
        case Type_MemoryBuf:
        case Type_StringBuf:
            // mData->writeInt32(pValue ? MSH_NOT_NULL : MSH_NULL);

            // if (pValue) {
            //     mData->write(pValue, sizeof(CarQuintet));

            //     if (CarQuintetFlag_Type_IObject
            //         != (((PCARQUINTET)pValue)->m_flags
            //                 & CarQuintetFlag_TypeMask)) {
            //         // copy the storaged data
            //         //
            //         mData->write(((PCARQUINTET)pValue)->m_pBuf,
            //                 ((PCARQUINTET)pValue)->m_size);
            //     }
            //     else {
            //         Int32 used = ((PCARQUINTET)pValue)->m_used
            //                     / sizeof(IInterface *);
            //         Int32 *pBuf = (Int32*)((PCARQUINTET)pValue)->m_pBuf;
            //         for (Int32 i = 0; i < used; i++) {
            //             if (pBuf[i]) {
            //                 mData->writeInt32(MSH_NOT_NULL);

            //                 InterfacePack itfPack;
            //                 ec = StdMarshalInterface(
            //                         (IInterface *)pBuf[i],
            //                         &itfPack);
            //                 if (FAILED(ec)) {
            //                     MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
            //                             "MshProc: marshal interface, ec = %x\n", ec));
            //                     return ec;
            //                 }

            //                 mData->writeStrongBinder(itfPack.m_pBinder);
            //                 itfPack.m_pBinder = NULL;
            //                 mData->write((void*)&itfPack, sizeof(itfPack));

            //                 IParcelable *pParcelable = \
            //                         (IParcelable*)((IInterface*)pBuf[i])->Probe(EIID_IParcelable);
            //                 if (pParcelable != NULL) pParcelable->WriteToParcel(this);
            //             }
            //             else {  // null pointer
            //                 mData->writeInt32(MSH_NULL);
            //             }
            //         }
            //     }
            // }
            assert(0);
            break;

        case Type_ArrayOfString:
            mData->writeInt32(value ? MSH_NOT_NULL : MSH_NULL);

            if (value) {
                mData->write(value, sizeof(CarQuintet));

                Int32 size = ((PCARQUINTET)value)->m_size
                            / sizeof(String);
                String *pBuf = (String*)((PCARQUINTET)value)->m_pBuf;
                for (Int32 i = 0; i < size; i++) {
                    if (!pBuf[i].IsNull()) {
                        mData->writeInt32(MSH_NOT_NULL);
                        mData->writeCString(pBuf[i].string());
                    }
                    else {  // null pointer
                        mData->writeInt32(MSH_NULL);
                    }
                }
            }
            break;

        case Type_BufferOfString:
            mData->writeInt32(value ? MSH_NOT_NULL : MSH_NULL);

            if (value) {
                mData->write(value, sizeof(CarQuintet));

                Int32 used = ((PCARQUINTET)value)->m_used
                            / sizeof(String);
                String *pBuf = (String*)((PCARQUINTET)value)->m_pBuf;
                for (Int32 i = 0; i < used; i++) {
                    if (!pBuf[i].IsNull()) {
                        mData->writeInt32(MSH_NOT_NULL);
                        mData->writeCString(pBuf[i].string());
                    }
                    else {  // null pointer
                        mData->writeInt32(MSH_NULL);
                    }
                }
            }
            break;

        case Type_Fd:
            mData->writeFileDescriptor(*((Int32*)value));
            break;

        case Type_Fd_Dup:
            mData->writeDupFileDescriptor(*((Int32*)value));
            break;

        default:
            assert(0);
            break;
    }

    return NOERROR;
}
