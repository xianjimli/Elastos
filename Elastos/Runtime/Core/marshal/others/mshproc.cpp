
#include <stdio.h>
#include <eltypes.h>
#include "marshal.h"

ECode Proxy_ProcessMsh_BufferSize(
    /* [in] */ const CIMethodInfo *pMethodInfo,
    /* [in] */ UInt32 *puArgs,
    /* [out] */ UInt32 *puInSize,
    /* [out] */ UInt32 *puOutSize)
{
    int n, cParams;
    UInt32 uInSize, uOutSize, uSize = 0;
    const CIBaseType *pParams;

    uInSize = 0;
    uOutSize = 0;
    cParams = pMethodInfo->paramNum;
    pParams = pMethodInfo->params;

    for (n = 0; n < cParams; n++) {
        if (BT_IS_OUT(pParams[n])) {    // [out]
            uOutSize += sizeof(UInt32); // for pointer

            switch (BT_TYPE(pParams[n])) {
                case BT_TYPE_PUINT8:
                case BT_TYPE_PUINT16:
                case BT_TYPE_PUINT32:
                    uSize = sizeof(UInt32);
                    break;

                case BT_TYPE_PUINT64:
                    uSize = sizeof(UInt64);
                    break;

                case BT_TYPE_PEMUID:
                    uSize = sizeof(EMuid);
                    break;

                case BT_TYPE_PEGUID:
                    uSize = sizeof(EMuid) + sizeof(char*) \
                            + 80 * sizeof(char);
                    break;

                case BT_TYPE_PSTRUCT :
                    uSize = sizeof(UInt32) * BT_TYPE_SIZE(pParams[n]);
                    break;
                    
                case BT_TYPE_PSTRING:
                    uSize = sizeof(String);
                    break;    

                case BT_TYPE_STRINGBUF:
                case BT_TYPE_BUFFEROF:
                case BT_TYPE_ARRAYOF:{
                    if (*puArgs) {
                        uSize = sizeof(CarQuintet) + \
                          MSH_ALIGN_4(((PCARQUINTET)*puArgs)->m_size);
                    }
                    break;
                }

                case BT_TYPE_PINTERFACE:
                    uSize = sizeof(InterfacePack);
                    break;  

                default:
                    MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                        "MshProc: Invalid [in, out] type(%08x).\n",
                        pParams[n]));
                    assert(0);
                    return E_INVALID_ARGUMENT;
            }

            if (*puArgs) {
                uOutSize += uSize;
            }
            // else: null pointer do not contain data.
        }

        if (BT_IS_IN(pParams[n])) { // [in]
            switch (BT_TYPE(pParams[n])) {
                case BT_TYPE_UINT8:
                case BT_TYPE_UINT16:
                case BT_TYPE_UINT32:
                    uInSize += sizeof(UInt32);
                    puArgs++;
                    break;

                case BT_TYPE_PUINT8:
                case BT_TYPE_PUINT16:
                case BT_TYPE_PUINT32:
                    if (*puArgs) {
                        uInSize += sizeof(UInt32) * 2;
                    }
                    else { // null pointer
                        uInSize += sizeof(UInt32);
                    }
                    puArgs++;
                    break;

                case BT_TYPE_UINT64:
#ifdef _mips
                    // Adjust for 64bits align on mips
                    if (!(n % 2)) {
                        uInSize += 4;
                        puArgs += 1;
                    }
#endif
                    uInSize += sizeof(UInt64);
                    puArgs += 2;
                    break;

                case BT_TYPE_PUINT64:
                    if (*puArgs) {
                        uInSize += sizeof(UInt32) + sizeof(UInt64);
                    }
                    else { // null pointer
                        uInSize += sizeof(UInt32);
                    }
                    puArgs++;
                    break;

                case BT_TYPE_EGUID:
                    uInSize += sizeof(EMuid) + sizeof(char*) \
                              + 80 * sizeof(char);
                    puArgs += sizeof(EMuid) / 4 + sizeof(char*) / 4;
                    break;

                case BT_TYPE_EMUID:
                    uInSize += sizeof(EMuid);
                    puArgs += sizeof(EMuid) / 4;
                    break;

                case BT_TYPE_PSTRUCT:
                case BT_TYPE_STRUCT:
                    if (*puArgs) {
                        uInSize += sizeof(UInt32) + sizeof(UInt32) * BT_TYPE_SIZE(pParams[n]);
                    }
                    else {  // null pointer
                        uInSize += sizeof(UInt32);
                    }
                    puArgs++;
                    break;

                case BT_TYPE_PEGUID:
                    if (*puArgs) {
                        uInSize += sizeof(UInt32) + sizeof(EMuid)
                                   + 80 * sizeof(char)
                                   + sizeof(wchar_t*);
                    }
                    else {  // null pointer
                        uInSize += sizeof(UInt32);
                    }
                    puArgs++;
                    break;

                case BT_TYPE_PEMUID:
                    if (*puArgs) {
                        uInSize += sizeof(UInt32) + sizeof(EMuid);
                    }
                    else {  // null pointer
                        uInSize += sizeof(UInt32);
                    }
                    puArgs++;
                    break;

                case BT_TYPE_STRING:
                    if (*puArgs) {
                        UInt32 nBufLen = (strlen((char *)(*puArgs)) + 1)
                            * sizeof(char);
                        uInSize += sizeof(UInt32) + sizeof(UInt32) +
                            MSH_ALIGN_4(nBufLen);
                    }
                    else {
                        uInSize += sizeof(UInt32);
                    }
                    puArgs++;
                    break;

                case BT_TYPE_INTERFACE:
                    if (*puArgs) {                                              
                        uInSize += sizeof(InterfacePack);
                    }
                    else {  // null pointer
                        uInSize += sizeof(UInt32);
                    }
                    puArgs++;
                    break;

                case BT_TYPE_PINTERFACE:
                    if (*puArgs) {
                        if (*(UInt32 *)(*puArgs)) {
                            uInSize += sizeof(InterfacePack);
                        }
                        else {
                            uInSize += sizeof(UInt32);
                        }
                    }
                    uInSize += sizeof(UInt32);  // for pointer
                    puArgs++;
                    break;

                case BT_TYPE_STRINGBUF:
                case BT_TYPE_BUFFEROF:
                case BT_TYPE_ARRAYOF:
                    if (*puArgs) {
                        if (CarQuintetFlag_Type_IObject
                            != (((PCARQUINTET)*puArgs)->m_flags
                                    & CarQuintetFlag_TypeMask)) {
                            uInSize += MSH_ALIGN_4(sizeof(UInt32) \
                              + sizeof(CarQuintet) \
                              + ((PCARQUINTET)*puArgs)->m_size);
                        }
                        else {
                            uInSize += MSH_ALIGN_4(sizeof(UInt32) \
                              + sizeof(CarQuintet));
                            int used = ((PCARQUINTET)*puArgs)->m_used
                                        / sizeof(IInterface *);
                            int *pBuf = (int*)((PCARQUINTET)*puArgs)->m_pBuf;
                            uint_t uUsedSize = 0;
                            for (int i = 0; i < used; i++) {
                                if (pBuf[i]) {                              
                                    uUsedSize += sizeof(InterfacePack);
                                }
                                else {  // null pointer
                                    uUsedSize += sizeof(UInt32);
                                }
                            }
                            uInSize += MAX((MemorySize)uUsedSize, \
                                MSH_ALIGN_4(((PCARQUINTET)*puArgs)->m_size));
                        }
                    }
                    else {  // null pointer
                        uInSize += sizeof(UInt32);
                    }
                    puArgs++;
                    break;

                default:
                    MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                        "MshProc: Invalid [in, out] type(%08x).\n",
                        pParams[n]));
                    assert(0);
                    return E_INVALID_ARGUMENT;
            }
        }
        else {  // [out]
            if (((BT_TYPE(pParams[n]) == BT_TYPE_BUFFEROF) ||
                (BT_TYPE(pParams[n]) == BT_TYPE_ARRAYOF) ||
                (BT_TYPE(pParams[n]) == BT_TYPE_STRINGBUF)) && *puArgs) {
                uInSize += sizeof(UInt32); // for size only
            }
            puArgs++;
            uInSize += sizeof(UInt32);
        }
    }

    *puInSize = uInSize;
    *puOutSize = uOutSize;
    return NOERROR;
}

