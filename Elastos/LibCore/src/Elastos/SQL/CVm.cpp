
#include "cmdef.h"
#include "CVm.h"

ECode CVm::constructor()
{
	return NOERROR;
}
ECode CVm::Step(
    /** [in] **/ICallback* cb,
    /** [out] **/Boolean* result)
{
    return Vm::Step(cb, result);
}

ECode CVm::Compile(
    /** [out] **/Boolean* result)
{
    return Vm::Compile(result);
}

ECode CVm::Stop()
{
    return Vm::Stop();
}

ECode CVm::Finalize()
{
    return Vm::Finalize();
}