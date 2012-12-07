
#include "cmdef.h"
#include "JarURLConnection.h"
#include "CURL.h"

//ECode JarURLConnection::GetAttributes(
//    /* [out] */ IAttributes** attr)
// {
//     VALIDATE_NOT_NULL(attr);
//     AutoPtr<IJarEntry> jEntry;
//     ASSERT_SUCCEEDED(GetJarEntry((IJarEntry**)&jEntry));
//     if (jEntry == NULL) {
//         *attr = NULL;
//         return NOERROR;
//     }
//     else {
//         return jEntry->GetAttributes(attr);
//     }
// }

ECode JarURLConnection::GetCertificates(
    /* [out] */ IObjectContainer** certs)
{
    // VALIDATE_NOT_NULL(certs);
    // AutoPtr<IJarEntry> jEntry;
    // ASSERT_SUCCEEDED(GetJarEntry((IJarEntry**)&jEntry));
    // if (jEntry == null) {
    //     *certs = NULL;
    //     return NOERROR;
    // }

    // return jEntry->GetCertificates(certs);
    return E_NOT_IMPLEMENTED;
}

ECode JarURLConnection::GetEntryName(
    /* [out] */ String* entryName)
{
    VALIDATE_NOT_NULL(entryName);
    *entryName = mEntryName;
    return NOERROR;
}

//ECode JarURLConnection::GetJarEntry(
//    /* [out] */ IJarEntry** jarEntry)
// {
//     VALIDATE_NOT_NULL(jarEntry);

//     if (!mConnected) {
//         Connect();
//     }
//     if (mEntryName.IsNull()) {
//         *jarEntry = NULL;
//         return NOERROR;
//     }
//     // The entry must exist since the connect succeeded
//     AutoPtr<IJarFile> jarFile;
//     ASSERT_SUCCEEDED(GetJarFile((IJarFile**)jarFile));
//     if (jarFile) {
//         return jarFile->GetJarEntry(mEntryName, jarEntry);
//     }

//     return NOERROR;
// }

//ECode JarURLConnection::GetManifest(
//    /* [out] */ IManifest** manifest)
// {
//     return (Manifest)getJarFile().getManifest().clone();
// }

ECode JarURLConnection::GetJarFileURL(
    /* [out] */ IURL** url)
{
    VALIDATE_NOT_NULL(url);
    *url = mFileURL;
    return NOERROR;
}

//ECode JarURLConnection::GetMainAttributes(
//    /* [out] */ IAttributes** attr)
// {
//     VALIDATE_NOT_NULL(attr);

//     AutoPtr<IJarFile> f;
//     ASSERT_SUCCEEDED(GetJarFile((IJarFile**)&f));
//     AutoPtr<IManifest> m;
//     F->GetManifest((IManifest**)&m);
//     if (m == NULL) {
//         *attr = NULL;
//         return NOERROR;
//     }
//     else {
//         return m->GetMainAttributes(attr);
//     }
// }

ECode JarURLConnection::Init(
    /* [in] */ IURL* url)
{
    JarURLConnection::Init(url);
    url->GetFile(&mFile);
    UInt32 sepIdx;
    if ((sepIdx = mFile.IndexOf("!/")) < 0) {
        return E_MALFORMED_URL_EXCEPTION;
//        throw new MalformedURLException();
    }
    CURL::New(mFile.Substring(0,sepIdx), (IURL**)&mFileURL);
    sepIdx += 2;
    if (mFile.GetLength() == sepIdx) {
        return NOERROR;
    }
    mEntryName = mFile.Substring(sepIdx, mFile.GetLength());
    String ref;
    url->GetRef(&ref);
    if (!ref.IsNull()) {
        mEntryName += String("#") + ref;
    }

    return NOERROR;
}
