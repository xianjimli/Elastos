
#include <elastos.h>
#include <clsinfo.h>

#include "app/marshal.h"
#include "utils/log.h"

using namespace Elastos;

ECode Proxy_WriteParameters(
    /* [in] */ const CIMethodInfo* methodInfo,
    /* [in] */ UInt32* args,
    /* [in, out] */ IParcel* params)
{
    Int32 paramNum, n;
    const CIBaseType* paramInfos;
    
    paramNum = methodInfo->paramNum;
    paramInfos = methodInfo->params;
    
    for (n = 0; n < paramNum; n++) {      
        switch (BT_TYPE(paramInfos[n])) {
            case BT_TYPE_UINT8:
            case BT_TYPE_UINT16:
            case BT_TYPE_UINT32:
                assert(BT_IS_IN(paramInfos[n]));
                params->WriteInt32(*args);
                args++;
                break;

            case BT_TYPE_PUINT8:
            case BT_TYPE_PUINT16:
            case BT_TYPE_PUINT32:
                params->WriteInt32Ptr((Handle32)*args);
                args++;

            case BT_TYPE_UINT64:
                assert(BT_IS_IN(paramInfos[n]));
                params->WriteInt64(*args);
                args += 2;
                break;

            case BT_TYPE_PUINT64:
                params->WriteInt64Ptr((Handle32)*args);
                args++;
                break;
                
                // ? 
            case BT_TYPE_STRUCT:
                assert(BT_IS_IN(paramInfos[n]));
                if (*args) {
                    params->WriteStruct((Handle32)args, BT_TYPE_SIZE(paramInfos[n]) * 4);
                }
                else {
                    params->WriteInt32(0);
                }
                args++;
                break;
                
            // ?    
            case BT_TYPE_PSTRUCT:
                assert(0);
                break;    
                
            case BT_TYPE_EMUID:
                assert(BT_IS_IN(paramInfos[n]));
                params->WriteEMuid(*(EMuid*)args);
                args += sizeof(EMuid) / 4;
                break;
            
            case BT_TYPE_EGUID:
                assert(BT_IS_IN(paramInfos[n]));
                params->WriteEGuid(*(EGuid*)args);
                args += sizeof(EMuid) / 4;
                args++;
                break;
                
            case BT_TYPE_STRING:
                assert(BT_IS_IN(paramInfos[n]));
                params->WriteString(*(String*)args);
                args++;
                break;
            
            case BT_TYPE_INTERFACE:
                assert(BT_IS_IN(paramInfos[n]));
                params->WriteInterfacePtr((IInterface*)*args);
                args++;
                break;
            
            case BT_TYPE_PINTERFACE:
                assert(BT_IS_OUT(paramInfos[n]));
                params->WriteInterfacePtrPtr((Handle32)*args);
                args++;
                break;    

            case BT_TYPE_ARRAYOF:
                params->WriteArrayOf((Handle32)args);
                args++;
                break;
            
            case BT_TYPE_BUFFEROF:
                params->WriteBufferOf((Handle32)args);
                args++;
                break;

            default:
                LOGE("MshProc: Invalid [in, out] type(%08x).\n", params[n]);
                assert(0);
                return E_INVALID_ARGUMENT;
        }
    }
    return NOERROR;
}
