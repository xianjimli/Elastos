
#include "CFileURLConnection.h"
#include "Elastos.IO.h"

ECode CFileURLConnection::Connect()
{
    // TODO: Add your code here
    AutoPtr<IFile> f;
    ECode ec = NOERROR;

    ec = CFile::New(mFileName, (IFile**)&f);
    if (FAILED(ec)) {
        return ec;
    }

    Boolean isDir;
    f->IsDirectory(&isDir);
    if (isDir) {
        mIsDir = TRUE;
        GetDirectoryListing((IFile*)f, &mIs);
    }
    else {
        AutoPtr<IFileInputStream> fis;
        ec = CFileInputStream::New((IFile*)f, (IFileInputStream**)&fis);
        if (FAILED(ec)) {
            return ec;
        }

        ec = CBufferedInputStream::New((IFileInputStream*)fis, (IBufferedInputStream**)&mIs);
        if (FAILED(ec)) {
            return ec;
        }
        mIs->Available(&mLength);
    }

    mConnected = TRUE;
    return NOERROR;
}

ECode CFileURLConnection::GetAllowUserInteraction(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::GetContent(
    /* [out] */ IInterface ** ppContent)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::GetContentEncoding(
    /* [out] */ String * pEncoding)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::GetContentLength(
    /* [out] */ Int32 * pLength)
{
    // TODO: Add your code here
    ECode ec = NOERROR;
    if (!mConnected) {
        ec = Connect();
        if (FAILED(ec)) {
            return ec;
        }
    }

    *pLength = mLength;
    return NOERROR;
}

ECode CFileURLConnection::GetContentType(
    /* [out] */ String * pType)
{
    // TODO: Add your code here
    ECode ec = NOERROR;
    if (!mConnected) {
        ec = Connect();
        if (FAILED(ec)) {
            return ec;
        }
    }

    if (mIsDir) {
        *pType = String("text/plain");
        return NOERROR;
    }

    String file;
    mUrl->GetFile(&file);
    String result = GuessContentTypeFromName(file);
    if (!result.IsNull()) {
        *pType = result;
        return NOERROR;
    } else {
        result = GuessContentTypeFromStream(mIs);
    }

    if (!result.IsNull()) {
        *pType = result;
        return NOERROR;
    }

    *pType = String("content/unknown");
    return NOERROR;
}

ECode CFileURLConnection::GetDate(
    /* [out] */ Int64 * pDate)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::GetDefaultUseCaches(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::GetDoInput(
    /* [out] */ Boolean * pAllowed)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::GetDoOutput(
    /* [out] */ Boolean * pAllowed)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::GetExpiration(
    /* [out] */ Int64 * pExpiration)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::GetHeaderFieldByPosition(
    /* [in] */ Int32 pos,
    /* [out] */ String * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::AddRequestProperty(
    /* [in] */ const String& field,
    /* [in] */ const String& newValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::GetHeaderFieldByKey(
    /* [in] */ const String& key,
    /* [out] */ String * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::GetHeaderFieldDate(
    /* [in] */ const String& field,
    /* [in] */ Int64 defaultValue,
    /* [out] */ Int64 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::GetHeaderFieldInt32(
    /* [in] */ const String& field,
    /* [in] */ Int32 defaultValue,
    /* [out] */ Int32 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::GetHeaderFieldKey(
    /* [in] */ Int32 posn,
    /* [out] */ String * pKey)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::GetIfModifiedSince(
    /* [out] */ Int64 * pTime)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::GetInputStream(
    /* [out] */ IInputStream ** ppIs)
{
    // TODO: Add your code here
    ECode ec = NOERROR;
    if (!mConnected) {
        ec = Connect();
        if (FAILED(ec)) {
            return ec;
        }
    }
    *ppIs = mIs;
    return NOERROR;
}

ECode CFileURLConnection::GetLastModified(
    /* [out] */ Int64 * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::GetOutputStream(
    /* [out] */ IOutputStream ** ppOs)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::GetRequestProperty(
    /* [in] */ const String& field,
    /* [out] */ String * pProperty)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::GetURL(
    /* [out] */ IURL ** ppUrl)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::GetUseCaches(
    /* [out] */ Boolean * pAllowed)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::SetAllowUserInteraction(
    /* [in] */ Boolean newValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::SetDefaultUseCaches(
    /* [in] */ Boolean newValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::SetDoInput(
    /* [in] */ Boolean newValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::SetDoOutput(
    /* [in] */ Boolean newValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::SetIfModifiedSince(
    /* [in] */ Int64 newValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::SetRequestProperty(
    /* [in] */ const String& field,
    /* [in] */ const String& newValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::SetUseCaches(
    /* [in] */ Boolean newValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::SetConnectTimeout(
    /* [in] */ Int32 timeout)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::GetConnectTimeout(
    /* [out] */ Int32 * pTimeout)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::SetReadTimeout(
    /* [in] */ Int32 timeout)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::GetReadTimeout(
    /* [out] */ Int32 * pTimeout)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CFileURLConnection::constructor(
    /* [in] */ IURL* pUrl)
{
    // TODO: Add your code here
    URLConnection::Init(pUrl);

    pUrl->GetFile(&mFileName);

    if (mFileName.IsNull()) {
        mFileName = String("");
    }

    //fileName = Util.decode(fileName, false);
}

ECode CFileURLConnection::GetDirectoryListing(
    /* [in] */ IFile* file,
    /* [out] */ IInputStream** is)
{
    ArrayOf<String>* fileList;
    file->List(&fileList);

    AutoPtr<IByteArrayOutputStream> bous;

    CByteArrayOutputStream::New((IByteArrayOutputStream**)&bous);

    return NOERROR;
//    ByteArrayOutputStream bytes = new java.io.ByteArrayOutputStream();
//    PrintStream out = new PrintStream(bytes);
//    out.print("<title>Directory Listing</title>\n");
//    out.print("<base href=\"file:");
//    out.print(f.getPath().replace('\\', '/') + "/\"><h1>" + f.getPath()
//            + "</h1>\n<hr>\n");
//    int i;
//    for (i = 0; i < fileList.length; i++) {
//        out.print(fileList[i] + "<br>\n");
//    }
//    out.close();
//    return new ByteArrayInputStream(bytes.toByteArray());
}

CARAPI_(PInterface) CFileURLConnection::Probe(
    /* [in]  */ REIID riid)
{
}