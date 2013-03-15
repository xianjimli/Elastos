
#include "app/backup/CRestoreSet.h"

CRestoreSet::CRestoreSet()
{

}

CRestoreSet::~CRestoreSet()
{

}

ECode CRestoreSet::constructor()
{
	return NOERROR;
}

ECode CRestoreSet::constructor(
    /* [in] */ const String& name,
    /* [in] */ const String& dev,
    /* [in] */ Int64 token)
{
	mName = name;
	mDevice = dev;
	mToken = token;

	return NOERROR;
}

ECode CRestoreSet::DescribeContents(
	/* [out] */ Int32* c)
{
	assert(c != NULL);
	*c = 0;
    return NOERROR;
}

ECode CRestoreSet::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRestoreSet::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
}
