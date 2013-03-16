#ifndef __CERTTOOL_H__
#define __CERTTOOL_H__

#include "ext/frameworkext.h"
#include <elastos/Vector.h>

class CertTool {
public:
	static const CString CERT;// = Credentials.CERTIFICATE;
    static const CString PKCS12;// = Credentials.PKCS12;

public:
    static CARAPI_(void) GetKeyStrengthList(
    	/* [in] */ Vector<String>& list);

    static CARAPI_(void) GetSignedPublicKey(
    	/* [in] */ IContext* context,
    	/* [in] */ Int32 index,
    	/* [in] */ const String& challenge,
    	/* [out] */ String& strOut);

    static CARAPI_(void) AddCertificate(
    	/* [in] */ IContext* context,
    	/* [in] */ const String& type,
    	/* [in] */ ArrayOf<Byte>& value);

private:
	static const CString LOGTAG;// = "CertTool";

//	static const AlgorithmIdentifier MD5_WITH_RSA =
//            new AlgorithmIdentifier(PKCSObjectIdentifiers.md5WithRSAEncryption);
private:
	CertTool() {}
};

#endif //__CERTTOOL_H__