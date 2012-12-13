
#include "view/inputmethod/CEditorInfo.h"

CEditorInfo::CEditorInfo():
	mInputType(InputType_TYPE_NULL),
	mImeOptions(EditorInfo_IME_NULL),
	mActionId(0),
	mInitialSelStart(-1),
	mInitialSelEnd(-1),
	mInitialCapsMode(0),
	//TOOD: add.
	mFieldId(0)
{

}

ECode CEditorInfo::constructor()
{
	return NOERROR;
}

ECode CEditorInfo::ReadFromParcel(
    /* [in] */ IParcel *source)
{
	assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CEditorInfo::WriteToParcel(
    /* [in] */ IParcel *dest)
{
	assert(0);
    return E_NOT_IMPLEMENTED;
}
