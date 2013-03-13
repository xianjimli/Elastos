
#include "webkit/CertTool.h"

const String CertTool::CERT;// = Credentials.CERTIFICATE;
const String CertTool::PKCS12;// = Credentials.PKCS12;

const char* CertTool::LOGTAG = "CertTool";

CARAPI_(void) CertTool::GetKeyStrengthList(
	/* [in] */ Vector<String> list)
{}

CARAPI_(void) CertTool::GetSignedPublicKey(
	/* [in] */ IContext* context,
	/* [in] */ Int32 index,
	/* [in] */ const String& challenge,
	/* [out] */ String& strOut)
{}

CARAPI_(void) CertTool::AddCertificate(
	/* [in] */ IContext* context,
	/* [in] */ const String& type,
	/* [in] */ Vector<Byte>& value)
{}
