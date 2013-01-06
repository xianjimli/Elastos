
#include "cmdef.h"
#include "CRuntime.h"


ECode CRuntime::Exec(
    /* [in] */ const ArrayOf<String>& progArray,
    /* [out] */ IProcess** proc)
{
    VALIDATE_NOT_NULL(proc);

    return ExecEx2(progArray, NULL, NULL, proc);
}

ECode CRuntime::ExecEx(
    /* [in] */ const ArrayOf<String>& progArray,
    /* [in] */ const ArrayOf<String>& envp,
    /* [out] */ IProcess** proc)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRuntime::ExecEx2(
    /* [in] */ const ArrayOf<String>& progArray,
    /* [in] */ ArrayOf<String>* envp,
    /* [in] */ IFile* directory,
    /* [out] */ IProcess** proc)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRuntime::ExecEx3(
    /* [in] */ const String& prog,
    /* [out] */ IProcess** proc)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRuntime::ExecEx4(
    /* [in] */ const String& prog,
    /* [in] */ const ArrayOf<String>& envp,
    /* [out] */ IProcess** proc)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRuntime::ExecEx5(
    /* [in] */ const String& prog,
    /* [in] */ const ArrayOf<String>& envp,
    /* [in] */ IFile* directory,
    /* [out] */ IProcess** proc)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRuntime::Exit(
    /* [in] */ Int32 code)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRuntime::Load(
    /* [in] */ const String& pathName)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRuntime::LoadLibrary(
    /* [in] */ const String& libName)
{
    return E_NOT_IMPLEMENTED;
}
