
#include "view/CInputBindResult.h"


CInputBindResult::CInputBindResult()
{

}

ECode CInputBindResult::constructor(
	/* [in] */ IInputMethodSessionStub* _method,
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
    return E_NOT_IMPLEMENTED;
}

ECode CInputBindResult::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    return E_NOT_IMPLEMENTED;
}

ECode CInputBindResult::constructor(
	/* [in] */ IParcel* source)
{
	//TODO
    // mMethod = IInputMethodSession.Stub.asInterface(source.readStrongBinder());
    // mId = source.readString();
    // mSequence = source.readInt();

    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CInputBindResult::GetIIMSessionStub(
    /* [out] */ IInputMethodSessionStub** stub)
{
    assert(stub != NULL);

    *stub = mMethod;
    if (*stub != NULL) {
        (*stub)->AddRef();
    }

    return NOERROR;
}
