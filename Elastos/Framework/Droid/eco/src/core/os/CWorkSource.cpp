
#include "os/CWorkSource.h"

CWorkSource::CWorkSource()
    : mNum(0)
{
}

ECode CWorkSource::Size(
    /* [out] */ Int32* size)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWorkSource::Get(
    /* [in] */ Int32 index,
    /* [out] */ Int32* uid)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWorkSource::Clear()
{
    return E_NOT_IMPLEMENTED;
}

ECode CWorkSource::Diff(
    /* [in] */ IWorkSource* other,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWorkSource::Set(
    /* [in] */ IWorkSource* other)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWorkSource::SetEx(
    /* [in] */ Int32 uid)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWorkSource::SetReturningDiffs(
    /* [in] */ IWorkSource* other,
    /* [out, callee] */ ArrayOf<IWorkSource>** diffs)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWorkSource::Add(
    /* [in] */ IWorkSource* other,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWorkSource::AddReturningNewbs(
    /* [in] */ IWorkSource* other,
    /* [out] */ IWorkSource** result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWorkSource::AddEx(
    /* [in] */ Int32 uid,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWorkSource::AddReturningNewbsEx(
    /* [in] */ Int32 uid,
    /* [out] */ IWorkSource** result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWorkSource::Remove(
    /* [in] */ IWorkSource* other,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWorkSource::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWorkSource::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWorkSource::constructor()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CWorkSource::constructor(
    /* [in] */ IWorkSource * orig)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWorkSource::constructor(
    /* [in] */ Int32 uid)
{
    return E_NOT_IMPLEMENTED;
}

ECode CWorkSource::constructor(
    /* [in] */ IParcel * src)
{
    return E_NOT_IMPLEMENTED;
}