ECode Proxy_ProcessMsh_In(
    /* [in] */ const CIMethodInfo *pMethodInfo,
    /* [in] */ UInt32 *puArgs,
    /* [in, out] */ IParcel *pParcel)
{
    int n, cParams;
    const CIBaseType *pParams;
    ECode ec;

    cParams = pMethodInfo->paramNum;
    pParams = pMethodInfo->params;

    for (n = 0; n < cParams; n++) {
        if (BT_IS_IN(pParams[n])) { // [in] or [in, out]
            switch (BT_TYPE(pParams[n])) {
                case BT_TYPE_UINT8:
                case BT_TYPE_UINT16:
                case BT_TYPE_UINT32:
                    pParcel->WriteInt32((Int32)*puArgs);
                    puArgs++;
                    break;

                case BT_TYPE_PUINT8:
                case BT_TYPE_PUINT16:
                case BT_TYPE_PUINT32:
                    pParcel->WriteInt32Ptr((Handle32)*puArgs);
                    puArgs++;
                    break;

                case BT_TYPE_UINT64:
#ifdef _mips
                    // Adjust for 64bits align on mips
                    if (!(n % 2)) puArgs += 1;
#endif
                    pParcel->WriteInt64((Int64)(*(UInt64 *)puArgs));
                    puArgs += 2;
                    break;

                case BT_TYPE_PUINT64:
                    pParcel->WriteInt64Ptr((Handle32)*puArgs);
                    puArgs++;
                    break;

                case BT_TYPE_STRUCT:
                case BT_TYPE_PSTRUCT:
                    pParcel->WriteStruct(*puArgs, BT_TYPE_SIZE(pParams[n]) * 4);
                    puArgs++;
                    break;
                    
                case BT_TYPE_EMUID:
                    pParcel->WriteEMuid(*(EMuid *)puArgs);
                    puArgs += sizeof(EMuid) / 4;
                    break;    

                case BT_TYPE_EGUID:
                    pParcel->WriteEGuid(*(EGuid *)puArgs);
                    puArgs += sizeof(EGuid) / 4;
                    break;

                case BT_TYPE_PEMUID:
                    pParcel->WriteEMuidPtr((Handle32)*puArgs);
                    puArgs++;
                    break;

                case BT_TYPE_PEGUID:
                    pParcel->WriteEGuidPtr((Handle32)*puArgs);
                    puArgs++;
                    break;
            
                case BT_TYPE_STRINGBUF:
                    pParcel->WriteStringBuf((Handle32)*puArgs);
                    puArgs++;
                    break;
                    
                case BT_TYPE_BUFFEROF:
                    pParcel->WriteBufferOf((Handle32)*puArgs);
                    puArgs++;
                    break;
                    
                case BT_TYPE_ARRAYOF:
                    pParcel->WriteArrayOf((Handle32)*puArgs);
                    puArgs++;
                    break;

                case BT_TYPE_STRING:
                    pParcel->WriteString(String((char*)*puArgs));
                    puArgs++;
                    break;

                case BT_TYPE_INTERFACE:
                    ec = pParcel->WriteInterfacePtr((IInterface *)*puArgs);
                    if (FAILED(ec)) {
                        MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                                "MshProc: marshal interface, ec = %x\n", ec));
                        return ec;
                    }
                    puArgs++;
                    break;

                case BT_TYPE_PINTERFACE:
                    ec = pParcel->WriteInterfacePtrPtr((Handle32)*puArgs);
                    if (FAILED(ec)) {
                        MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                                "MshProc: marshal interface, ec = %x\n", ec));
                        return ec;
                    }
                    puArgs++;
                    break;

                default:
                    MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                        "MshProc: Invalid [in, out] type(%08x).\n",
                        pParams[n]));
                    assert(0);
                    return E_INVALID_ARGUMENT;
            }
        }
        else {  // [out]
            if (*puArgs) pParcel->WriteInt32(MSH_NOT_NULL);
            else pParcel->WriteInt32(MSH_NULL);

            if (((BT_TYPE(pParams[n]) == BT_TYPE_BUFFEROF) ||
                (BT_TYPE(pParams[n]) == BT_TYPE_ARRAYOF) ||
                (BT_TYPE(pParams[n]) == BT_TYPE_STRINGBUF)) && *puArgs) {
                pParcel->WriteInt32(((PCARQUINTET)*puArgs)->m_size);
            }
            puArgs++;
        }
    }

    return NOERROR;
}

