#include "Vm.h"

Vm::Vm()
    :mHandle(0)
    ,mError_code(0)
{
}

ECode Vm::Step(
    /** [in] **/ICallback* cb,
    /** [out] **/Boolean* result)
{
    return NOERROR;
}

ECode Vm::Compile(
    /** [out] **/Boolean* result)
{
    return NOERROR;
}

ECode Vm::Stop()
{
    return NOERROR;
}

ECode Vm::Finalize()
{
    return NOERROR;
}

ECode Vm::Internal_init()
{
    return NOERROR;
}