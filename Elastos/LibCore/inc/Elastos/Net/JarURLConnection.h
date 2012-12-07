
#ifndef __JARURLCONNECTION_H__
#define __JARURLCONNECTION_H__

#include "URLConnection.h"

class JarURLConnection : public URLConnection
{
public:
    //virtual CARAPI GetAttributes(
    //    /* [out] */ IAttributes** attr);

    virtual CARAPI GetCertificates(
        /* [out] */ IObjectContainer** certs);

    virtual CARAPI GetEntryName(
        /* [out] */ String* entryName);

    //virtual CARAPI GetJarEntry(
    //    /* [out] */ IJarEntry** jarEntry);

    //virtual CARAPI GetManifest(
    //    /* [out] */ IManifest** manifest);

    //virtual CARAPI GetJarFile(
    //    /* [out] */ IJarFile** jarFile) = 0;

    virtual CARAPI GetJarFileURL(
        /* [out] */ IURL** url);

    //virtual CARAPI GetMainAttributes(
    //    /* [out] */ IAttributes** attr);

protected:
    CARAPI Init(
        /* [in] */ IURL* url);

protected:
    /**
     * The location part of the represented URL.
     */
    AutoPtr<IURLConnection> mJarFileURLConnection;

private:
    String mEntryName;

    AutoPtr<IURL> mFileURL;

    // the file component of the URL
    String mFile;
};

#endif //__JARURLCONNECTION_H__