ECode Proxy_ProcessUnmsh_Out(
    /* [in] */ const CIMethodInfo *pMethodInfo,
    /* [in] */ IParcel *pParcel,
    /* [in] */ UInt32 uDataSize,
    /* [in, out] */ UInt32 *puArgs)
{
    Int32 n, cParams, pointValue;
    const CIBaseType *pParams;
    cParams = pMethodInfo->paramNum;
    pParams = pMethodInfo->params;

    for (n = 0; n < cParams; n++) {
        if (BT_IS_OUT(pParams[n])) {   // [out] or [in, out]
            if (*puArgs) {
                pParcel->ReadInt32(&pointValue);
                if (pointValue != (Int32)MSH_NOT_NULL) {
                    MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                        "MshProc: arg conflict in proxy's unmarshal.\n"));
                    return E_INVALID_ARGUMENT;
                }

                switch (BT_TYPE(pParams[n])) {
                    case BT_TYPE_PUINT8:
                        pParcel->ReadByte((Byte*)*puArgs);
                        break;

                    case BT_TYPE_PUINT16:
                        pParcel->ReadInt16((Int16*)*puArgs);
                        break;

                    case BT_TYPE_PUINT32:
                        pParcel->ReadInt32((Int32*)*puArgs);
                        break;

                    case BT_TYPE_PUINT64:
                        pParcel->ReadInt64((Int64*)*puArgs);
                        break;
                        
                    case BT_TYPE_PSTRUCT:
                        pParcel->ReadStruct((Handle32*)*puArgs);
                        break;    

                    case BT_TYPE_PEMUID:
                        pParcel->ReadEMuid((EMuid*)*puArgs);
                        break;

                    case BT_TYPE_PEGUID:
                        pParcel->ReadEGuid((EGuid*)*puArgs);
                        break;
                        
                    case BT_TYPE_PSTRING:
                        pParcel->ReadString((String*)*puArgs);
                        break;      

                    case BT_TYPE_STRINGBUF:
                    case BT_TYPE_BUFFEROF:
                    case BT_TYPE_ARRAYOF:
                        pParcel->ReadBufferOf((Handle32*)*puArgs);
                        break;

                    case BT_TYPE_PINTERFACE:
                        pParcel->ReadInterfacePtr((Handle32*)*puArgs);
                        break;

                    default:
                        MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                                "MshProc: Invalid arg type(%08x)\n", pParams[n]));
                        assert(0);
                        return E_INVALID_ARGUMENT;
                }
            }
            else {
                pParcel->ReadInt32(&pointValue);
                if (pointValue != MSH_NULL) {
                    MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                            "MshProc: arg conflict in proxy's unmarshal.\n"));
                    return E_INVALID_ARGUMENT;
                }
            }
            puArgs++;
        }
        else {      // [in]
            switch (BT_TYPE(pParams[n])) {
                case BT_TYPE_UINT64:
#ifdef _mips
                    // Adjust for 64bits align on mips
                    if (!(n % 2)) {
                        puArgs += 1;
                    }
#endif
                    puArgs += 2;
                    break;

                case BT_TYPE_EMUID:
                    puArgs += sizeof(EMuid) / 4;
                    break;

                case BT_TYPE_EGUID:
                    puArgs += sizeof(EMuid) / 4 + sizeof(char*) /4;
                    break;

                default:
                    puArgs++;
                    break;
            }
        }
    }

    return NOERROR;
}

