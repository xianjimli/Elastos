
#include "webkit/CURLUtil.h"

#include <Logger.h>
//#include "net/CWebAddress.h"
#include <StringBuffer.h>
#include <Uri.h>
#include "webkit/CMimeTypeMap.h"
#include <elastos/AutoFree.h>

// to refer to bar.png under your package's asset/foo/ directory, use
// "file:///android_asset/foo/bar.png".
const CString CURLUtil::ASSET_BASE = "file:///android_asset/";
// to refer to bar.png under your package's res/drawable/ directory, use
// "file:///android_res/drawable/bar.png". Use "drawable" to refer to
// "drawable-hdpi" directory as well.
const CString CURLUtil::RESOURCE_BASE = "file:///android_res/";
const CString CURLUtil::FILE_BASE = "file://";
const CString CURLUtil::PROXY_BASE = "file:///cookieless_proxy/";

const CString CURLUtil::LOGTAG = "webkit";

//const AutoPtr<CPattern> CURLUtil::CONTENT_DISPOSITION_PATTERN = CPattern::Compile("attachment;\\s*filename\\s*=\\s*(\"?)([^\"]*)\\1\\s*$",CPattern::CASE_INSENSITIVE);

Int32 CURLUtil::ParseHex(
        /* [in] */ Byte b)
{
    Int32 retVal;
    if (b >= '0' && b <= '9') {
        retVal = (b - '0');
        return retVal;
    }
    if (b >= 'A' && b <= 'F') {
        retVal = ((b - 'A') + 10);
        return retVal;
    }
    if (b >= 'a' && b <= 'f') {
        retVal = ((b - 'a') + 10);
        return retVal;
    }

    Utility::Logging::Logger::E(LOGTAG, String("Invalid hex char '") + b + String("'") );
    return -1;//return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

String CURLUtil::GuessUrl(
    /* [in] */ const String& inUrl)
{
    String outUrl;
    String retVal = inUrl;

    Utility::Logging::Logger::V(LOGTAG, String("guessURL before queueRequest: ") + inUrl );

    if(inUrl.GetLength() == 0) {
        outUrl = inUrl;
        return outUrl;
    }
    if(inUrl.Substring(0,6) == String("about:")) {
        outUrl = inUrl;
        return outUrl;
    }
    // Do not try to interpret data scheme URLs
    if (inUrl.Substring(0,5) == String("data:")) {
        outUrl = inUrl;
        return outUrl;
    }
    // Do not try to interpret file scheme URLs
    if (inUrl.Substring(0,5) == String("file:")) {
        outUrl = inUrl;
        return outUrl;
    }
    // Do not try to interpret javascript scheme URLs
    if (inUrl.Substring(0,11) == String("javascript:")) {
        outUrl = inUrl;
        return outUrl;
    }

    // bug 762454: strip period off end of url
    String tInUrl;
    if (inUrl.Substring(inUrl.GetLength()-1,inUrl.GetLength()) == String(".") )  {
        tInUrl = inUrl.Substring(0, inUrl.GetLength() - 1);
    }

    /*
    AutoPtr<IWebAddress> webAddress;
    ECode ec = CWebAddress::New(tInUrl,(IWebAddress**)&webAddress);
    if(FAILED(ec)) {        
        if (DebugFlags::sURL_UTIL)  {
            Utility::Logging::Logger::V(LOGTAG, String("smartUrlFilter: failed to parse url = ") + tInUrl );
        }        
        outUrl = retVal;
        return outUrl;
    }    

    // Check host
    if ((webAddress -> mHost).Find(".") == -1) {
        // no dot: user probably entered a bare domain.  try .com
        webAddress -> mHost = String("www.") + (webAddress -> mHost) + String(".com");
    }
    webAddress -> ToString(&outUrl);
    */

    return outUrl;
}

String CURLUtil::ComposeSearchUrl(
    /* [in] */ const String& inQuery,
    /* [in] */ const String& strTemplate,
    /* [in] */ const String& queryPlaceHolder)
{
    String url;
    Int32 placeHolderIndex = strTemplate.IndexOf(queryPlaceHolder);
    if (placeHolderIndex < 0) {
        url = String(NULL);
        return url;
    }

    String query;
    StringBuffer buffer(NULL);
    buffer = strTemplate.Substring(0, placeHolderIndex);

    /*
    ECode ec = Elastos::URLEncoder::Encode(inQuery, "utf-8", &query);
    if(FAILED(ec)) {
        //JAVA:catch (UnsupportedEncodingException ex)        
        url = String(NULL);
        return url;
    }
    */
    
    buffer += query;
    buffer += strTemplate.Substring(placeHolderIndex + queryPlaceHolder.GetLength());

    url = (const char*)buffer;

    return url;
}

ECode CURLUtil::Decode(
    /* [in] */ const ArrayOf<Byte> & url,
    /* [out] */ ArrayOf<Byte> ** outUrl)
{
    VALIDATE_NOT_NULL(outUrl);
    if(url.GetLength() == 0) {
        *outUrl = NULL;
        return NOERROR;
    }

    // Create a new byte array with the same length to ensure capacity
    AutoFree< ArrayOf<Byte> > tempData = ArrayOf<Byte>::Alloc(url.GetLength());

    Int32 tempCount = 0;
    Int32 tHexH = 0,tHexL = 0;
    for(Int32 i = 0; i < url.GetLength(); i++) {
        Byte b = url[i];
        if(b == '%') {
            if(url.GetLength() - i > 2) {
                tHexH = ParseHex(url[i+1]);
                tHexL = ParseHex(url[i+2]);
                b = (byte) (tHexH * 16 + tHexL);
                i += 2;
            }
            else {
                ArrayOf<Byte>::Free(tempData);
                //JAVA:throw new IllegalArgumentException("Invalid format");
                Utility::Logging::Logger::E(LOGTAG, String("Invalid format") );
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }            
        }
        (*tempData)[tempCount++] = b;
    }

    *outUrl = tempData;
    // ArrayOf<Byte>::Free(*outUrl);
    // *outUrl = ArrayOf<Byte>::Alloc(tempCount);
    // for(Int32 i = 0; i < tempCount; i++) {
    //     (**outUrl)[i] = (*tempData)[i];
    // }
    // //ArrayOf<Byte>::Free(tempData);

    return NOERROR;
}

Boolean CURLUtil::IsAssetUrl(
    /* [in] */ const String& url)
{
    Boolean flag;
    flag = ((NULL != url) && (url.Substring(0,String(ASSET_BASE).GetLength()) == String(ASSET_BASE) ) );
    return flag;
}

Boolean CURLUtil::IsResourceUrl(
    /* [in] */ const String& url)
{
    Boolean flag;
    flag = ((NULL != url ) && (url.Substring(0,String(RESOURCE_BASE).GetLength()) == String(RESOURCE_BASE) ) );
    return flag;
}

Boolean CURLUtil::IsCookielessProxyUrl(
    /* [in] */ const String& url)
{
    Boolean flag;
    flag = ((NULL != url ) && (url.Substring(0,String(PROXY_BASE).GetLength()) == String(PROXY_BASE) ) );
    return flag;
}

Boolean CURLUtil::IsFileUrl(
    /* [in] */ const String& url)
{
    Boolean flag;
    flag = (NULL != url );
    flag = (flag) && (url.Substring(0,String(FILE_BASE).GetLength()) == String(FILE_BASE) );
    flag = (flag) && (!(url.Substring(0,String(ASSET_BASE).GetLength()) == String(ASSET_BASE) ) );
    flag = (flag) && (!(url.Substring(0,String(PROXY_BASE).GetLength()) == String(PROXY_BASE) ) );
    return flag;
}

Boolean CURLUtil::IsAboutUrl(
    /* [in] */ const String& url)
{
    Boolean flag;
    String strUrlFlag("about:");
    flag =  (NULL != url ) && (url.Substring(0,String(strUrlFlag).GetLength()) == String(strUrlFlag) );
    return flag;
}

Boolean CURLUtil::IsDataUrl(
    /* [in] */ const String& url)
{
    Boolean flag;
    String strUrlFlag("data:");
    flag =  (NULL != url ) && (url.Substring(0,String(strUrlFlag).GetLength()) == String(strUrlFlag) );
    return flag;
}

Boolean CURLUtil::IsJavaScriptUrl(
    /* [in] */ const String& url)
{
    Boolean flag;
    String strUrlFlag("javascript:");
    flag =  (NULL != url ) && (url.Substring(0,String(strUrlFlag).GetLength()) == String(strUrlFlag) );
    return flag;
}

Boolean CURLUtil::IsHttpUrl(
    /* [in] */ const String& url)
{
    Boolean flag;
    flag = (NULL != url);
    flag = (flag) && ( url.GetLength() > 6 );
    flag = (flag) && ( url.Substring(0,7).EqualsIgnoreCase("http://") );
    return flag;
}

Boolean CURLUtil::IsHttpsUrl(
    /* [in] */ const String& url)
{
    Boolean flag;
    flag = ( !(url.IsNull()) );
    flag = (flag) && ( url.GetLength() > 7 );
    flag = (flag) && ( url.Substring(0,8).EqualsIgnoreCase("https://") );
    return flag;
}

Boolean CURLUtil::IsNetworkUrl(
    /* [in] */ const String& url)
{
    Boolean flag;
    if (url == NULL || url.GetLength() == 0)  {
        flag = FALSE;
        return flag;
    }
    Boolean bHttp,bHttps;
    bHttp = IsHttpUrl(url);
    bHttps = IsHttpsUrl(url);
    flag =  bHttp || bHttps;
    return flag;
}

Boolean CURLUtil::IsContentUrl(
    /* [in] */ const String& url)
{
    Boolean flag;
    String strUrlFlag("content:");
    flag =  (NULL != url ) && (url.Substring(0,String(strUrlFlag).GetLength()) == String(strUrlFlag) );
    return flag;
}

Boolean CURLUtil::IsValidUrl(
    /* [in] */ const String& url)
{
    if (url == NULL || url.GetLength() == 0)  {
        return FALSE;
    }
    Boolean bAssetUrl,bResourceUrl,bFileUrl,bAboutUrl,bHttpUrl,bHttpsUrl,bJavaScriptUrl,bContentUrl;
    bAssetUrl = IsAssetUrl(url);
    bResourceUrl = IsResourceUrl(url);
    bFileUrl = IsFileUrl(url);
    bAboutUrl = IsAboutUrl(url);
    bHttpUrl = IsHttpUrl(url);
    bHttpsUrl = IsHttpsUrl(url);
    bJavaScriptUrl = IsJavaScriptUrl(url);
    bContentUrl = IsContentUrl(url);
    return bAssetUrl || bResourceUrl || bFileUrl || bAboutUrl || bHttpUrl || bHttpsUrl || bJavaScriptUrl || bContentUrl;
}

String CURLUtil::StripAnchor(
    /* [in] */ const String& url)
{
    Int32 anchorIndex = url.IndexOf('#');
    if (anchorIndex != -1)  {
        return url.Substring(0, anchorIndex);
    }
    return url;
}

String CURLUtil::GuessFileName(
    /* [in] */ const String& url,
    /* [in] */ const String& contentDisposition,
    /* [in] */ const String& mimeType)
{
    String filename(NULL);
    String extension(NULL);

    // If we couldn't do anything with the hint, move toward the content disposition
    if (filename.IsNullOrEmpty() && (!contentDisposition.IsNullOrEmpty()) )  {        
        filename = ParseContentDisposition(contentDisposition);
        if (!filename.IsNullOrEmpty())  {
            Int32 index = filename.LastIndexOf('/') + 1;
            if (index > 0)  {
                filename = filename.Substring(index);
            }
        }
    }

    // If all the other http-related approaches failed, use the plain uri
    if (filename.IsNullOrEmpty())  {
        String decodedUrl(NULL);
        Uri::Decode(url,&decodedUrl);
        if (decodedUrl.IsNullOrEmpty())  {
            Int32 queryIndex = decodedUrl.IndexOf('?');
            // If there is a query string strip it, same as desktop browsers
            if (queryIndex > 0)  {
                decodedUrl = decodedUrl.Substring(0, queryIndex);
            }
            if (!decodedUrl.EndWith("/"))  {
                Int32 index = decodedUrl.LastIndexOf('/') + 1;
                if (index > 0)  {
                    filename = decodedUrl.Substring(index);
                }
            }
        }
    }

    // Finally, if couldn't get filename from URI, get a generic filename
    if (filename.IsNullOrEmpty()) {
        filename = "downloadfile";
    }

    // Split filename between base and extension
    // Add an extension if filename does not have one
    Int32 dotIndex = filename.IndexOf('.');
    if (dotIndex < 0)  {
        if (!mimeType.IsNullOrEmpty())  {
            //(CMimeTypeMap::GetSingleton()) -> GetExtensionFromMimeType(mimeType,&extension);
            if (extension.IsNullOrEmpty())  {
                extension = String(".") + extension;
            }
        }
        if (extension.IsNullOrEmpty())  {
            if ((!mimeType.IsNullOrEmpty()) && ((mimeType.Substring(0,5)).EqualsIgnoreCase("text/")) )  {
                if (mimeType.EqualsIgnoreCase("text/html"))  {
                    extension = ".html";
                } 
                else  {
                    extension = ".txt";
                }
            } 
            else  {
                extension = ".bin";
            }
        }        
    } 
    else  {
        if (!mimeType.IsNullOrEmpty())  {
            // Compare the last segment of the extension against the mime type.
            // If there's a mismatch, discard the entire extension.            
            String typeFromExt(NULL);
            Int32 lastDotIndex = filename.LastIndexOf('.');
            //(CMimeTypeMap::GetSingleton()) -> GetMimeTypeFromExtension(filename.substring(lastDotIndex + 1),&typeFromExt);
            if ((!typeFromExt.IsNullOrEmpty()) && (!typeFromExt.EqualsIgnoreCase(mimeType)) )  {
                //(CMimeTypeMap::GetSingleton()) -> GetExtensionFromMimeType(mimeType,&extension);
                if (!extension.IsNullOrEmpty())  {
                    extension = String(".") + extension;
                }
            }
        }
        if (extension.IsNullOrEmpty())  {
            extension = filename.Substring(dotIndex);
        }
        filename = filename.Substring(0, dotIndex);
    }

    return filename + extension;
}

Boolean CURLUtil::VerifyURLEncoding(
        /* [in] */ String url)
{
    Int32 count = url.GetLength();
    if(count == 0) {
        return FALSE;
    }

    int index = url.IndexOf('%');
    while (index >= 0 && index < count)  {
        if (index < (count - 2) )  {
            //JAVA:  try{
            ParseHex((Byte) url.GetChar(++index));
            ParseHex((Byte) url.GetChar(++index));
            //JAVA:  }            
            //JAVA:  catch (IllegalArgumentException e)
            //JAVA:  {
            //     return FALSE;
            //     //return E_ILLEGAL_ARGUMENT_EXCEPTION;                    
            //JAVA:  }            
        }
        else {
            return FALSE;
        }
        index = url.IndexOf('%', index + 1);
    }
    return TRUE;    
}

String CURLUtil::ParseContentDisposition(
        /* [in] */ String contentDisposition)
{
    /*
    AutoPtr<IMatcher> m;
    CONTENT_DISPOSITION_PATTERN -> Matcher(contentDisposition,(IMatcher**)&m);
    Boolean bMatchFound;
    ECode ec = m -> Find(&bMatchFound);
    if(FAILED(ec)) {
        return NULL;
        //return ec;  //return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (bMatchFound) {
        String retVal;
        ec = m -> Group(&retVal);
        if(FAILED(ec)) {
            return NULL;
            //return ec;  //return E_ILLEGAL_STATE_EXCEPTION;
        }
        return retVal;
    }
    return NULL;
    */
}

