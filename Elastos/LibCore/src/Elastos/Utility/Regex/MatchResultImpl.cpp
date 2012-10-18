
#include "MatchResultImpl.h"
#include "cmdef.h"

MatchResultImpl::MatchResultImpl(
    /* [in] */ const String& text,
    /* [in] */ ArrayOf<Int32>* offsets)
    : mText(text)
{
    mOffsets = offsets->Clone();
}

PInterface MatchResultImpl::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IMatchResult*)this;
    }
    else if (riid == EIID_IMatchResult) {
        return (IMatchResult*)this;
    }

    return NULL;
}

UInt32 MatchResultImpl::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 MatchResultImpl::Release()
{
    return ElRefBase::Release();
}

ECode MatchResultImpl::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode MatchResultImpl::End(
    /* [out] */ Int32* index)
{
    return EndEx(0, index);
}

ECode MatchResultImpl::EndEx(
    /* [in] */ Int32 group,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    *index = (*mOffsets)[2 * group + 1];

    return NOERROR;
}

ECode MatchResultImpl::Group(
    /* [out] */ String* text)
{
    VALIDATE_NOT_NULL(text);

    Int32 startId, endId;
    Start(&startId);
    End(&endId);
    *text = mText.Substring(startId, endId);

    return NOERROR;
}

ECode MatchResultImpl::GroupEx(
    /* [in] */ Int32 group,
    /* [out] */ String* text)
{
    VALIDATE_NOT_NULL(text);

    Int32 from = (*mOffsets)[group * 2];
    Int32 to = (*mOffsets)[(group * 2) + 1];
    if (from == -1 || to == -1) {
        *text = String(NULL);
    }
    else {
        *text = mText.Substring(from, to);
    }

    return NOERROR;
}

ECode MatchResultImpl::GroupCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    *count = (mOffsets->GetLength() / 2) - 1;

    return NOERROR;
}

ECode MatchResultImpl::StartEx(
    /* [in] */ Int32 group,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    *index = (*mOffsets)[2 * group];

    return NOERROR;
}

ECode MatchResultImpl::Start(
    /* [out] */ Int32* index)
{
    return Start(0);
}