ECode Stub_ProcessUnmsh_In(
    /* [in] */ const CIMethodInfo *pMethodInfo,
    /* [in] */ IParcel *pParcel,
    /* [in, out] */ UInt32 *puOutBuffer,
    /* [in, out] */ UInt32 *puArgs)
{
    int n, cParams;
    const CIBaseType *pParams;
    Int32 pointValue, size;
    ECode ec;

    if (puOutBuffer) {
        puOutBuffer = (UInt32 *)((MarshalHeader *)puOutBuffer + 1);
    }
    cParams = pMethodInfo->paramNum;
    pParams = pMethodInfo->params;

    for (n = 0; n < cParams; n++) {
        if (BT_IS_OUT(pParams[n])) {    // [out] or [in, out]
            assert(puOutBuffer);
            pParcel->ReadInt32(&pointValue);
            if (pointValue == (Int32)MSH_NOT_NULL) {
                *puOutBuffer = MSH_NOT_NULL;
                puOutBuffer++;

                switch (BT_TYPE(pParams[n])) { // [out]
                    case BT_TYPE_PUINT8:
                    case BT_TYPE_PUINT16:
                    case BT_TYPE_PUINT32:
                        *puArgs = (UInt32)puOutBuffer;
                        puOutBuffer++;
                        break;

                    case BT_TYPE_PUINT64:
                        *puArgs = (UInt32)puOutBuffer;
                        puOutBuffer += 2;
                        break;

                    case BT_TYPE_PEGUID:
                        *puArgs = (UInt32)puOutBuffer;
                        puOutBuffer += sizeof(EMuid) / 4;
                        *puOutBuffer = (UInt32)(puOutBuffer + 1);
                        puOutBuffer += sizeof(char*) / 4
                                    + 80 * sizeof(char) / 4;
                        break;

                    case BT_TYPE_PEMUID:
                        *puArgs = (UInt32)puOutBuffer;
                        puOutBuffer += sizeof(EMuid) / 4;
                        break;

                    case BT_TYPE_PSTRUCT:
                        *puArgs = (UInt32)puOutBuffer;
                        puOutBuffer += BT_TYPE_SIZE(pParams[n]);
                        break;

                    case BT_TYPE_PSTRING:
                        *puArgs = (UInt32)puOutBuffer;
                        puOutBuffer++;
                        break;
                    
                    case BT_TYPE_STRINGBUF:
                    case BT_TYPE_BUFFEROF:
                    case BT_TYPE_ARRAYOF:
                        pParcel->ReadInt32(&size);
                        *puArgs = (UInt32)(byte *)puOutBuffer;
                        ((PCARQUINTET)*puArgs)->m_reserve = 0;
                        ((PCARQUINTET)*puArgs)->m_size = size;
                        ((PCARQUINTET)*puArgs)->m_flags =
                                            CarQuintetFlag_Type_Unknown;
                        ((PCARQUINTET)*puArgs)->m_pBuf = \
                            (byte *)puOutBuffer + sizeof(CarQuintet);
                        if ((BT_TYPE(pParams[n]) == BT_TYPE_STRINGBUF)) {
                            ((PCARQUINTET)*puArgs)->m_used = sizeof(Char8);
                            *(Char8 *)(((PCARQUINTET)*puArgs)->m_pBuf) = 0;
                        }
                        else if (BT_TYPE(pParams[n]) == BT_TYPE_ARRAYOF) {
                            ((PCARQUINTET)*puArgs)->m_used =
                                    ((PCARQUINTET)*puArgs)->m_size;
                        }
                        else {
                            ((PCARQUINTET)*puArgs)->m_used = 0;
                        }

                        puOutBuffer = (UInt32 *)((Byte *)puOutBuffer
                                + MSH_ALIGN_4(sizeof(CarQuintet)
                                + ((PCARQUINTET)*puArgs)->m_size));
                        break;

                    case BT_TYPE_PINTERFACE:
                        *puOutBuffer = 0;
                        *puArgs = (UInt32)puOutBuffer;
                        puOutBuffer += sizeof(InterfacePack) / 4;
                        break;

                    default:
                        MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                            "MshProc: Invalid arg type(%08x) in UnMsh_in.\n",
                            pParams[n]));
                        return E_INVALID_ARGUMENT;
                }
            }
            else if (pointValue == MSH_NULL) {
                *puOutBuffer = MSH_NULL;
                puOutBuffer++;
                *puArgs = 0;
            }
            else {
                MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                    "MshProc: Invalid pointer value in unmarshal in.\n"));
                return E_INVALID_ARGUMENT;
            }
            puArgs++;
        }
        else if (BT_IS_IN(pParams[n])) {    // [in]
            switch (BT_TYPE(pParams[n])) {
                case BT_TYPE_UINT8:
                case BT_TYPE_UINT16:
                case BT_TYPE_UINT32:
                    pParcel->ReadInt32((Int32*)puArgs);
                    puArgs++;
                    break;

                case BT_TYPE_PUINT8:
                case BT_TYPE_PUINT16:
                case BT_TYPE_PUINT32:
                    pParcel->ReadInt32Ptr((Handle32*)puArgs);
                    puArgs++;
                    break;

                case BT_TYPE_UINT64:
#ifdef _mips
                    // Adjust for 64bits align on mips
                    if (!(n % 2)) {
                        puArgs += 1;
                    }
#endif
                    pParcel->ReadInt64((Int64*)puArgs);
                    puArgs += 2;
                    break;

                case BT_TYPE_PUINT64:
                    pParcel->ReadInt64Ptr((Handle32*)puArgs);
                    puArgs++;
                    break;
                
                case BT_TYPE_EMUID:
                    pParcel->ReadEMuid((EMuid*)puArgs);
                    puArgs += sizeof(EMuid) / 4;                   
                    break;    

                case BT_TYPE_EGUID:
                    pParcel->ReadEGuid((EGuid*)puArgs);
                    puArgs += (sizeof(EGuid) + 
                               MSH_ALIGN_4(strlen(((EGuid*)puArgs)->pUunm) + 1)) / 4;  
                    break;

                case BT_TYPE_PEMUID:
                    pParcel->ReadEMuidPtr((Handle32*)puArgs);
                    puArgs++;
                    break;  

                case BT_TYPE_PEGUID:
                    pParcel->ReadEGuidPtr((Handle32*)puArgs);
                    puArgs++;
                    break;

                case BT_TYPE_STRUCT:
                case BT_TYPE_PSTRUCT:
                    pParcel->ReadStructPtr((Handle32*)puArgs);
                    puArgs++;
                    break;

                case BT_TYPE_STRINGBUF:
                case BT_TYPE_BUFFEROF:
                case BT_TYPE_ARRAYOF:
                    pParcel->ReadBufferOfPtr((Handle32*)puArgs);
                    puArgs++;
                    break;

                case BT_TYPE_STRING:
                    pParcel->ReadStringPtr((Handle32*)puArgs);
                    puArgs++;
                    break;

                case BT_TYPE_INTERFACE:
                    ec = pParcel->ReadInterfacePtr((Handle32*)puArgs);
                    if (FAILED(ec)) {
                        MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                                "MshProc: unmsh interface, ec = %x\n", ec));
                        return ec;
                    }
                    puArgs++;
                    break;                   

                case BT_TYPE_PINTERFACE:
                    ec = pParcel->ReadInterfacePtrPtr((Handle32*)puArgs);
                    if (FAILED(ec)) {
                        MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                                "MshProc: unmsh interface, ec = %x\n", ec));
                        return ec;
                    }
                    puArgs++;
                    break;

                default:
                    MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                        "MshProc: Invalid arg type(%08x)\n", pParams[n]));
                    return E_INVALID_ARGUMENT;
            }
        }
        else {
            MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                "MshProc: Invalid arg type(%08x)\n", pParams[n]));
            return E_INVALID_ARGUMENT;
        }
    }

    return NOERROR;
}

