
#include "ext/frameworkdef.h"
#include "view/CInputBindResult.h"


ECode CInputBindResult::constructor()
{
    return NOERROR;
}

ECode CInputBindResult::constructor(
	/* [in] */ IInputMethodSession* _method,
	/* [in] */ const String& _id,
	/* [in] */ Int32 _sequence)
{
    mMethod = _method;
    mId = _id;
    mSequence = _sequence;
    return NOERROR;
}

ECode CInputBindResult::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    AutoPtr<IInterface> obj;
    source->ReadInterfacePtr((Handle32*)&obj);
    if (obj != NULL) {
        mMethod = IInputMethodSession::Probe(obj);
    }
    source->ReadString(&mId);
    source->ReadInt32(&mSequence);
    return NOERROR;
}

ECode CInputBindResult::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    dest->WriteInterfacePtr((IInterface*)mMethod.Get());
    dest->WriteString(mId);
    dest->WriteInt32(mSequence);
    return NOERROR;
}

ECode CInputBindResult::GetIIMSession(
    /* [out] */ IInputMethodSession** session)
{
    VALIDATE_NOT_NULL(session);

    *session = mMethod;
    if (*session != NULL) {
        (*session)->AddRef();
    }
    return NOERROR;
}

ECode CInputBindResult::GetId(
    /* [out] */ String* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mId;

    return NOERROR;
}

ECode CInputBindResult::GetSequence(
    /* [out] */ Int32* sequence)
{
    VALIDATE_NOT_NULL(sequence);
    *sequence = mSequence;

    return NOERROR;
}
