#include "media/audiofx/CAudioEffect.h"
#include <elastos/System.h>
#include <Logger.h>
#include <StringBuffer.h>


using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

CAudioEffect::CAudioEffect()
{

}

ECode CAudioEffect::constructor(
    /* [in] */ IUUID* type,
    /* [in] */ IUUID* uuid,
    /* [in] */ Int32 priority,
    /* [in] */ Int32 audioSession)
{
    return AudioEffect::Init(type, uuid, priority, audioSession);
}

ECode CAudioEffect::ReleaseResources()
{
    return AudioEffect::ReleaseResources();
}

ECode CAudioEffect::GetDescriptor(
    /* [out] */ IAudioEffectDescriptor** descriptor)
{
    VALIDATE_NOT_NULL(descriptor);

    return AudioEffect::GetDescriptor(descriptor);
}

ECode CAudioEffect::QueryEffects(
    /* [out, callee] */ ArrayOf<IAudioEffectDescriptor*>** descriptor)
{
    VALIDATE_NOT_NULL(descriptor);


    return AudioEffect::QueryEffects(descriptor);
}

ECode CAudioEffect::SetEnabled(
    /* [in]  */ Boolean enabled,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    return AudioEffect::SetEnabled(enabled, result);
}

ECode CAudioEffect::SetParameter(
    /* [in] */ ArrayOf<Byte>* param,
    /* [in] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    return AudioEffect::SetParameter(param, value, result);
}

ECode CAudioEffect::SetParameterEx(
    /* [in] */ Int32 param,
    /* [in] */ Int32 value,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    return AudioEffect::SetParameterEx(param, value, result);
}

ECode CAudioEffect::SetParameterEx2(
    /* [in] */ Int32 param,
    /* [in] */ Int16 value,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    return AudioEffect::SetParameterEx2(param, value, result);
}

ECode CAudioEffect::SetParameterEx3(
    /* [in] */ Int32 param,
    /* [in] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    return AudioEffect::SetParameterEx3(param, value, result);
}

ECode CAudioEffect::SetParameterEx4(
    /* [in] */ ArrayOf<Int32>* param,
    /* [in] */ ArrayOf<Int32>* value,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    return AudioEffect::SetParameterEx4(param, value, result);
}

ECode CAudioEffect::SetParameterEx5(
    /* [in] */ ArrayOf<Int32>* param,
    /* [in] */ ArrayOf<Int16>* value,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    return AudioEffect::SetParameterEx5(param, value, result);
}
ECode CAudioEffect::SetParameterEx6(
    /* [in] */ ArrayOf<Int32>* param,
    /* [in] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    return AudioEffect::SetParameterEx6(param, value, result);
}

ECode CAudioEffect::GetParameter(
    /* [in] */ ArrayOf<Byte>* param,
    /* [out] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_NOT_NULL(status);

    return AudioEffect::GetParameter(param, value, status);
}

ECode CAudioEffect::GetParameterEx(
    /* [in] */ const Int32 param,
    /* [out] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(status);

    return AudioEffect::GetParameterEx(param, value, status);
}

ECode CAudioEffect::GetParameterEx2(
    /* [in] */ Int32 param,
    /* [out] */ ArrayOf<Int32>* value,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_NOT_NULL(status);

    return AudioEffect::GetParameterEx2(param, value, status);
}

ECode CAudioEffect::GetParameterEx3(
    /* [in] */ Int32 param,
    /* [out] */ ArrayOf<Int16>* value,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_NOT_NULL(status);

    return AudioEffect::GetParameterEx3(param, value, status);
}

ECode CAudioEffect::GetParameterEx4(
    /* [in] */ ArrayOf<Int32>* param,
    /* [out] */ ArrayOf<Int32>* value,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(status);

    return AudioEffect::GetParameterEx4(param, value, status);
}

ECode CAudioEffect::GetParameterEx5(
    /* [in] */ ArrayOf<Int32>* param,
    /* [out] */ ArrayOf<Int16>* value,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(value);
    VALIDATE_NOT_NULL(status);

    return AudioEffect::GetParameterEx5(param, value, status);
}

ECode CAudioEffect::GetParameterEx6(
    /* [in] */ ArrayOf<Int32>* param,
    /* [in] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(status);

    return AudioEffect::GetParameterEx6(param, value, status);
}

ECode CAudioEffect::Command(
    /* [in] */ Int32 cmdCode,
    /* [in] */ ArrayOf<Byte>* command,
    /* [out] */ ArrayOf<Byte>* reply,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(reply);
    VALIDATE_NOT_NULL(result);

    return AudioEffect::Command(cmdCode, command, reply, result);
}

ECode CAudioEffect::GetId(
    /* [out] */ Int32* Id)
{
    VALIDATE_NOT_NULL(Id);

    return AudioEffect::GetId(Id);
}

ECode CAudioEffect::GetEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);

    return AudioEffect::GetEnabled(enabled);
}

ECode CAudioEffect::HasControl(
    /* [out] */ Boolean* control)
{
    VALIDATE_NOT_NULL(control);

    return AudioEffect::HasControl(control);
}

ECode CAudioEffect::SetEnableStatusListener(
    /* [in] */ IAudioEffectOnEnableStatusChangeListener* listener)
{
    return AudioEffect::SetEnableStatusListener(listener);
}

ECode CAudioEffect::SetControlStatusListener(
    /* [in] */ IAudioEffectOnControlStatusChangeListener* listener)
{
    return AudioEffect::SetControlStatusListener(listener);
}

ECode CAudioEffect::SetParameterListener(
    /* [in] */ IAudioEffectOnParameterChangeListener* listener)
{
    return AudioEffect::SetParameterListener(listener);
}


ECode CAudioEffect::CheckState(
    /* [in] */ CString methodName)
{
    return AudioEffect::CheckState(methodName);
}

ECode CAudioEffect::CheckStatus(
    /* [in] */ Int32 status)
{
    return AudioEffect::CheckStatus(status);
}

ECode CAudioEffect::ByteArrayToInt32(
    /* [in] */ ArrayOf<Byte>* valueBuf,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    return AudioEffect::ByteArrayToInt32Ex(valueBuf, 0, result);
}

ECode CAudioEffect::ByteArrayToInt32Ex(
    /* [in] */ ArrayOf<Byte>* valueBuf,
    /* [in] */ Int32 offset,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    return AudioEffect::ByteArrayToInt32Ex(valueBuf, offset, result);
}

ECode CAudioEffect::Int32ToByteArray(
    /* [in] */ Int32 value,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);

    return AudioEffect::Int32ToByteArray(value, result);
}

ECode CAudioEffect::ByteArrayToInt16(
    /* [in] */ ArrayOf<Byte>* valueBuf,
    /* [out] */ Int16* result)
{
    VALIDATE_NOT_NULL(result);

    return ByteArrayToInt16Ex(valueBuf, 0, result);
}

ECode CAudioEffect::ByteArrayToInt16Ex(
    /* [in] */ ArrayOf<Byte>* valueBuf,
    /* [in] */ Int32 offset,
    /* [out] */ Int16* result)
{
    VALIDATE_NOT_NULL(result);

    return AudioEffect::ByteArrayToInt16Ex(valueBuf, offset, result);
}

ECode CAudioEffect::Int16ToByteArray(
    /* [in] */ Int16 value,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);

    return AudioEffect::Int16ToByteArray(value, result);
}

ECode CAudioEffect::ConcatArrays(
    /* [in] */ ArrayOf<Byte>* array1,
    /* [in] */ ArrayOf<byte>* array2,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);

    return AudioEffect::ConcatArrays( array1, array2, result);
}