ECode Stub_ProcessMsh_Out(
        /* [in] */ const CIMethodInfo *pMethodInfo,
        /* [in] */ UInt32 *puArgs,
        /* [in] */ UInt32 *puOutBuffer,
        /* [in] */ Boolean bOnlyReleaseIn,
        /* [in, out] */ IParcel* pParcel)
{
    int n, cParams;
    const CIBaseType *pParams;
    MarshalHeader * pOutHeader = NULL;
    ECode ec;

    cParams = pMethodInfo->paramNum;
    pParams = pMethodInfo->params;

    // skip the Out Marshal Header;
    if (puOutBuffer) {
        pOutHeader = (MarshalHeader *)(Void *)puOutBuffer;
        puOutBuffer = (UInt32 *)((MarshalHeader *)puOutBuffer + 1);
    }

    for (n = 0; n < cParams; n++) {
        if (BT_IS_OUT(pParams[n])) {    // [out] or [in, out]
            puArgs++;
            if (*puOutBuffer != MSH_NULL) {
                assert(*puOutBuffer == MSH_NOT_NULL);
                pParcel->WriteInt32(MSH_NOT_NULL);
                puOutBuffer++;
                
                switch (BT_TYPE(pParams[n])) {
                    case BT_TYPE_PUINT64:
                        pParcel->WriteInt64((Int64)*puOutBuffer);
                        puOutBuffer += 2;
                        break;

                    case BT_TYPE_PSTRING:
                        if (*puOutBuffer) {
                            pParcel->WriteString(String((char*)*puOutBuffer));
                            free((void*)*puOutBuffer);
                        }
                        else {
                            pParcel->WriteInt32(MSH_NULL);
                        }
                        puOutBuffer++;
                        break;
                        
                    case BT_TYPE_STRINGBUF:
                        pParcel->WriteStringBuf((Handle32)puOutBuffer);                        
                        puOutBuffer = (UInt32 *)((Byte *)puOutBuffer +
                            MSH_ALIGN_4(sizeof(CarQuintet)
                            + ((PCARQUINTET)((Byte *)puOutBuffer))->m_size));
                        break;
                                               
                    case BT_TYPE_BUFFEROF:
                        pParcel->WriteBufferOf((Handle32)puOutBuffer);
                        puOutBuffer = (UInt32 *)((Byte *)puOutBuffer +
                            MSH_ALIGN_4(sizeof(CarQuintet)
                            + ((PCARQUINTET)((Byte *)puOutBuffer))->m_size));
                        break;
                                                
                    case BT_TYPE_ARRAYOF:
                        pParcel->WriteArrayOf((Handle32)puOutBuffer);
                        puOutBuffer = (UInt32 *)((Byte *)puOutBuffer +
                            MSH_ALIGN_4(sizeof(CarQuintet)
                            + ((PCARQUINTET)((Byte *)puOutBuffer))->m_size));    
                        break;

                    case BT_TYPE_PEMUID:
                        pParcel->WriteEMuid(*(EMuid*)puOutBuffer);
                        puOutBuffer += sizeof(EMuid) / 4;
                        break;

                    case BT_TYPE_PSTRUCT:
                        pParcel->WriteStruct((Handle32)puOutBuffer, BT_TYPE_SIZE(pParams[n]));
                        puOutBuffer += BT_TYPE_SIZE(pParams[n]);
                        break;

                    case BT_TYPE_PEGUID:
                        pParcel->WriteEGuid(*(EGuid*)puOutBuffer);
                        puOutBuffer += sizeof(EMuid) / 4 + sizeof(char*) / 4 \
                                       + 80 * sizeof(char) / 4;               
                        break;

                    case BT_TYPE_PINTERFACE:
                        if (!bOnlyReleaseIn) {
                            IInterface *pObj;

                            pObj = (IInterface *)*puOutBuffer;
                            ec = pParcel->WriteInterfacePtr(pObj);
                            if (FAILED(ec)) {
                                MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                                        "MshProc: marshal interface, ec = %x\n", ec));
                                return ec;
                            }
                            
                            if (NULL != pObj) pObj->Release();

                            puOutBuffer += sizeof(InterfacePack) / 4;
                        }
                        break;

                    default:
                        pParcel->WriteInt32(*(Int32*)puOutBuffer);
                        puOutBuffer++;
                        break;
                }
            }
            else {
                assert(*puOutBuffer == MSH_NULL);
                pParcel->WriteInt32(MSH_NULL);
                puOutBuffer++;
            }
        }
        else {  // BT_IS_IN
            switch (BT_TYPE(pParams[n])) {
                case BT_TYPE_UINT64:
#ifdef _mips
                    // Adjust for 64bits align on mips
                    if (!(n % 2)) puArgs += 1;
#endif
                    puArgs += 2;
                    break;

                case BT_TYPE_EMUID:
                    puArgs += sizeof(EMuid) / 4;
                    break;

                case BT_TYPE_EGUID:
                    puArgs += sizeof(EMuid) / 4 + sizeof(char*) / 4;
                    break;

                case BT_TYPE_PINTERFACE:
                    if (*puArgs && *(UInt32 *)(*puArgs)) {
                        ((IInterface *)*(UInt32 *)*puArgs)->Release();
                    }
                    *puArgs++;
                    break;

                case BT_TYPE_INTERFACE:
                    if (*puArgs) {
                        ((IInterface *)*puArgs)->Release();
                    }
                    *puArgs++;
                    break;

                case BT_TYPE_STRINGBUF:
                case BT_TYPE_BUFFEROF:
                case BT_TYPE_ARRAYOF:
                    if (*puArgs) {
                        if (CarQuintetFlag_Type_IObject
                            == (((PCARQUINTET)*puArgs)->m_flags
                                    & CarQuintetFlag_TypeMask)) {
                            int used = ((PCARQUINTET)*puArgs)->m_used
                                        / sizeof(IInterface *);
                            int *pBuf = (int*)((PCARQUINTET)*puArgs)->m_pBuf;
                            for (int i = 0; i < used; i++) {
                                if (pBuf[i]) {
                                    ((IInterface *)pBuf[i])->Release();
                                }
                            }
                        }
                    }
                    *puArgs++;
                    break;

                default:
                    *puArgs++;
                    break;
            }
        }
    }

    return NOERROR;
}
